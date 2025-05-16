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
	UFUNCTION(BlueprintCallable)
	bool PlayMontageAbility(UAnimMontage* Montage, TSubclassOf<UMyMontageGameplayAbility> MontageAbility);

	UFUNCTION(BlueprintCallable, Category = "Context Ability System")
	bool TryActivateContextAbility(const TInstancedStruct<FAbilityContext>& Payload);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByClassWithPayload(TSubclassOf<UGameplayAbility> InAbilityToActivate, FGameplayEventData Payload);

	void OnLocalPredictionAbilityRejected(FPredictionKey AbilityPredictionKey, UGameplayAbility* Ability);

	
	void CallClientSetReplicatedTargetData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey);

	/** Replicates targeting data to the server */
	UFUNCTION(Client, reliable, WithValidation)
	void ClientSetReplicatedTargetData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey);

protected:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
};
