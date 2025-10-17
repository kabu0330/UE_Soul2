// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer ActiveGameplayTags;
	
public:
	UStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	/** 태그 추가 */
	void AddGameplayTag(const FGameplayTag& GameplayTag);
	void AddGameplayTags(const FGameplayTagContainer& GameplayTags);

	/** 태그 삭제 */
	void RemoveGameplayTag(const FGameplayTag& GameplayTag);
	void RemoveGameplayTags(const FGameplayTagContainer& GameplayTags);

	/** 태그 검색 */
	bool IsActiveGameplayTag(const FGameplayTag& GameplayTag) const;
	bool IsAnyActiveGameplayTags(const FGameplayTagContainer& GameplayTags) const;
	bool IsAllActiveGameplayTags(const FGameplayTagContainer& GameplayTags) const;

	/** 태그 초기화 */
	void ClearAllGameplayTags();

	/** 이동관련 입력을 처리할 별도의 함수 */
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);
	bool IsMovementInputEnabled() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MovementInputEnableAction();

};
