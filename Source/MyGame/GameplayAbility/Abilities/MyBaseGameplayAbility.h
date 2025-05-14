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

	UMyBaseGameplayAbility();

	void OnClientActivateAbilityRejected() const;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ConfirmActivateSucceed() override;
	
};
