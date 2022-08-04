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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
	/*告诉编译器UCameraComponent和USpringArmComponent是class，即可不在头文件中inlucde这两个头文件，从而加快编译速度
	添加UPROPERTY()宏将两个组件暴露给编辑器，设置权限为VisibleAnywhere*/
	//添加弹簧架组件
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	//添加摄像头组件
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();//绑定Action不需要value参数

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
