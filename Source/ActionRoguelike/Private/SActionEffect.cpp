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
		//��Buff����ʱ������󣬵���StopAction
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	if (Period > 0.f)
	{
		//��buff�ĳ���ʱ���ڣ�ÿ��Periodʱ�䣬����ִ��һ��ExecutePeriodicEffect
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//��Buffʣ��ʱ��ӽ���0��ʱ�򣬴�ʱ��ֹͣ��Ϊ����Ȼ�����һ��Ч��
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	//���ø��࣬����RemoveTags
	Super::StopAction_Implementation(Instigator);

	//Ӧ�������Handle����ʹBuff���ⲿ�����
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		//Comp->RemoveAction�����ActionToRemove�Ƿ�Ϊ���Լ��Ƿ��������У���Ϊ�������Action Array���Ƴ�
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	//USActionEffect��Ϊ����Buff���࣬��������Ϊ�գ�����������д
}
