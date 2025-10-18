// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulWeapon.h"
#include "SoulFistWeapon.generated.h"

UCLASS()
class SOUL_API ASoulFistWeapon : public ASoulWeapon
{
	GENERATED_BODY()

public:
	ASoulFistWeapon();
	virtual void EquipItem() override;

};
