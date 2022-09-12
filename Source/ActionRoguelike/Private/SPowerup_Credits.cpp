// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Credits.h"
#include "SPlayerState.h"


ASPowerup_Credits::ASPowerup_Credits()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	//UPROPERTY(EditAnywhere,Category = "Credits")

	CreditsAmount = 80;
}


void ASPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ASPlayerState *PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HidenAndCooldownPowerup();
	}
}
