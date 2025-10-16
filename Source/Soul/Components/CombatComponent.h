// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class ASoulWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeapon(ASoulWeapon* NewWeapon);
	
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; }
	FORCEINLINE ASoulWeapon* GetMainWeapon() const { return MainWeapon; }

	FORCEINLINE FGameplayTag GetLastAttackType() const {return LastAttackType;}
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) {LastAttackType = NewAttackTypeTag;}

protected:
	virtual void BeginPlay() override;

	/** 전투가 가능한 상태(무기를 꺼낸)인지?
	 *  애님 몽타주의 노티파이에서 상태를 변경시키고 있다.
	*/
	UPROPERTY()
	bool bCombatEnabled = false;

	UPROPERTY()
	TObjectPtr<ASoulWeapon> MainWeapon;

	/** 마지막 공격 타입 : 현재 진행 중인 공격 */
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

};
