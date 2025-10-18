// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soul/SoulDefine.h"
#include "UObject/Interface.h"
#include "SoulCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USoulCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOUL_API ISoulCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivateCollision(EWeaponCollisionType InCollisionType) = 0;
	virtual void DeactivateCollision(EWeaponCollisionType InCollisionType) = 0;
};
