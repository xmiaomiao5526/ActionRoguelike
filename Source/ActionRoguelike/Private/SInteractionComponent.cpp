// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "SWorldUserWidget.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Ŀǰÿ֡���У������ϴ�֮���Ż�ʱ����һ����ʱ��
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	UCameraComponent* Camera = Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	//������ײɨ�䣬����������ײ������Ʒ
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LinearColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	//ѭ��������������ײ��������
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LinearColor, false, 2.0f);
	}
	if (FocusedActor)
	{
		//����(����)ģʽ
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		 if (DefaultWidgetInstance)
		 {
			 DefaultWidgetInstance->AttachedActor = FocusedActor;

			 if (!DefaultWidgetInstance->IsInViewport())
			 {
				 DefaultWidgetInstance->AddToViewport();
			 }
		 }
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	//DrawDebugLine(GetWorld(), EyeLocation, End, LinearColor, false, 2.0f, 0, 2.0f);
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

//������Ҫ����ָ�뼴�ɣ������⿴��ȥ��̫���ܣ���Ϊ�������Ϳͻ����е��ڴ�洢��ʽ�϶��ǲ�һ����
//��������Զ������ǰ�ָ��ָ���Object������ID�Ķ������͹�ȥ�����ɽ��������
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}