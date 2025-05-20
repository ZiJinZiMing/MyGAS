// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameplayAbilityTargetData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyAbilityBlueprintFunctionLibrary.generated.h"

struct FGameplayAbilityActorInfo;

/**
 * 
 */
UCLASS()
class MYGAME_API UMyAbilityBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool GetActionValidatorDataFromTargetData(FGameplayAbilityTargetDataHandle Handle, int& ActionIndex, int& ActionStep, int& EffectIndex);

	UFUNCTION(BlueprintPure)
	static FGameplayAbilityTargetDataHandle MakeActionValidatorTargetData(int ActionIndex, int ActionStep, int EffectIndex);

	UFUNCTION(BlueprintPure)
	static bool IsForRemoteClient(const FGameplayAbilityActorInfo& ActorInfo);
};
