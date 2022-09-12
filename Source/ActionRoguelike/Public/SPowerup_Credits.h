// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category = "Credits")
	int32 CreditsAmount = 80;

	UPROPERTY(EditAnywhere, Category = "Components")
	 UStaticMeshComponent *MeshComp ;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	ASPowerup_Credits();
};
