// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoulInteract.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USoulInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOUL_API ISoulInteract
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* InteractActor) = 0;
};
