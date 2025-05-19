// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Context/AbilityContext.h"
#include "StructUtils/InstancedStruct.h"
#include "MyAbilitySystemComponent.generated.h"


class UMyMontageGameplayAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYGAME_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyAbilitySystemComponent();

public:

	UFUNCTION(BlueprintCallable, Category = "Context Ability System")
	bool TryActivateContextAbility(const TInstancedStruct<FAbilityContext>& Payload);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByClassWithPayload(TSubclassOf<UGameplayAbility> InAbilityToActivate, FGameplayEventData Payload);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByClassWithTargetData(TSubclassOf<UGameplayAbility> InAbilityToActivate, const FGameplayAbilityTargetDataHandle& TargetData);


public:
	
	UFUNCTION(BlueprintCallable)
	bool PlayMontageAbility(UAnimMontage* Montage, TSubclassOf<UMyMontageGameplayAbility> MontageAbility);

protected:
	/** Implementation of ServerTryActivateAbility */
	virtual void InternalServerTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData) override;

	//processing for ability rejected
	void ServerTryActivateAbilityRejected(FGameplayAbilitySpecHandle Handle, const FGameplayEventData* EventData);

	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;

	virtual void NotifyAbilityCommit(UGameplayAbility* Ability) override;

	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void ClientActivateAbilitySucceedWithEventData_Implementation(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey PredictionKey, FGameplayEventData TriggerEventData) override;
};
