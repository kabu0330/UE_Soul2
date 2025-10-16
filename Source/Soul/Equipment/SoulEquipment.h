// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulEquipment.generated.h"

UCLASS()
class SOUL_API ASoulEquipment : public AActor
{
	GENERATED_BODY()

public:
	ASoulEquipment();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// 파츠가 어디냐에 따라 EquipItem 구현이 달라야 하므로
	virtual void EquipItem() {};
	virtual void UnequipItem() {};
	virtual void AttachToOwner(FName SocketName);

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TObjectPtr<UStaticMesh> MeshAsset;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent;


};
