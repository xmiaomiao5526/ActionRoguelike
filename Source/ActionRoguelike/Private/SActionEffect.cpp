// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}


void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration>0.f)
	{
		//在Buff作用时间结束后，调用StopAction
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	if (Period > 0.f)
	{
		//在buff的持续时间内，每隔Period时间，都会执行一次ExecutePeriodicEffect
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//当Buff剩余时间接近于0的时候，此时若停止行为，依然会产生一次效果
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	//调用父类，包括RemoveTags
	Super::StopAction_Implementation(Instigator);

	//应清除两个Handle，可使Buff从外部被清除
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		//Comp->RemoveAction：检查ActionToRemove是否为空以及是否正在运行，皆为否则将其从Action Array中移除
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	//USActionEffect作为其余Buff父类，因此这里可为空，在子类中重写
}
