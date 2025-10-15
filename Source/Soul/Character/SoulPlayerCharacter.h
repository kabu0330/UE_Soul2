// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "SoulPlayerCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SOUL_API ASoulPlayerCharacter : public ASoulCharacterBase
{
	GENERATED_BODY()
	
public:
	ASoulPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE float GetTargetArmLength() const { return TargetArmLength; }
	
protected:
	virtual void BeginPlay() override;

protected: // Components
	/** 카메라 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

protected: 
	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 400.0f;
	
};
