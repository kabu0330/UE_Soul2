﻿#include "SoulGameplayTag.h"

namespace SoulGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Attacking, "Character.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, "Character.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Sprinting, "Character.State.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_GeneralAction, "Character.State.GeneralAction");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Death, "Character.State.Death");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Movement_Disabled, "Character.State.Movement.Disabled");
	
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Equip, "Character.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Unequip, "Character.Action.Unequip");
	
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Light  , "Character.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Running, "Character.Attack.Running");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Special, "Character.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Heavy  , "Character.Attack.Heavy");
}