// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	
protected:
	//持续时间
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Effect")
	float Duration;

	//周期性作用效果的时间间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	//在头文件中定义FTimerHandle，方便在外部提前结束计时器
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent,Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
