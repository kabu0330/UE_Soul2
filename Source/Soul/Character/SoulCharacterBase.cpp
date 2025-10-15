// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulCharacterBase.h"

#include "Soul/Components/SoulAttributeComponent.h"
#include "Soul/Components/SoulCharacterMovementComponent.h"

ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<USoulAttributeComponent>("AttributeComponent");
	
}

void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

