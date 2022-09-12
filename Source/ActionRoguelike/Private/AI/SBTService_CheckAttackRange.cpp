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

					//判断是否能看见角色，如果甚至不在角色附近，则无需判断
					bool bHasLOS = false;
					if (bWithinRange)
					{
						//该函数做了一系列光线追踪，从眼睛处出发，检查视线是否被阻挡（透明不影响）。
						bHasLOS = MyController->LineOfSightTo(TargetAcotor);
					}
					BlacBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bHasLOS && bWithinRange));
				}
			}
		}
	}
}
