// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable,Category = "Gameplay")
	static bool ApplyDamage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount);

	//加入BlueprintCallable后，函数中的引用参数会作为蓝图节点中的输出引脚，除非加const
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyRage(AActor* DamgeCauser, AActor* TargetActor, float DamageAmount);
};
