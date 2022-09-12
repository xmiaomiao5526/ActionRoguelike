// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("Sound");
	AudioComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	SetReplicates(true);
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	//击中则会调用Explode函数，只需在子类中重写Explode即可。
	Explode();
}

void ASProjectileBase::Explode_Implementation()
{
	//检查并确保没有提前被销毁
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
