// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "TimerManager.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	注意添加对应头文件，推荐使用Visual Assist，插件，可一键插入头文件
	使用CreateDefaultSubobject创建组件，<>中为创建类型，对应指针类型，（）中为组件的名字
	并使用SetupAttachment将其附加到别的组件下
	将Camerta附加到弹簧架下，可防止摄像头被物体阻挡：当摄像头被墙阻挡时，弹簧臂会回缩
	*/
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->bUsePawnControlRotation = true; //让弹簧臂上的Camerta跟着PlayerControl的视角一起动
	GetCharacterMovement()->bOrientRotationToMovement = true;//人物朝向会转向运动方向，避免扫射情况,include CharacterMovementComponent
	bUseControllerRotationYaw = false;//取消人物方向随控制器（摄像机）方向移动

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
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
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float value)
{
	//AddMovementInput(GetActorForwardVector(), value);封装好的移动函数，在指定方向移动
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	AddMovementInput(ControlRot.Vector(),value);
}

void ASCharacter::MoveRight(float value)
{
	/*AddMovementInput(GetActorRightVector(), value);
	这句话会导致按D角色一直转圈，因为开启bOrientRotationToMovement后，ActorRightvector会一直改变
	此时应改为获得Controller的Rotation，正确代码如下*/
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed,0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	/*用ensure代替check,UE自带的一个较温和的主动检查方式，且在打包好的游戏中不会触发
	ensureAlways每次检查错误都会报错并暂停，
	而ensure只会在第一次时报错*/
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

		FActorSpawnParameters SpawnParams;
		//调整生成物生成时的碰撞检测：总是生成，及不管是否碰撞到人物
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//指定Instigator为当前角色
		SpawnParams.Instigator = this;

		//在Wrold下调用生成Actor的函数，
		//第一个参数为类名，第二个为Transform矩阵,第三个是FActorSpawnParameters类，其中有很多生成Actor是需要的参数
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}