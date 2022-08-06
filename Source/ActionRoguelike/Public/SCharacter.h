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

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();//��Action����Ҫvalue����
	void PrimaryInteract();

	void PrimaryAttack_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
