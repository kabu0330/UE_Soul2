// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoulPlayerControllerBase.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SOUL_API ASoulPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnhancedInput|MappingContext")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> SprintAndRollingAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> ToggleCombatModeAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "EnhancedInput|InputAction")
	TObjectPtr<UInputAction> HeavyAttackAction;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	/** 점프 */
	void Jump();
	void StopJumping();

	/** 질주 */
	void Sprint(); 
	void StopSprinting();

	/** 구르기 */
	void Rolling();

	void Interact();
	void ToggleCombatMode();

	/** 공격 */
	void AutoCombatMode();
	void Attack();
	void SpecialAttack();
	void HeavyAttack();

public:
	ASoulPlayerControllerBase();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
};
