// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbilityTargetData.h"

bool FGameplayAbilityTargetData_Montage::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	Ar << Montage;
	return bOutSuccess;
}

bool FGameplayAbilityTargetData_AbilityContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	//需要转换成FInstancedStruct的网络序列化方式
	AbilityContext.NetSerialize(Ar, Map, bOutSuccess);
	return bOutSuccess;
}

bool FGameplayAbilityTargetData_ActionValidator::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	Ar << ActionIndex;
	Ar << ActionStep;
	return bOutSuccess;
}
