// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "SActionEffect.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");
	//UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	//If true, this projectile will have its rotation updated each frame to match the direction of its velocity.
	MovementComp->bRotationFollowsVelocity = true;

	//If true, the initial Velocity is interpreted as being in local space upon startup.
	MovementComp->bInitialVelocityInLocalSpace = true;

	Damage = 20;

	SetReplicates(true);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor!= GetInstigator())
	{
		//�ڵ���HasTagʱ����������FName����HasTag("Parrying")���������°취����Ӳ����
		
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Parrying");
		//HasTag(Tag);

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			//��֮ǰ������MovementComp->bRotationFollowsVelocity = true������ٶȷ�תʱ��RotationҲ�ᷴת
			MovementComp->Velocity = -MovementComp->Velocity;

			//�ı�ɵ���Instigator��ʹ�俴���������������ܶ�AI����˺�����return����ִ�к����ı�ը
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult)
			&& USGameplayFunctionLibrary::ApplyRage(GetInstigator(), OtherActor, Damage))
		{
			
			Destroy();
		}
		if (ActionComp)
		{
			ActionComp->AddAction(GetInstigator(), BurningActionClass);
		}

	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

