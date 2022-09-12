// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamgeCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamgeCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyRage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyRage(DamgeCauser, DamageAmount);
	}

	return false;
}
