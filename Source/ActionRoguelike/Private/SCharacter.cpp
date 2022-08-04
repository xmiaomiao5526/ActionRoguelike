// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	ע����Ӷ�Ӧͷ�ļ����Ƽ�ʹ��Visual Assist�������һ������ͷ�ļ�
	ʹ��CreateDefaultSubobject���������<>��Ϊ�������ͣ���Ӧָ�����ͣ�������Ϊ���������
	��ʹ��SetupAttachment���丽�ӵ���������
	��Camerta���ӵ����ɼ��£��ɷ�ֹ����ͷ�������赲��������ͷ��ǽ�赲ʱ�����ɱۻ����
	*/
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->bUsePawnControlRotation = true; //�õ��ɱ��ϵ�Camerta����PlayerControl���ӽ�һ��
	GetCharacterMovement()->bOrientRotationToMovement = true;//���ﳯ���ת���˶����򣬱���ɨ�����,include CharacterMovementComponent
	bUseControllerRotationYaw = false;//ȡ�����﷽�����������������������ƶ�
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
	//����������������Ӧ�༭���а󶨰���ʱ�����֣�����һ����this�����õĺ�����BindAxisʱ�ú�����Ҫ�������float value
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
}

void ASCharacter::MoveForward(float value)
{
	//AddMovementInput(GetActorForwardVector(), value);��װ�õ��ƶ���������ָ�������ƶ�
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	AddMovementInput(ControlRot.Vector(),value);
}

void ASCharacter::MoveRight(float value)
{
	/*AddMovementInput(GetActorRightVector(), value);
	��仰�ᵼ�°�D��ɫһֱתȦ����Ϊ����bOrientRotationToMovement��ActorRightvector��һֱ�ı�
	��ʱӦ��Ϊ���Controller��Rotation����ȷ��������*/
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void ASCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	//��������������ʱ����ײ��⣺�������ɣ��������Ƿ���ײ������
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//��Wrold�µ�������Actor�ĺ�����
	//��һ������Ϊ�������ڶ���ΪTransform����,��������FActorSpawnParameters�࣬�����кܶ�����Actor����Ҫ�Ĳ���
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}
