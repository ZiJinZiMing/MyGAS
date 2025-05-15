// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameplayAbilityTargetData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyAbilityBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UMyAbilityBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool GetActionValidatorDataFromTargetData(FGameplayAbilityTargetDataHandle Handle, int& ActionIndex, int& ActionStep);

	UFUNCTION(BlueprintCallable)
	static FGameplayAbilityTargetDataHandle MakeActionValidatorTargetData(int ActionIndex, int ActionStep);
};
