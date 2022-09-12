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
	//�˺�������Ӱ󶨣���ָ�뽫��Widget��ͼ��ͬ��ͬ���͵�С���
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	//meta�д������ã�������ͼ�д����ýڵ�ʱ�����˱�����Ϊ����
	UPROPERTY(BlueprintReadWrite,Category = "UI",meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;
};
