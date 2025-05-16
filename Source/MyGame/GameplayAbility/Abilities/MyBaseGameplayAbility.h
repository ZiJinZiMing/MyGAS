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


	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SendClientTargetData(const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintNativeEvent)
	void OnClientActivateAbilityRejected() const;
	virtual void OnClientActivateAbilityRejected_Implementation() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnClientActivateAbilityCaughtUp() const;
	virtual void OnClientActivateAbilityCaughtUp_Implementation() const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ConfirmActivateSucceed() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnConfirmActivateSucceed();

	void OnClientTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);

	UFUNCTION(BlueprintNativeEvent)
	void OnClientReceiveTargetData(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
	
};
