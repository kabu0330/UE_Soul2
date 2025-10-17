// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "SoulPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateMessage, const FString&, Message);

class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SOUL_API ASoulPlayerCharacter : public ASoulCharacterBase
{
	GENERATED_BODY()
	
public:
	FOnStateMessage OnStateMessage;
	FTimerHandle StateMessageTimer;

protected: // Components
	/** 카메라 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

protected: // Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> RollingMontage;
	
protected: 
	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Attribute|Data")
	float RollingCost = 15.f;

public:
	ASoulPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE float GetTargetArmLength() const { return TargetArmLength; }


	/** 캐릭터가 특정 동작을 수행할 수 없을 때 UI로 출력시켜줄 메시지를 전송할 함수 */
	void BroadcastStateMessage(const FString& Message);

	// 상태 체크 함수
	bool CanToggleCombatMode() const;

	// 동작
	/** 구르기 */
	void Rolling();
	
	/** 아이템 줍기 */
	void Interact();

	/** 전투 모드 변환 : 무기 손에 쥐기 <-> 무기 등에 부착 */
	void ToggleCombatMode();
	void AutoCombatMode();

	// Combo Section : Input
	void Attack();
	void SpecialAttack();
	void HeavyAttack();

protected:
	virtual void BeginPlay() override;
};
