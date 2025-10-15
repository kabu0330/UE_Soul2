// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPlayerControllerBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/SoulCharacterMovementComponent.h"

ASoulPlayerControllerBase::ASoulPlayerControllerBase()
{
}

void ASoulPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASoulPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComp->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::Zoom);
		
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

		EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Sprint);
		EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprinting);
	}
}

void ASoulPlayerControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -45.f; // 위
		PlayerCameraManager->ViewPitchMax = 5.f; // 아래
	}
}

void ASoulPlayerControllerBase::Move(const FInputActionValue& Value)
{
	if (GetCharacter() == nullptr) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	GetCharacter()->AddMovementInput(ForwardVector, MovementVector.X);
	GetCharacter()->AddMovementInput(RightVector, MovementVector.Y);
}

void ASoulPlayerControllerBase::Look(const FInputActionValue& Value)
{
	if (GetCharacter() == nullptr) return;

	const FVector2D RotationVector = Value.Get<FVector2D>();

	GetCharacter()->AddControllerYawInput(RotationVector.X);
	
	GetCharacter()->AddControllerPitchInput(RotationVector.Y);
}

void ASoulPlayerControllerBase::Jump()
{
	if (GetCharacter() == nullptr) return;
	GetCharacter()->Jump();
}

void ASoulPlayerControllerBase::StopJumping()
{
	if (GetCharacter() == nullptr) return;
	GetCharacter()->StopJumping();
}

void ASoulPlayerControllerBase::Sprint()
{
	if (GetCharacter() == nullptr) return;
	if (USoulCharacterMovementComponent* MovementComponent = Cast<USoulCharacterMovementComponent>(GetCharacter()->GetCharacterMovement()))
	{
		MovementComponent->Sprint();
	}
}

void ASoulPlayerControllerBase::StopSprinting()
{
	if (GetCharacter() == nullptr) return;
	if (USoulCharacterMovementComponent* MovementComponent = Cast<USoulCharacterMovementComponent>(GetCharacter()->GetCharacterMovement()))
	{
		MovementComponent->StopSprinting();
	}
}

void ASoulPlayerControllerBase::Zoom(const FInputActionValue& Value)
{
	if (GetCharacter() == nullptr) return;
	if (ASoulPlayerCharacter* SoulCharacter = Cast<ASoulPlayerCharacter>(GetCharacter()))
	{
		if (USpringArmComponent* CameraBoom = SoulCharacter->GetCameraBoom(); IsValid(CameraBoom))
		{
			const float ZoomValue = Value.Get<float>() * 10.f;
			const float Length = SoulCharacter->GetTargetArmLength();
			CameraBoom->TargetArmLength = FMath::Clamp(
				CameraBoom->TargetArmLength + ZoomValue, Length - 300.f, Length + 100.f);
		}
	}
		
}
