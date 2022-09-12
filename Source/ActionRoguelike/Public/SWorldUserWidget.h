// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//此宏用于添加绑定，该指针将绑定Widget蓝图中同名同类型的小组件
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	//meta中代码作用：当在蓝图中创建该节点时，将此变量作为引脚
	UPROPERTY(BlueprintReadWrite,Category = "UI",meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;
};
