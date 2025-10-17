#pragma once

#include "CoreMinimal.h"

// 콜리전 오브젝트
#define COLLISION_OBJECT_INTERACT ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Stamina,
	Health,
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Left,
	Right,
	Back
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None,
	Left,
	Right
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None,
	SwordShield,
	TwoHanded,
	MeleeFists
};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	MainCollision,
	SecondCollision
};