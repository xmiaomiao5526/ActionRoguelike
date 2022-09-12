// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree,TEXT("Behavior Tree is nullptr! Please assign Behavior Tree in you AI Controller!")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	APawn* Mypawn =  UGameplayStatics::GetPlayerPawn(this,0);
	if (Mypawn)
	{
		//会获取行为树Asset中绑定的Blackboard
		//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Mypawn->GetActorLocation());

		//行为树中的MoveTo可以用Actor取代Location，让玩家移动后，AI也能追踪到正确位置
		//GetBlackboardComponent()->SetValueAsObject("TargetActor", Mypawn);
	}
}
