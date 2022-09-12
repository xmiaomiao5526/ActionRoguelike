// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMyBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	//必备重载函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float MAxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<AActor> ProjectileClass;

public:
	UMyBTTask_RangedAttack();
};
