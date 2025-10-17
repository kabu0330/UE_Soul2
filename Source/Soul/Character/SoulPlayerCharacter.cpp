// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Components/AttributeComponent.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/SoulCharacterMovementComponent.h"
#include "Soul/Components/StateComponent.h"
#include "Soul/Equipment/SoulWeapon.h"
#include "Soul/Interface/SoulInteract.h"

ASoulPlayerCharacter::ASoulPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,540.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = TargetArmLength;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 스프링 암이 캐릭터 회전을 따라가므로 카메라는 꺼준다.
}

void ASoulPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASoulPlayerCharacter::BroadcastStateMessage(const FString& Message)
{
	if (OnStateMessage.IsBound())
	{
		OnStateMessage.Broadcast(Message);
	}
}

void ASoulPlayerCharacter::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (false == IsValid(RollingMontage))
	{
		LOG_ERROR("플레이어에 RollingMontage가 설정되어 있지 않아 Rolling 동작을 실행할 수 없습니다.");
		return;
	}
	
	if (AttributeComponent->CheckHasEnoughStamina(RollingCost))
	{
		// 1. 스태미나 감소
		AttributeComponent->DecreaseStamina(RollingCost);

		// 2. 스태미나 회복 중지
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 3. 이동 입력 중지
		StateComponent->ToggleMovementInput(false);

		// 4. 상태 설정
		StateComponent->AddGameplayTag(SoulGameplayTag::Character_State_Rolling);
		
		// 5. 애니메이션 실행
		PlayAnimMontage(RollingMontage);

		// 6. 스태미나 회복 타이머 예약
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASoulPlayerCharacter::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 150.f; 

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACT));

	TArray<AActor*> ActorToIgnore;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, Start, End, Radius, ObjectTypes,false,
		ActorToIgnore, EDrawDebugTrace::ForDuration, OutHit,true);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (ISoulInteract* Interaction = Cast<ISoulInteract>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void ASoulPlayerCharacter::ToggleCombatMode()
{
	check(CombatComponent);

	// 무기를 장착하고 있는지?
	const ASoulWeapon* Weapon = CombatComponent->GetMainWeapon();
	if (false == IsValid(Weapon)) return;

	// 전투 모드로 변환 가능한 상태인지?
	if (CanToggleCombatMode() == false)
	{
		const FString Message = TEXT("지금은 전투 혹은 비전투 상태로 변경할 수 없습니다.");
		BroadcastStateMessage(Message);
		return;
	}

	UAnimMontage* AnimMontage = nullptr;
	if (CombatComponent->IsCombatEnabled()) // 전투 모드일 때 토글 함수가 호출되면 비전투 모드로
	{
		AnimMontage = Weapon->GetMontageForTag(SoulGameplayTag::Character_Action_Unequip);
	}
	else // 비전투 모드일 때 토글 함수가 호출되면 전투 모드로
	{
		AnimMontage = Weapon->GetMontageForTag(SoulGameplayTag::Character_Action_Equip);
	}

	if (false == IsValid(AnimMontage))
	{
		LOG_ERROR("Weapon에 무기 장착/탈착 애님 몽타주 또는 태그가 설정되지 않았습니다.: %s", *Weapon->GetName());
		return;
	}

	// 상태 등록 및 애니메이션 실행
	StateComponent->AddGameplayTag(SoulGameplayTag::Character_State_GeneralAction);
	PlayAnimMontage(AnimMontage);
}

void ASoulPlayerCharacter::AutoCombatMode()
{
	check(CombatComponent);
	
	// 비전투 상태일 때만 토글 함수가 호출되므로 항상 전투모드로 전환시켜주는 함수
	if (CombatComponent->IsCombatEnabled() == false)
	{
		ToggleCombatMode();
	}
}

bool ASoulPlayerCharacter::CanToggleCombatMode() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SoulGameplayTag::Character_State_GeneralAction);
	CheckTags.AddTag(SoulGameplayTag::Character_State_Rolling);
	CheckTags.AddTag(SoulGameplayTag::Character_State_Attacking);
	
	return false == StateComponent->IsAnyActiveGameplayTags(CheckTags);
}

void ASoulPlayerCharacter::Attack()
{
	check(CombatComponent);
	CombatComponent->Attack();
}

void ASoulPlayerCharacter::SpecialAttack()
{
	check(CombatComponent);
	CombatComponent->SpecialAttack();
}

void ASoulPlayerCharacter::HeavyAttack()
{
	check(CombatComponent);
	CombatComponent->HeavyAttack();
}


