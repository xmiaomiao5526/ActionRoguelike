// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,  NodeMemory, DeltaSeconds);

	//Check distance between ai pawn and target actor
	UBlackboardComponent* BlacBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlacBoardComp))
	{
		AActor* TargetAcotor = Cast<AActor>(BlacBoardComp->GetValueAsObject("TargetActor"));
		if (TargetAcotor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistenceTo = FVector::Distance(TargetAcotor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistenceTo < 2000.f;

					//�ж��Ƿ��ܿ�����ɫ������������ڽ�ɫ�������������ж�
					bool bHasLOS = false;
					if (bWithinRange)
					{
						//�ú�������һϵ�й���׷�٣����۾�����������������Ƿ��赲��͸����Ӱ�죩��
						bHasLOS = MyController->LineOfSightTo(TargetAcotor);
					}
					BlacBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bHasLOS && bWithinRange));
				}
			}
		}
	}
}
