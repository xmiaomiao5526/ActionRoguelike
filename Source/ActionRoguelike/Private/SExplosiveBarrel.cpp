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

	/*源码注释：
	 A component can have multiple physics presences (e.g. destructible mesh components).
	 The component should handle the radial force for all of the physics objects it contains
	 so here we grab all of the unique components to avoid applying impulses more than once.
	 个人理解;
	 若Activate为true，每一帧都会给物体的各个Component施加一个微小的径向力，为啥我目前也不知道。
	 在本项目中，我们只有一个Mesh，所以AutoActivate可以为False*/
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;

	//如果为真，冲量将忽略物体的质量，并始终导致固定的速度变化。(若为假，冲击力会大的离谱)
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
	//别忘了调用父类函数
	Super::PostInitializeComponents();
	//在构造函数中绑定可能会导致Bug，推测可能是热重载的原因。因此在此绑定函数，同时也比在BeginPlay中绑定会好一点点
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explopsive Barrel"));

	//输出字符串的变量名前要加*号
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->GetTimeSeconds());

	FString CombinedString = FString::Printf(TEXT("Hit At location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

