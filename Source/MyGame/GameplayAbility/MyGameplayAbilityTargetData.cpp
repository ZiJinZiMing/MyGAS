// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbilityTargetData.h"


bool FGameplayAbilityTargetData_ActionValidator::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	Ar << ActionIndex;
	Ar << ActionStep;
	Ar << EffectIndex;
	return bOutSuccess;
}
