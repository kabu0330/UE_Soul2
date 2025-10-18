// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Components/AttributeComponent.h"
#include "Soul/Components/StateComponent.h"
#include "Sound/SoundCue.h"


ASoulEnemy::ASoulEnemy(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 타게팅
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>("TargetingSphereComponent");
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 비지빌리티 트레이스 채널만 켜고 나머지는 다 무시

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("LockOnWidgetComponent");
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	LockOnWidgetComponent->SetDrawSize(FVector2D(100.f, 100.f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 2D
	LockOnWidgetComponent->SetVisibility(false); // 안 보이게
}

void ASoulEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ASoulEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	LOG_WARNING(">>> TakeDamage <<< ");
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	check(AttributeComponent);

	// 데미지 체력에 적용 및 사망 체크
	AttributeComponent->TakeDamageAmount(Damage);
	const float BaseHealth = AttributeComponent->GetBaseHealth();

	DEBUG_MESSAGE("Damage: %f, BaseHealth : %f", Damage, BaseHealth);
	
	// UGameplayStatics::ApplyPointDamage 함수를 호출하면 FPointDamageEvent::ClassID를 통해 PointDamage를 입었다는 사실을 체크할 수 있다. 
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// 데미지 방향
		const FVector ShotDirection = PointDamageEvent->ShotDirection;

		// 히트 위치 (에너미 충돌 위치)
		const FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;

		// 히트 방향
		const FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;

		// 히트한 객체의 위치 (부딪힌 무기의 위치)
		const FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);
		HitReaction(EventInstigator->GetPawn());
	}
	
	return ActualDamage;
}

void ASoulEnemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Location);
	}
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, Location);
	}
}

void ASoulEnemy::HitReaction(const AActor* Attacker)
{
	if (UAnimMontage* HitReactAnimation = GetHitReactAnimation(Attacker))
	{
		const float DelaySecond = PlayAnimMontage(HitReactAnimation);
	}
}

UAnimMontage* ASoulEnemy::GetHitReactAnimation(const AActor* Attacker) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = HitReactAnimFront;
		break;
	case EHitDirection::Left:
		SelectedMontage = HitReactAnimLeft;
		break;
	case EHitDirection::Back:
		SelectedMontage = HitReactAnimBack;
		break;
	case EHitDirection::Right:
		SelectedMontage = HitReactAnimRight;
		break;
	default:
		break;
	}
	return SelectedMontage;
}

void ASoulEnemy::OnTargeted(bool bTarget)
{
	check(LockOnWidgetComponent);
	LockOnWidgetComponent->SetVisibility(bTarget);
}

bool ASoulEnemy::CanBeTargeted()
{
	check(StateComponent);
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SoulGameplayTag::Character_State_Death);
	return false == StateComponent->IsAnyActiveGameplayTags(CheckTags);
}

