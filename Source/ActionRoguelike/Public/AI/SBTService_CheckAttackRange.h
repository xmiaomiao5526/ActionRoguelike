// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:
	//创建一个下拉菜单，其中可以选择黑板中的变量
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector AttackRangeKey;

	//作为一个Service，TickNode必须有
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};