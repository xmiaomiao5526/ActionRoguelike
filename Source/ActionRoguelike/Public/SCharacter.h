// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category = "Effects")
	FName TimeToHitParamName;
	
	/*告诉编译器UCameraComponent和USpringArmComponent是class，即可不在头文件中inlucde这两个头文件，从而加快编译速度
	添加UPROPERTY()宏将两个组件暴露给编辑器，设置权限为VisibleAnywhere*/
	//添加弹簧架组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//添加摄像头组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	class USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USActionComponent* ActionComp;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();//绑定Action不需要value参数
	void PrimaryInteract();

	void BlackholeAttack();

	void Dash();

	void SprintStart();
	void SprintStop();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	void StartAttackEffects();
	
	//在第三人称时最好重写成获得摄像机的位置，（默认是人物位置）
	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
