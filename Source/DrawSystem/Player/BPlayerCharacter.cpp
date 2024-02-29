// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerCharacter.h"
#include "DrawSystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ABPlayerCharacter::ABPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 매쉬 불러오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, -90));
	}

	// 스프링암 컴포넌트 생성 및 시작 위치 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0, 70, 90));
	SpringArmComponent->TargetArmLength = 400;
	SpringArmComponent->bUsePawnControlRotation = true;

	// 카메라 컴포넌트 생성 및 부착
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ABPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABPlayerCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

}

// Called to bind functionality to input
void ABPlayerCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController == nullptr)
	{
		LOG_FATAL(TEXT("Failed Find APlayerController"));
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (Subsystem == nullptr)
	{
		LOG_FATAL(TEXT("Failed Get GetSubsystem<UEnhancedInputLocalPlayerSubsystem>"));
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	if (nullptr == Input)
	{
		LOG_FATAL(TEXT("Failed Find InputComponent"));
	}
	if (nullptr == MoveAction)
	{
		LOG_FATAL(TEXT("is Not Set MoveAction"));
	}
	if (nullptr == RotateAction)
	{
		LOG_FATAL(TEXT("is Not Set LookAction"));
	}
	if (nullptr == JumpAction)
	{
		LOG_FATAL(TEXT("is Not Set JogAction"));
	}
	if (nullptr == JogAction)
	{
		LOG_FATAL(TEXT("is Not Set JogAction"));
	}

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::PlayerMove);
	Input->BindAction(MoveAction, ETriggerEvent::None, this, &ABPlayerCharacter::PlayerIdle);
	Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::PlayerRotate);
	Input->BindAction(RotateAction, ETriggerEvent::None, this, &ABPlayerCharacter::StopRotate);

	Input->BindAction(JogAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::ActiveJog);
	Input->BindAction(JogAction, ETriggerEvent::Completed, this, &ABPlayerCharacter::DisableJog);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::ActiveJump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABPlayerCharacter::DisableJump);
}

void ABPlayerCharacter::PlayerIdle(const FInputActionInstance& _MoveAction)
{
	MoveInput = FVector::ZeroVector;
}

void ABPlayerCharacter::PlayerMove(const FInputActionInstance& _MoveAction)
{
	FVector2D InputVector = _MoveAction.GetValue().Get<FVector2D>();
	//MoveInput = FVector(InputVector.X, InputVector.Y, 0);

	if (IsValid(Controller))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwordDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwordDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void ABPlayerCharacter::PlayerRotate(const FInputActionInstance& _RotateAction)
{
	PlayerRotate(_RotateAction.GetValue().Get<FVector2D>());
}

void ABPlayerCharacter::PlayerRotate(const FVector2D& _RotateAction)
{
	if (IsValid(Controller))
	{
		RotateInput.X = _RotateAction.X;
		RotateInput.Y = _RotateAction.Y;
		
		RotateInput.X = FMath::Min(RotateInput.X, MaxTurnSpeed);
		RotateInput.Y = FMath::Min(RotateInput.Y, MaxTurnSpeed);
		
		float TurnX = RotateInput.X * TurnSpeed * GetWorld()->GetDeltaSeconds();
		float TurnY = RotateInput.Y * TurnSpeed * GetWorld()->GetDeltaSeconds();

		AddControllerYawInput(-TurnX);
		AddControllerPitchInput(TurnY);
	}
}

void ABPlayerCharacter::StopRotate()
{
	RotateInput = FVector::ZeroVector;
}

void ABPlayerCharacter::ActiveJump()
{
	bIsJumpTrigger = true;
	ACharacter::Jump();
}

void ABPlayerCharacter::DisableJump()
{
	bIsJumpTrigger = false;
}

void ABPlayerCharacter::ActiveJog()
{
	bIsJogTrigger = true;
}

void ABPlayerCharacter::DisableJog()
{
	bIsJogTrigger = false;
}

void ABPlayerCharacter::IdleEnter() {}

void ABPlayerCharacter::IdleUpdate(float _DeltaTime)
{
	bIsMove = false;
	bIsJog = false;
}

void ABPlayerCharacter::IdleExit() {}

void ABPlayerCharacter::MoveEnter() {}

void ABPlayerCharacter::MoveUpdate(float _DeltaTime)
{
	bIsJog = false;
	bIsMove = true;
}

void ABPlayerCharacter::MoveExit() {}