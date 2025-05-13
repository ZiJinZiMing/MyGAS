// © 2025 mrbaconvn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "StructUtils/InstancedStruct.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityContext.h"
#include "ContextGameplayAbility.generated.h"

class UContextAbilitySystemComponent; 

UCLASS(Blueprintable)
class MYGAME_API UContextGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void StartAbility(const TInstancedStruct<FAbilityContext>& Context);
};
