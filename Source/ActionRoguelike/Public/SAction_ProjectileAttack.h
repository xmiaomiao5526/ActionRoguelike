// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	class UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_AttackDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:
	USAction_ProjectileAttack();

	void StartAction_Implementation(AActor* Instigator) override;

};
