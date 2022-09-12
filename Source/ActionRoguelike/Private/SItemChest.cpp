// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	Targetpitch = 110;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;

	//�����ڴ˴�����OnRep_LidOpend�������ڷ������˴�����
	OnRep_LidOpend();
}

void ASItemChest::OnRep_LidOpend()
{
	float CurrPitch = bLidOpened ? Targetpitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

//�ɲ��������˺�����ֱ��д����
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//ÿ��bLipOpened�ı�ʱ�����͸����пͻ��ˣ���include "Net/UnrealNetwork.h"
	DOREPLIFETIME(ASItemChest, bLidOpened);
}


