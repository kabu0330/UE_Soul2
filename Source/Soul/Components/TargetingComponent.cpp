// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Interface/SoulTargeting.h"


UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ASoulPlayerCharacter>(GetOwner());
	Camera = Character->GetCameraComponent();
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false == bIsLockOn) return;
	if (false == IsValid(Character)) return;
	if (false == IsValid(LockedTargetActor)) return;

	const float Distance = FVector::Distance(Character->GetActorLocation(), LockedTargetActor->GetActorLocation());

	if (ISoulTargeting* Targeting = Cast<ISoulTargeting>(LockedTargetActor))
	{
		// 타겟이 없거나 타겟과의 거리가 타게팅 범위를 벗어나면
		if (false == Targeting->CanBeTargeted() || Distance > TargetingRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}
}

void UTargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	if (ISoulTargeting* Targeting = Cast<ISoulTargeting>(LockedTargetActor))
	{
		// 락온 대상의 위젯을 꺼준다.
		Targeting->OnTargeted(bIsLockOn);
	}
	
	LockedTargetActor = nullptr;
	OrientMovement();
}

// 카메라가 바라보는 방향으로 고정
void UTargetingComponent::FaceLockOnActor() const
{
	const FRotator CurrentControlRotation = Character->GetControlRotation();

	// 카메라 방향으로 바라보면 너무 낮게 보이는 문제를 해소하기 위해 Z축을 임의로 높임
	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.f, 0.f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetLocation);

	FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);
	
	// roll은 기존 컨트롤러 값으로 유지하고 pitch/yaw 값만 적용
	Character->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
}

void UTargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UTargetingComponent::SwitchingLockedOnActor(ESwitchingDirection InDirection)
{
	if (IsValid(LockedTargetActor))
	{
		if (ISoulTargeting* Targeting = Cast<ISoulTargeting>(LockedTargetActor))
		{
			// 현재 타게팅 된 대상의 위젯을 끈다.
			Targeting->OnTargeted(false);

			// 다시 근처의 타겟을 찾는다.
			TArray<AActor*> OutTargets;
			FindTargets(OutTargets);
			AActor* TargetActor = FindClosestTarget(OutTargets, InDirection);

			if (IsValid(TargetActor))
			{
				if (ISoulTargeting* NewTargeting = Cast<ISoulTargeting>(TargetActor))
				{
					LockedTargetActor = TargetActor;
					NewTargeting->OnTargeted(true);
				}
			}
			else
			{
				StopLockOn();
			}
		}
	}
}

void UTargetingComponent::FindTargets(TArray<AActor*>& OutTargetingActors) const
{
	TArray<FHitResult> OutHits;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));

	TArray<AActor*> ActorsToIgnore;

	// 스피어 범위 내에서 콜리전 오브젝트 타입이 Targeting인 녀석들을 모두 추출 
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TargetingRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		true
		);

	if (bHit)
	{
		for (const FHitResult& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			if (ISoulTargeting* Targeting = Cast<ISoulTargeting>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					OutTargetingActors.Add(HitActor);
				}
			}
		}
	}
}

AActor* UTargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection)
{
	// 가장 가까운 타게팅을 찾는 방법으로 카메라를 기준으로 가장 가까이 있는 대상을 찾는다.
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;

	for (const AActor* TargetActor : InTargets)
	{
		// 방향이 설정되어 있는 녀석 중에 이미 락온이 되어 있는 대상이면 컨티뉴
		if (InDirection != ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;
		}

		FHitResult OutHit;
		// 카메라 기준
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;

		// 라인 트레이스를 이용하기 위해서 Visibility 채널이 Block이어야 한다.
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true
			);

		if (bHit)
		{
			// 내적을 통해 필터링
			if (InDirection == ESwitchingDirection::Left)
			{
				// 카메라의 오른쪽 벡터와 대상의 노말 벡터를 내적해서 양수이면 왼쪽
				// 음수라면 필터링
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) > 0.f == false)
				{
					continue;
				}
			}
			
			if (InDirection == ESwitchingDirection::Right)
			{
				// 카메라의 오른쪽 벡터와 대상의 노말 벡터를 내적해서 음수이면 오른쪽
				// 양수라면 필터링
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) < 0.f == false)
				{
					continue;
				}
			}

			AActor* HitActor = OutHit.GetActor();

			// 플레이어와 가장 가까운 타겟을 찾기 위해 카메라의 전방 벡터와 대상을 내적한다.
			// 서로 마주보고 있으면 1, 좌우로 멀어질수록 0이므로 1과 가까운 값이 가장 가까이 있는 대상
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitActor->GetActorLocation());
			const float CheckValue = FVector::DotProduct(Camera->GetForwardVector(), LookAtRotation.Vector());

			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}
	return ClosestTarget;
}

void UTargetingComponent::OrientCamera() const
{
	// 카메라가 바라보는 방향과 캐릭터가 바라보는 방향을 일치시킨다.
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UTargetingComponent::OrientMovement() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UTargetingComponent::LockOnTarget()
{
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	
	AActor* TargetActor = FindClosestTarget(OutTargets);
	
	if (IsValid(TargetActor))
	{
		LockedTargetActor = TargetActor;
		bIsLockOn = true;
		if (ISoulTargeting* Targeting = Cast<ISoulTargeting>(LockedTargetActor))
		{
			// 타게팅 된 대상의 위젯을 켠다.
			Targeting->OnTargeted(bIsLockOn);
		}
		
		OrientCamera();
	}
}



