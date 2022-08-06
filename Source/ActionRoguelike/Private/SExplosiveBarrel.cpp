// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForeceComp");

	/*Դ��ע�ͣ�
	 A component can have multiple physics presences (e.g. destructible mesh components).
	 The component should handle the radial force for all of the physics objects it contains
	 so here we grab all of the unique components to avoid applying impulses more than once.
	 �������;
	 ��ActivateΪtrue��ÿһ֡���������ĸ���Componentʩ��һ��΢С�ľ�������Ϊɶ��ĿǰҲ��֪����
	 �ڱ���Ŀ�У�����ֻ��һ��Mesh������AutoActivate����ΪFalse*/
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;

	//���Ϊ�棬�����������������������ʼ�յ��¹̶����ٶȱ仯��(��Ϊ�٣��������������)
	ForceComp->bImpulseVelChange = true;

	//optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	//�����˵��ø��ຯ��
	Super::PostInitializeComponents();
	//�ڹ��캯���а󶨿��ܻᵼ��Bug���Ʋ�����������ص�ԭ������ڴ˰󶨺�����ͬʱҲ����BeginPlay�а󶨻��һ���
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explopsive Barrel"));

	//����ַ����ı�����ǰҪ��*��
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->GetTimeSeconds());

	FString CombinedString = FString::Printf(TEXT("Hit At location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

