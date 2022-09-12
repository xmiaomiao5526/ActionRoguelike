// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable,Category = "Attribute")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static bool IsActorAlive(AActor* Actor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated, Category = "Attribute")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float RageMax;



	//HealthMax,Stamina,Strength

	//实际上作为Unreliable更为合理
	UFUNCTION(NetMulticast,Reliable) // @FIXME: mark as unreliable once we moved the 'state' out of scharacter
	void MuticastHealthChanged(AActor* InstigatorActor,float NewHealth,float Delta);

public:	
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsFullHealth() const;

	float GetHealthMax() const;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable,Category="Attribute")
	bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable)
	float GetRage() const{return Rage;};

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
};
