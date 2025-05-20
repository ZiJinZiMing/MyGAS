// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "MyGameplayAbilityTargetData.generated.h"




USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_ActionValidator : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ActionIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ActionStep;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EffectIndex;

	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
};


template <>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_ActionValidator> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_ActionValidator>
{
	enum
	{
		WithNetSerializer = true // For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};




