// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Animation")
	bool bIsStunned;

	void NativeInitializeAnimation() override;

	//¿‡À∆”⁄Tick
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
