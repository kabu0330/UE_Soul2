// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPickupItem.h"

#include "Soul/Soul.h"
#include "Soul/SoulDefine.h"
#include "Soul/Equipment/SoulEquipment.h"


ASoulPickupItem::ASoulPickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true); // 공중에 떠 있지 않고 바닥에 떨어뜨리기
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComponent->SetCollisionObjectType(COLLISION_OBJECT_INTERACT);
}

void ASoulPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (const ASoulEquipment* CDO = EquipmentClass->GetDefaultObject<ASoulEquipment>())
		{
			MeshComponent->SetStaticMesh(CDO->MeshAsset);
		}
	}
	else
	{
		LOG_ERROR("픽업 아이템 클래스에 EquipmentClass가 설정되지 않았습니다 : %s", *this->GetName());
	}
}

void ASoulPickupItem::Interact(AActor* InteractActor)
{
	DEBUG_MESSAGE("Pickup Item");
	FActorSpawnParameters SpawnParams;

	// 여기서 캐릭터가 Equipment의 오너임을 알려준다.
	SpawnParams.Owner = InteractActor;

	ASoulEquipment* Equipment = GetWorld()->SpawnActor<ASoulEquipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (Equipment)
	{
		Equipment->EquipItem();
		Destroy();
	}
}

