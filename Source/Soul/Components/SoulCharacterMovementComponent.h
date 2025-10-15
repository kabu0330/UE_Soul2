// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ViewportToolbar/UnrealEdViewportToolbar.h"
#include "SoulCharacterMovementComponent.generated.h"


class USoulAttributeComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API USoulCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USoulCharacterMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	void Sprint();
	void StopSprinting();

	bool IsMoving() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "MovementData")
	float BaseSpeed = 700.f;
	
	UPROPERTY(EditAnywhere, Category = "MovementData")
	float BaseSprintSpeed = 1000.f;

	UPROPERTY()
	bool bSprinting = false;

	UPROPERTY()
	TObjectPtr<USoulAttributeComponent> AttributeComponent;

public:
	FORCEINLINE float GetBaseSpeed() const {return BaseSpeed; }
	FORCEINLINE float GetBaseSprintSpeed() const {return BaseSprintSpeed;}
	FORCEINLINE bool IsSprinting() const {return bSprinting;}
	
	FORCEINLINE void SetBaseSpeed(const float Speed) {BaseSpeed = Speed;}
	FORCEINLINE void SetBaseSprintSpeed(const float SprintSpeed) {BaseSprintSpeed = SprintSpeed;}
	FORCEINLINE void SetSprinting(const bool bSprint) {bSprinting = bSprint;}
	
};
