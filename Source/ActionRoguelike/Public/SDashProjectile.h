// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float DetonateDelay;

	//当提前撞到东西时，用于取消计时器。因此需写进头文件里
	FTimerHandle TimerHandle_DelayedDetonate;

	//当撞到物品时会被调用，且由于父类用了宏BlueprintNativeEvent，重写时需要加后缀_Implementation
	virtual void Explode_Implementation() override;

	void TeleportInstigator();
public:
	ASDashProjectile();
};
