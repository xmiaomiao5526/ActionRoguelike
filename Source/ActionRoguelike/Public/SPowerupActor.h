// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerupActor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere,Category="Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerup();

	void HidenAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComp;

};
