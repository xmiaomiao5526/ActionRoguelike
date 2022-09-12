// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UEnvQuery* SpawnBotQuery;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	ASGameModeBase();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsPerKill;

	UFUNCTION(Exec)
	void KillAll();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	class UEnvQuery* PoweupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 DesirePowerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float RequirePowerupDistance;

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
