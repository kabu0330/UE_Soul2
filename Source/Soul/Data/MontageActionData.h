// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MontageActionData.generated.h"

USTRUCT(BlueprintType)
struct FMontageGroup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> Animations;
};
/**
 * 
 */
UCLASS()
class SOUL_API UMontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index = 0);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Montage Groups"))
	TMap<FGameplayTag, FMontageGroup> MontageGroups; 
};
