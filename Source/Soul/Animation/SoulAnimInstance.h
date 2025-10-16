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

	// 앞에 AnimNotify_ 를 명시하면 뒤의 노티파이 이름과 동일한 일반 노티파이 생성하면 해당 시점에 함수 콜백
	UFUNCTION()
	void AnimNotify_ResetMovementInput();


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
