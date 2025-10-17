// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class UCombatComponent;
class UStateComponent;
class UAttributeComponent;



/** 캐릭터와 몬스터가 공통으로 가지고 있는 컴포넌트
 * 
 */
UCLASS()
class SOUL_API ASoulCharacterBase : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStateComponent> StateComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCombatComponent> CombatComponent;

public:
	ASoulCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UAttributeComponent* GetAttributeComponent() const {return AttributeComponent; }
	FORCEINLINE UStateComponent* GetStateComponent() const {return StateComponent; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const {return CombatComponent; }

	/** 현재 상태에서 수행 가능한 일반 공격 */
	FGameplayTag GetAttackPerform() const;

	/** 공격 가능 조건 체크 */
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;

	bool GetCurrentState(const FGameplayTag& InTag) const;

	/** 공격 실행 */
	void DoAttack(const FGameplayTag& AttackTypeTag);
	void AttackFinished();

	virtual void OnDeath();
	
protected:
	virtual void BeginPlay() override;
};
