// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbilityTypes.h"



bool FMyGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	return Super::NetSerialize(Ar, Map, bOutSuccess) && TargetData.NetSerialize(Ar, Map, bOutSuccess);
}
