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

	// Reliable - ������TCP���ɿ����䣬����������Ϣ��ֹͣ�ж�����Ϸ������߼�
	//Unreliable - ������UDP������֤�ɿ����䣬�������Ŷ����������ȷ���Ϸ���߼�
	//�����淶���ڷ������е�rpc������ǰ��Server���ͻ������Client
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

	//��ʹ���ǲ����㱩¶FocusedActor�ڱ༭���У�ҲӦ�ü���UPROPERTY()��ʹ�ö�������ʱ����մ�ָ�룬��ֹ�ڴ�й¶
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
