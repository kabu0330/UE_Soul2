// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulEquipment.h"
#include "Soul/SoulDefine.h"
#include "SoulWeapon.generated.h"

class UWeaponStatData;
class UWeaponCollisionComponent;
class UCombatComponent;
class UMontageActionData;

UCLASS()
class SOUL_API ASoulWeapon : public ASoulEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment|SocketName")
	FName EquipSocketName = "SwordEquipWeapon";

	UPROPERTY(EditDefaultsOnly, Category = "Equipment|SocketName")
	FName UnequipSocketName = "SwordUnequipWeapon";

	/** 캐릭터의 무기 장착과 관련된 기능을 처리하는 컴포넌트 */
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWeaponCollisionComponent> WeaponCollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWeaponCollisionComponent> SecondWeaponCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment|Data")
	TObjectPtr<UWeaponStatData> WeaponStatDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment|Animation")
	TObjectPtr<UMontageActionData> MontageActionData;

	/** Combat Type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	ECombatType CombatType = ECombatType::SwordShield;

public:
	ASoulWeapon();
	virtual void Tick(float DeltaTime) override;
	
	virtual void EquipItem() override;
	virtual void UnequipItem() override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
	
	
	float GetStaminaCost(const FGameplayTag& InTag) const;

	UAnimMontage* GetMontageForTag(const FGameplayTag& InTag, const int32 Index = 0) const;

	// 콜리저 관련
	float GetAttackDamage() const;
	FORCEINLINE UWeaponCollisionComponent* GetCollision() const {return WeaponCollisionComponent;}
	void OnHitActor(const FHitResult& Hit);

	FORCEINLINE ECombatType GetCombatType() const {return CombatType;}
	virtual void ActivateCollision(EWeaponCollisionType InCollisionType);
	virtual void DeactivateCollision(EWeaponCollisionType InCollisionType);

protected:
	virtual void BeginPlay() override;
	
};
