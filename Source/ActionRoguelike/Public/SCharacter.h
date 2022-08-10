// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	class UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	
	/*���߱�����UCameraComponent��USpringArmComponent��class�����ɲ���ͷ�ļ���inlucde������ͷ�ļ����Ӷ��ӿ�����ٶ�
	���UPROPERTY()�꽫���������¶���༭��������Ȩ��ΪVisibleAnywhere*/
	//��ӵ��ɼ����
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	//�������ͷ���
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Component")
	class USAttributeComponent* AttributeComp;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();//��Action����Ҫvalue����
	void PrimaryInteract();

	void PrimaryAttack_TimeElapsed();

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;
	FTimerHandle TimerHandle_BlackholeAttack;
	void BlackholeAttack();
	void BlackholeAttack_TimeElapsed();

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;
	FTimerHandle TimerHandle_Dash;
	void Dash();
	void Dash_TimeElapsed();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
};
