// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_RangedAttack.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

UMyBTTask_RangedAttack::UMyBTTask_RangedAttack()
{
	MAxBulletSpread = 2.f;
}


EBTNodeResult::Type UMyBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//��Ϊ�������ж���ͨ��������AIController����
	AAIController* MyController =  OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor==nullptr)
		{
			return EBTNodeResult::Failed;
		}
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();
		
		//�������AI�Ĺ���û��ô׼
		MuzzleRotation.Pitch += FMath::RandRange(0.f, MAxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MAxBulletSpread, MAxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, MyPawn->GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

