// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "GameFramework/GameModeBase.h"

#include "SMagicProjectile.h"
#include "SAttributeComponent.h"

#include "SCharacter.generated.h"



class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE1_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	

protected:
	//ArmComponent
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	//CameraComponent
	UPROPERTY(VisibleAnywhere);
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	
	//MoveFoward(Backward)&Right(Left)
	void MoveForward(float value);
	void MoveRight(float value);

	void Attack();
	void Attack_TimeElapsed();
	
// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_Attack;
	
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	// 投射体子类
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	FTimerHandle TimerHandle_FireCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;
	bool bIsSprinting;
	float OriginalWalkSpeed;
	void StartSprinting();
	void StopSprinting();

	
	
};
