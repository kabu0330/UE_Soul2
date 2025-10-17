// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "Soul/Interface/SoulTargeting.h"
#include "SoulEnemy.generated.h"

class UWidgetComponent;
class USphereComponent;

UCLASS()
class SOUL_API ASoulEnemy : public ASoulCharacterBase, public ISoulTargeting
{
	GENERATED_BODY()

protected:
	// Effect Section
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

	// Montage Section
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimFront;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimBack;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimLeft;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimRight;

	/** 타게팅 충돌 체크용 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> TargetingSphereComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

public:
	ASoulEnemy(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// ISoul_Targeting
	/** 타게팅 처리 함수 */
	virtual void OnTargeted(bool bTarget) override;
	
	/** 타게팅이 가능한 상태인지 체크하는 함수 */
	virtual bool CanBeTargeted() override;


protected:
	virtual void BeginPlay() override;
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;

};
