// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BPlayerCharacter.generated.h"

struct FInputActionInstance;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class DRAWSYSTEM_API ABPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float _DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

	// ***************** Component *****************

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* CameraComponent;

	// ***************** Input *****************

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* RotateAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* JogAction = nullptr;

	FVector MoveInput = FVector::ZeroVector;
	FVector RotateInput = FVector::ZeroVector;
	FVector MoveDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 30.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	float MaxTurnSpeed = 30.0f;

	uint32 bIsJog : 1 = false;
	uint32 bIsMove : 1 = false;
	uint32 bIsJogTrigger : 1 = false;
	uint32 bIsJumpTrigger : 1 = false;

	void PlayerIdle(const FInputActionInstance& _MoveAction);
	void PlayerMove(const FInputActionInstance& _MoveAction);
	void PlayerRotate(const FInputActionInstance& _RotateAction);
	void PlayerRotate(const FVector2D& _RotateAction);

	void StopRotate();

	void ActiveJog();
	void DisableJog();

	void ActiveJump();
	void DisableJump();

	void IdleEnter();
	void IdleUpdate(float _DeltaTime);
	void IdleExit();

	void MoveEnter();
	void MoveUpdate(float _DeltaTime);
	void MoveExit();
};
