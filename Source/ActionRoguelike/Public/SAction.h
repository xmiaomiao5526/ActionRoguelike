// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

/**
 * 
 */
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	//ͨ��������һ���ṹ���ʱ������Replicated�����Խṹ�������еı�����������Ҫ����Replicated��Ҳ����и���
	//������ṹ���еı��������ƣ������UPROPERTY(NotReplicated)
	//�ṹ���б��������紫��ʱ��ͬʱ����

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};



//Blueprintable��ʹ�ڴ�����ͼ��ʱ���Դ���Ϊ����
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:
	//����ΪReplicated������ͻ�����Ȼ�ᱨ��
	UPROPERTY(Replicated)
	class USActionComponent* ActionComp;

	//����Ϊ����ʱ����ӵ�Tags������Ϊֹͣʱ����ЩTags�Ƴ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//������ֹ�ж���Tags����û����ЩTagsʱ���ж����ܿ�ʼ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable,Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
public:

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const { return RepData.bIsRunning; };

	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool CanStart(AActor* Instigator);

	//��ΪUCLASS(Blueprintable)����д��Ϊ������ͼ�з���ʱ��GetWorld����ȷʹ��
	class UWorld* GetWorld() const override;


	bool IsSupportedForNetworking() const override
	{
		return true;
	}
}; 