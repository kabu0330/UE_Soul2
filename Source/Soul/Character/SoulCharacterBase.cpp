// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulCharacterBase.h"

#include "Soul/Components/AttributeComponent.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/SoulCharacterMovementComponent.h"
#include "Soul/Components/StateComponent.h"

ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>("AttributeComponent");
	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");
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

