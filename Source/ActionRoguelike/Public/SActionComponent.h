// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

	//直接在此头文件中实例化的对象，而非创建一个指针，因此需在此头文件中include
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable,Category = "Action")
	void AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category = "Action")
	bool StartActionByName(AActor* Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;
protected:
	UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	//在游戏一开始主角就会自带的能力，可以在蓝图中添加
	UPROPERTY(EditAnywhere,Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

	//用一数组维护Action
	UPROPERTY(Replicated)
	TArray<class USAction*> Actions;

	virtual void BeginPlay() override;

public:	

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
