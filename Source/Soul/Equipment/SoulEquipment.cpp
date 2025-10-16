// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulEquipment.h"

#include "GameFramework/Character.h"


ASoulEquipment::ASoulEquipment()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ASoulEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		MeshComponent->SetStaticMesh(MeshAsset);
	}
}

void ASoulEquipment::AttachToOwner(FName SocketName)
{
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh,
				FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
				SocketName);
		}
	}
}


