// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile() 
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComp->InitialSpeed = 600.f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	//��Actor������ʱ������һ����ʱ�������ڷ��У���ʱ������ʱ���������ø����Explode������ǰ����д���ģ�
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}


void ASDashProjectile::Explode_Implementation()
{
	//����ײ��ĳ��ʱ��ɾ�����м�ʱ��
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	//�ص���Ч�������ƶ���ȡ����ײ�����ڴ�������Hit��overlap�¼�
	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	//����Ҫ����ǰȡ�������ʱ��������FTimerHandleû��д��ͷ�ļ���,��̫���������ʱ�������ã�������Ϊ�˷�����Ӷ�����������Ҳ����
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//ʹ��TeleportTo�������������ȫһ�㣬���з�װ���ڶ��飬�������ͺ�Ŀռ䡢��ײ��
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	Destroy();
}
