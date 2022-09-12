// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* MyCharacter = Cast<ACharacter>(Instigator);
	if (MyCharacter)
	{
		MyCharacter->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, MyCharacter->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if (MyCharacter->HasAuthority())
		{
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", MyCharacter);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}

	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		//同PrimaryAttack_TimeElapsed函数，对生成物进行初始化，包括位置、方向、生成方式、Instigator
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		//从摄像机出发的碰撞检测，检测摄像机方向上的WorldDynamic、WorldStatic、Pawn
		FCollisionShape shape;
		shape.SetSphere(20.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		//traceEnd的默认初值
		FVector traceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 2500);

		FHitResult Hit;

		//如果光线追踪检测到了感兴趣的物体，将traceEnd的值修改为物体的值
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, traceEnd, FQuat::Identity, ObjParams, shape, QueryParams))
		{
			traceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(traceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}


