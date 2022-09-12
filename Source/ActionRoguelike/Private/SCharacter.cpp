// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"

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

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	TimeToHitParamName = "TimeToHit";
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

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
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
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ASCharacter::BlackholeAttack()
{
	ActionComp->StartActionByName(this, "BlackholeAttack");
}


void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}


void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	if (NewHealth<=0.0f&&Delta<0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.f);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	//加一点小值是为了修正摄像机被阻挡时的位置
	return CameraComp->GetComponentLocation() +(GetControlRotation().Vector() * 50);
}

