// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyBaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API UMyBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyBaseGameplayAbility();

	UFUNCTION(BlueprintNativeEvent)
	void OnClientActivateAbilityRejected() const;
	virtual void OnClientActivateAbilityRejected_Implementation() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnClientActivateAbilityCaughtUp() const;
	virtual void OnClientActivateAbilityCaughtUp_Implementation() const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintNativeEvent)
	void OnActivateAbilityRejected(const FGameplayAbilityActorInfo& ActorInfo, const FGameplayEventData& EventData);
};
