// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoulTargeting.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USoulTargeting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOUL_API ISoulTargeting
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** 타게팅 처리 함수 */
	virtual void OnTargeted(bool bTarget) = 0;
	
	/** 타게팅이 가능한 상태인지 체크하는 함수 */
	virtual bool CanBeTargeted() = 0;
};
