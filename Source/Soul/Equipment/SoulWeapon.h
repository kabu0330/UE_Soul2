// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulEquipment.h"
#include "SoulWeapon.generated.h"

class UWeaponStatData;
class UWeaponCollisionComponent;
class UCombatComponent;
class UMontageActionData;

UCLASS()
class SOUL_API ASoulWeapon : public ASoulEquipment
{
	GENERATED_BODY()

public:
	ASoulWeapon();
	virtual void Tick(float DeltaTime) override;
	
	virtual void EquipItem() override;
	virtual void UnequipItem() override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
	
	FORCEINLINE UWeaponCollisionComponent* GetCollision() const {return WeaponCollisionComponent;}
	
	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GetAttackDamage() const;

	UAnimMontage* GetMontageForTag(const FGameplayTag& InTag, const int32 Index = 0) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SocketName")
	FName EquipSocketName = "SwordEquipWeapon";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SocketName")
	FName UnequipSocketName = "SwordUnequipWeapon";

	/** 캐릭터의 무기 장착과 관련된 기능을 처리하는 컴포넌트 */
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWeaponCollisionComponent> WeaponCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Data")
	TObjectPtr<UWeaponStatData> WeaponStatDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Animation")
	TObjectPtr<UMontageActionData> MontageActionData;

	/** Combat Type */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	// ECombatType CombatType = ECombatType::SwordShield;
	
	
	
};
