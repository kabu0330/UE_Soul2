// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soul/Interface/SoulInteract.h"
#include "SoulPickupItem.generated.h"

class ASoulEquipment;

UCLASS()
class SOUL_API ASoulPickupItem : public AActor, public ISoulInteract
{
	GENERATED_BODY()

public:
	ASoulPickupItem();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<ASoulEquipment>& NewEquipmentClass) {EquipmentClass = NewEquipmentClass;}

	/** ISoulInteract Interface */
	virtual void Interact(AActor* InteractActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem")
	TSubclassOf<ASoulEquipment> EquipmentClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickupItem")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	
};
