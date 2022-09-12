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
	//当Actor被生成时，设置一个计时器，用于飞行，计时器结束时，产生调用父类的Explode（被当前类重写过的）
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}


void ASDashProjectile::Explode_Implementation()
{
	//用于撞到某物时，删除飞行计时器
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	//关掉特效，不再移动、取消碰撞，不在触发其他Hit和overlap事件
	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	//不需要再提前取消这个计时器，所以FTimerHandle没有写进头文件里,不太明白这个计时器的作用，可能是为了方便添加动画，或许不加也可以
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//使用TeleportTo来传送人物更安全一点，其中封装了众多检查，包括传送后的空间、碰撞等
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	Destroy();
}
