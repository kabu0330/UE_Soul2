// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Soul/SoulDefine.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EAttributeType, Type, float, Ratio);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnAttributeChanged OnAttributeChanged;
	FOnDeath OnDeath;

protected:
	// 스태미나 Section
	/** 스태미나 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attribute|Stamina")
	float BaseStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attribute|Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attribute|Stamina")
	float StaminaRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attribute|Stamina")
	float StaminaRegenRate = 0.1f;

	FTimerHandle StaminaRegenTimer;

	// 체력
	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

public:
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE float GetBaseStamina() const {return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const {return MaxStamina;}
	FORCEINLINE float GetStaminaRate() const {return StaminaRate;}

	/** 스태미나 소비/회복 액션 실행 여부 체크 */ 
	bool CheckHasEnoughStamina(const float Stamina) const;

	/** 스태미나 소비 */
	void DecreaseStamina(const float StaminaCost);

	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 1.f);
	void BroadCastAttributeChanged(EAttributeType Type) const;

	// 체력
	FORCEINLINE float GetBaseHealth() const {return BaseHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	void TakeDamageAmount(float DamageAmount);

protected:
	virtual void BeginPlay() override;
	
private:
	void RegenStaminaHandler();
	

};
