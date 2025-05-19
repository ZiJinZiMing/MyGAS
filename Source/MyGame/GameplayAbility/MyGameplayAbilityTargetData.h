// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "MyGameplayAbilityTargetData.generated.h"


struct FAbilityContext;


USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_AbilityContext : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY()
	// TInstancedStruct<FAbilityContext> AbilityContext;
	FInstancedStruct AbilityContext;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
};


template <>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_AbilityContext> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_AbilityContext>
{
	enum
	{
		WithNetSerializer = true // For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};


USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_ActionValidator : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ActionIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ActionStep;

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




