// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	注意添加对应头文件，推荐使用Visual Assist插件，可一键插入头文件
	使用CreateDefaultSubobject创建组件，<>中为创建类型，对应指针类型，（）中为组件的名字
	并使用SetupAttachment将其附加到别的组件下
	将Camerta附加到弹簧架下，可防止摄像头被物体阻挡：当摄像头被墙阻挡时，弹簧臂会回缩
	*/
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//参数：按键名，对应编辑器中绑定按键时的名字；对象，一般是this；调用的函数，BindAxis时该函数需要传入参数float value
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

void ASCharacter::MoveForward(float value)
{
	//封装好的移动函数，在指定方向移动
	AddMovementInput(GetActorForwardVector(),value);
}

