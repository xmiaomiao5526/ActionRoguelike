// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	// Reliable - 类似于TCP，可靠传输，例：发送消息，停止行动等游戏性相关逻辑
	//Unreliable - 类似于UDP，不保证可靠传输，例：播放动画、声音等非游戏性逻辑
	//命名规范：在服务器中的rpc函数名前加Server，客户端则加Client
	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InFocus);

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel>  CollisionChannel;

	void FindBestInteractable();

	//即使我们不打算暴露FocusedActor在编辑器中，也应该加上UPROPERTY()，使该对象被销毁时，清空此指针，防止内存泄露
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly,Category = "UI")
	TSubclassOf<class USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	class USWorldUserWidget* DefaultWidgetInstance;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();
		
};
