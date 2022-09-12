// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//AttachedActor中会调用Destroy函数，在检查调用了Destroy的对象指针是否存在时，用IsValid会好点，直接判断指针是否存在会导致某些时候崩溃
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		return;
	}

	FVector2D ScreenPosiition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosiition))
	{
		//同蓝图中一样，屏幕修正放缩比例
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosiition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosiition);
		}
	}
}
