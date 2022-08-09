// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float DetonateDelay;

	//����ǰײ������ʱ������ȡ����ʱ���������д��ͷ�ļ���
	FTimerHandle TimerHandle_DelayedDetonate;

	//��ײ����Ʒʱ�ᱻ���ã������ڸ������˺�BlueprintNativeEvent����дʱ��Ҫ�Ӻ�׺_Implementation
	virtual void Explode_Implementation() override;

	void TeleportInstigator();
public:
	ASDashProjectile();
};
