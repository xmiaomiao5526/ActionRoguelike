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
	ע����Ӷ�Ӧͷ�ļ����Ƽ�ʹ��Visual Assist�������һ������ͷ�ļ�
	ʹ��CreateDefaultSubobject���������<>��Ϊ�������ͣ���Ӧָ�����ͣ�������Ϊ���������
	��ʹ��SetupAttachment���丽�ӵ���������
	��Camerta���ӵ����ɼ��£��ɷ�ֹ����ͷ�������赲��������ͷ��ǽ�赲ʱ�����ɱۻ����
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
	//����������������Ӧ�༭���а󶨰���ʱ�����֣�����һ����this�����õĺ�����BindAxisʱ�ú�����Ҫ�������float value
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

void ASCharacter::MoveForward(float value)
{
	//��װ�õ��ƶ���������ָ�������ƶ�
	AddMovementInput(GetActorForwardVector(),value);
}

