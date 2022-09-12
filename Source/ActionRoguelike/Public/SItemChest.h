// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	UPROPERTY(EditAnywhere)
	float Targetpitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//ReplicatedUsing：当值改变时，将调用该函数，但仅在客户端调用
	UPROPERTY(ReplicatedUsing="OnRep_LidOpend", BlueprintReadOnly)
	bool bLidOpened;

	//命名规范： “OnRep_” + 函数名
	UFUNCTION()
	void OnRep_LidOpend();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UStaticMeshComponent* LidMesh;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//因为前面在UFUNCTION中指定了BlueprintNativeEvent，所有函数名后要加_Implementation
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
