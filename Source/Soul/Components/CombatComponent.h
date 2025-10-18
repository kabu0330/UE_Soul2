// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedCombat, bool);

class ASoulWeapon;

/** 주요 프로퍼티 : 캐릭터가 소유한 무기, 전투 모드, 콤보 관련 데이터 
 *  공격과 관련된 주요 기능들은 컴뱃 컴포넌트와 Weapon에서 처리하고
 *  상태나 속성과 같은 데이터 조회가 필요한 경우 ASoulCharacterBase에서 처리한다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 무기 장착 모션 상태 변화를 알리는 델리게이트 */
	FOnChangedCombat OnChangedCombat;
	
protected:
	UPROPERTY()
	TObjectPtr<ASoulWeapon> MainWeapon;

	/** 전투가 가능한 상태(무기를 꺼낸)인지? 애님 몽타주의 노티파이에서 상태를 변경시키고 있다. */
	UPROPERTY()
	bool bCombatEnabled = false;

	// 콤보 공격
	/** 마지막 공격 타입 : 현재 진행 중인 공격 */
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

	/** 콤보 시퀀스 진행 중 */
	bool bComboSequenceRunning = false;

	/** 콤보 입력 가능 상태 체크 */
	bool bCanComboInput = false;

	/** 콤보 입력 여부 */
	bool bSavedComboInput = false;

	/** 콤보 카운팅 */
	int32 ComboCounter = 0;

	/** 콤보 리셋 타이머 핸들 */
	FTimerHandle ComboResetTimerHandle;

	/** 공격 실행 */
	void DoAttack(const FGameplayTag& AttackTypeTag);

	/** 콤보 실행 */
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);

	/** 콤보 초기화 */
	void ResetCombo();
	
public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	/** 픽업 아이템으로 무기 습득 시 호출 */
	void SetWeapon(ASoulWeapon* NewWeapon);

	FORCEINLINE ASoulWeapon* GetMainWeapon() const { return MainWeapon; }

	// 전투 모드 
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	void SetCombatEnabled(const bool bEnabled);
	

	// Combo Section 
	FORCEINLINE FGameplayTag GetLastAttackType() const {return LastAttackType;}
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) {LastAttackType = NewAttackTypeTag;}
	FORCEINLINE int32 GetComboCounter() const { return ComboCounter; }
	FORCEINLINE void SetComboCounter(const int32 NewComboCounter) { ComboCounter = NewComboCounter; }

	// Combo Section : Input
	void Attack();
	void SpecialAttack();
	void HeavyAttack();

	// Combo Section: AnimNotify
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);

protected:
	virtual void BeginPlay() override;
	
};
