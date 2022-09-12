// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";

	TargetActorKey = "TargetActor";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		//用一个FName变量TargetActorKey来取代硬编码
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		USWorldUserWidget* NewWidget = nullptr;
		if (SpottedWidgetClass)
		{
			NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		}
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
			// May end up behind the minion health bar otherwise.
			NewWidget->AddToViewport(10);
		}
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorAcotr != this)
		{
			SetTargetActor(InstigatorAcotr);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				//AddToVieprot将会调用Widget的构造函数，蓝图中是Event Construct，因此需要在此之前指定AttachedActor，否则其为空指针
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		//当AI死亡时，应做3件事：停止行为树、将模型设为Ragdoll、设置Lifespan，在某时间后从Map中移除该Actor
		if (NewHealth <= 0.f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				//BrainComponent是行为树的基类
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//若仅设置NoCollision，会导致与地面也不会发生碰撞，此时MovementComponent会施加重力使其跌落地面，因此还需DisableMovement
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//set lifespan
			SetLifeSpan(10.f);
		}
	}
}

