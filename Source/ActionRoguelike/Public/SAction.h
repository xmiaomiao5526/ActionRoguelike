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
	//通常在声明一个结构体的时候会加上Replicated，所以结构体中所有的变量，都不需要加上Replicated，也会进行复制
	//若不想结构体中的变量被复制，因加上UPROPERTY(NotReplicated)
	//结构体中变量在网络传输时将同时到达

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};



//Blueprintable宏使在创建蓝图类时能以此作为父类
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:
	//需标记为Replicated，否则客户端依然会报错
	UPROPERTY(Replicated)
	class USActionComponent* ActionComp;

	//当行为激活时，添加的Tags，在行为停止时将这些Tags移除
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//用于阻止行动的Tags，当没有这些Tags时，行动才能开始
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

	//因为UCLASS(Blueprintable)，重写是为了在蓝图中访问时，GetWorld能正确使用
	class UWorld* GetWorld() const override;


	bool IsSupportedForNetworking() const override
	{
		return true;
	}
}; 