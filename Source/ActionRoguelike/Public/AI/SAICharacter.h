// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	class USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere,Category = "Effects")
	FName TimeToHitParamName;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	UPROPERTY(VisibleAnywhere,Category = "Components")
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USAttributeComponent* AttributeComp;

	void SetTargetActor(AActor* NewTarget);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USActionComponent* ActionComp;



public:	
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
