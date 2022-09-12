// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
} 


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}



void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

		// Draw All Actions
	for (USAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	//创建一个ActionClass类的对象，用父类USAction的指针指向它
	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);
		\
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	//遍历所有Action，根据ActionName以找到需要启动的Action
	for (USAction* Action:Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				//用continue，而非直接return，主要是为了后期可能会拓展多个Action而考虑
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				//如果角色是客户端，调用该函数
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	//同样遍历整个Action Array
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}

			if (!GetOwner()->HasAuthority())
			{
				//如果角色是客户端，调用该函数
				ServerStopAction(Instigator, ActionName);
			}
		}
	}
	return false;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	//当UE中默认需要复制的发现变化时，WroterSomething为true
	bool WroterSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action:Actions)
	{
		if (Action)
		{
			//当其他默认需复制的，或Action发生变化时，WroterSomething为true
			WroterSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroterSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}