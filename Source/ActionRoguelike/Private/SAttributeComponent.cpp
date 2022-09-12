// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	//在Component中用SetIsReplicatedByDefault代替SetReplicated
	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		//若在服务器中被调用，则自己和所有客户端都会调用；若在客户端中被调用，则只在客户端中调用
		if (ActualDelta != 0.f)
		{
			MuticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		if (ActualDelta < 0.f && Health == 0.f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f; 
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	//COND_OwnerOnly:当HealthMax在游戏中发生变化时，只有Owner能接收到，作为网络优化的手段
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax,COND_OwnerOnly);

}

void USAttributeComponent::MuticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this,NewHealth, Delta);
}

