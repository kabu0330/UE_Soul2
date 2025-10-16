#pragma once

#include "CoreMinimal.h"

// 콜리전 오브젝트
#define COLLISION_OBJECT_INTERACT ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Stamina,
	Health,
};