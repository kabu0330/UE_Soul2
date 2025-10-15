// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoulAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class SOUL_API USoulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USoulAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSecond) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OwningActor")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OwningActor|Component")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData")
	bool bIsFalling;
	
	
};
