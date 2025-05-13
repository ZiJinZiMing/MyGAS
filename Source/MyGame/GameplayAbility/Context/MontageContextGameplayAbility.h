// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContextGameplayAbility.h"
#include "InstancedStruct.h"
#include "AbilityContext.h"
#include "MontageContextGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class MYGAME_API UMontageContextGameplayAbility : public UContextGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void StartAbility_Implementation(const TInstancedStruct<FAbilityContext>& Context) override;


	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* AbilityTask;


	UFUNCTION()
	void OnMontageEnded();


	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
