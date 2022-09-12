// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

UCLASS(ABSTRACT)//Abstract标记指明该类是抽象基类，是不能被直接创建出来的。将其排除在某些下拉窗口之外，如SpawnActor
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	UPROPERTY(EditDefaultsOnly,Category = "Effects")
	class UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UAudioComponent* AudioComp;
	//设为virtual可以在子类中重写该函数
	UFUNCTION()
	virtual	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;
public:	

};
