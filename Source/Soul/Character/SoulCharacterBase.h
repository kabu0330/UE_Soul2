// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class UCombatComponent;
class UStateComponent;
class UAttributeComponent;



/** 캐릭터와 몬스터가 공통으로 가지고 있는 컴포넌트
 * 
 */
UCLASS()
class SOUL_API ASoulCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASoulCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UAttributeComponent* GetAttributeComponent() const {return AttributeComponent; }
	FORCEINLINE UStateComponent* GetStateComponent() const {return StateComponent; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const {return CombatComponent; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStateComponent> StateComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCombatComponent> CombatComponent; 

public:

};
