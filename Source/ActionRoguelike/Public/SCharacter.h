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

	UPROPERTY(VisibleAnywhere,Category = "Effects")
	FName TimeToHitParamName;
	
	/*���߱�����UCameraComponent��USpringArmComponent��class�����ɲ���ͷ�ļ���inlucde������ͷ�ļ����Ӷ��ӿ�����ٶ�
	���UPROPERTY()�꽫���������¶���༭��������Ȩ��ΪVisibleAnywhere*/
	//��ӵ��ɼ����
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//�������ͷ���
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	class USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USActionComponent* ActionComp;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();//��Action����Ҫvalue����
	void PrimaryInteract();

	void BlackholeAttack();

	void Dash();

	void SprintStart();
	void SprintStop();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorAcotr, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	void StartAttackEffects();
	
	//�ڵ����˳�ʱ�����д�ɻ���������λ�ã���Ĭ��������λ�ã�
	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
