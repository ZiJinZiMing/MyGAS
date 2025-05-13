// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "MyGameplayAbilityTargetData.generated.h"



USTRUCT()
struct FGameplayAbilityTargetData_Montage : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY()
	TObjectPtr<UAnimMontage> Montage;
	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
};


template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_Montage> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_Montage>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};

