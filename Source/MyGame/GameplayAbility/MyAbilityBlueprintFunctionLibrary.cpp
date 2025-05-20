// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilityBlueprintFunctionLibrary.h"

#include "Abilities/GameplayAbilityTypes.h"

bool UMyAbilityBlueprintFunctionLibrary::GetActionValidatorDataFromTargetData(FGameplayAbilityTargetDataHandle Handle, int& ActionIndex,
                                                                              int& ActionStep, int& EffectIndex)
{
	if (FGameplayAbilityTargetData_ActionValidator* TargetData = static_cast<FGameplayAbilityTargetData_ActionValidator*>(Handle.Get(0)))
	{
		ActionIndex = TargetData->ActionIndex;
		ActionStep = TargetData->ActionStep;
		EffectIndex = TargetData->EffectIndex;
		return true;
	}
	return false;;
}

FGameplayAbilityTargetDataHandle UMyAbilityBlueprintFunctionLibrary::MakeActionValidatorTargetData(int ActionIndex, int ActionStep, int EffectIndex)
{
	FGameplayAbilityTargetData_ActionValidator* TargetData = new FGameplayAbilityTargetData_ActionValidator();
	TargetData->ActionIndex = ActionIndex;
	TargetData->ActionStep = ActionStep;
	TargetData->EffectIndex = EffectIndex;
	return FGameplayAbilityTargetDataHandle(TargetData);
}

bool UMyAbilityBlueprintFunctionLibrary::IsForRemoteClient(const FGameplayAbilityActorInfo& ActorInfo)
{
	// const FGameplayAbilityActorInfo* const CurrentActorInfoPtr = GetCurrentActorInfo();
	if (ActorInfo.OwnerActor.IsValid())
	{
		bool bIsLocallyControlled = ActorInfo.IsLocallyControlled();
		bool bIsAuthority = ActorInfo.IsNetAuthority();

		if (bIsAuthority && !bIsLocallyControlled)
		{
			return true;
		}
	}

	return false;
}
