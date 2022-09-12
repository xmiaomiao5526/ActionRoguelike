// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//AttachedActor�л����Destroy�������ڼ�������Destroy�Ķ���ָ���Ƿ����ʱ����IsValid��õ㣬ֱ���ж�ָ���Ƿ���ڻᵼ��ĳЩʱ�����
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		return;
	}

	FVector2D ScreenPosiition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosiition))
	{
		//ͬ��ͼ��һ������Ļ������������
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosiition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosiition);
		}
	}
}
