// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponStatData.h"

#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"

UWeaponStatData::UWeaponStatData()
{
	StaminaCostMap.Add(SoulGameplayTag::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(SoulGameplayTag::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(SoulGameplayTag::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(SoulGameplayTag::Character_Attack_Heavy, 20.f);

	DamageMultiplierMap.Add(SoulGameplayTag::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(SoulGameplayTag::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(SoulGameplayTag::Character_Attack_Special, 2.1f);
}




