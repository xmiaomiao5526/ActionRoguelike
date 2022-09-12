// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopted: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//始终保证调用StopAction时，bIsRunning为真
	ensureAlways(RepData.bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

USActionComponent* USAction::GetOwningComponent() const
{
	//return Cast<USActionComponent>(GetOuter());
	return ActionComp;
}

class UWorld* USAction::GetWorld() const
{
	//修改Outer为Actor
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	USActionComponent* Comp = GetOwningComponent();

	//HasAny还会检查父类，例如：{A.1}.HasAny{A,B},return true，若不想检查父类，使用HasAnyExact
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}