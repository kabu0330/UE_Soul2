// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulEquipment.h"
#include "SoulWeapon.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Animation")
	TObjectPtr<UMontageActionData> MontageActionData;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SocketName")
	FName EquipSocketName = "SwordEquipWeapon";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SocketName")
	FName UnequipSocketName = "SwordUnequipWeapon";

	
	// UPROPERTY(EditDefaultsOnly, Category = "SoulWeapon|StaminaCost")
	// TMap<FGameplayTag, float> StaminaCostMap;
	
	
};
