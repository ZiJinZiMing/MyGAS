// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilityBlueprintFunctionLibrary.h"

bool UMyAbilityBlueprintFunctionLibrary::GetActionValidatorDataFromTargetData(FGameplayAbilityTargetDataHandle Handle, int& ActionIndex,
                                                                              int& ActionStep)
{
	if (FGameplayAbilityTargetData_ActionValidator* TargetData = static_cast<FGameplayAbilityTargetData_ActionValidator*>(Handle.Get(0)))
	{
		ActionIndex = TargetData->ActionIndex;
		ActionStep = TargetData->ActionStep;
		return true;
	}
	return false;;
}

FGameplayAbilityTargetDataHandle UMyAbilityBlueprintFunctionLibrary::MakeActionValidatorTargetData(int ActionIndex, int ActionStep)
{
	FGameplayAbilityTargetData_ActionValidator* TargetData = new FGameplayAbilityTargetData_ActionValidator();
	TargetData->ActionIndex = ActionIndex;
	TargetData->ActionStep = ActionStep;
	return FGameplayAbilityTargetDataHandle(TargetData);
}
