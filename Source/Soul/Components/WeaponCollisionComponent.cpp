// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCollisionComponent.h"

UWeaponCollisionComponent::UWeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UWeaponCollisionComponent::BroadcastHitActor(const FHitResult& HitResult)
{
	if (OnHitActor.IsBound())
	{
		OnHitActor.Broadcast(HitResult);
	}
}

void UWeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UWeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void UWeaponCollisionComponent::AddIgnoredActor(AActor* Actor)
{
	ActorsToIgnore.Add(Actor);
}

void UWeaponCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
	ActorsToIgnore.Remove(Actor);
}

bool UWeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void UWeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;
	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		Start, // 시작 위치
		End, // 끝 위치
		TraceRadius, // 스피어 반지름 길이
		TraceObjectTypes, // 오브젝트 타입
		false, 
		ActorsToIgnore, // 충돌 검사에서 제외할 액터(무기를 소유한 자신)
		DrawDebugType, // EDrawDebugTrace::ForDuration : 기본 5초 동안 
		OutHits, // 충돌한 액터를 모두 담아줄 배열
		true // 나 자신(CollisionComponent)은 제외
		);

	if (bHit)
	{
		for (const FHitResult& OutHit : OutHits)
		{
			AActor* HitActor = OutHit.GetActor();
			if (nullptr == HitActor) return;

			if (CanHitActor(HitActor)) // 이번 프레임에 충돌한 액터라면
			{
				AlreadyHitActors.Add(HitActor);
				BroadcastHitActor(OutHit);
			}
		}
	}
}



