// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	ע����Ӷ�Ӧͷ�ļ����Ƽ�ʹ��Visual Assist���������һ������ͷ�ļ�
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

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
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

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		//ͬPrimaryAttack_TimeElapsed����������������г�ʼ��������λ�á��������ɷ�ʽ��Instigator
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//���������������ײ��⣬�������������ϵ�WorldDynamic��WorldStatic��Pawn
		FCollisionShape shape;
		shape.SetSphere(20.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		//traceEnd��Ĭ�ϳ�ֵ
		FVector traceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 2500);

		FHitResult Hit;

		//�������׷�ټ�⵽�˸���Ȥ�����壬��traceEnd��ֵ�޸�Ϊ�����ֵ
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, traceEnd, FQuat::Identity, ObjParams, shape, QueryParams))
		{
			//traceEnd = Hit.ImpactPoint;
		}
		//�������ף���Ŀ���ǻ�ȡ�ֵ�����ķ���
		FRotator ProjRotation = FRotationMatrix::MakeFromX(traceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}