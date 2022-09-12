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
		//���ȡ��Ϊ��Asset�а󶨵�Blackboard
		//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Mypawn->GetActorLocation());

		//��Ϊ���е�MoveTo������Actorȡ��Location��������ƶ���AIҲ��׷�ٵ���ȷλ��
		//GetBlackboardComponent()->SetValueAsObject("TargetActor", Mypawn);
	}
}
