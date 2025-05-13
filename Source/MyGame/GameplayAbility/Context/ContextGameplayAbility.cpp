// © 2025 mrbaconvn. All Rights Reserved.


#include "ContextGameplayAbility.h"

#include "MyGame/GameplayAbility/MyGameplayAbilityTargetData.h"
#include "MyGame/GameplayAbility/MyGameplayAbilityTypes.h"

void UContextGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FMyGameplayEffectContext* GEContext = static_cast<const FMyGameplayEffectContext*>(TriggerEventData->ContextHandle.Get());
	FGameplayAbilityTargetData_AbilityContext* TargetData = static_cast<FGameplayAbilityTargetData_AbilityContext*>(GEContext->GetTargetData().Get(0));
	if (!TargetData)
	{
		K2_EndAbility();
		return;
	}

	

	TInstancedStruct<FAbilityContext> InstancedStruct;
	InstancedStruct.InitializeAsScriptStruct(TargetData->AbilityContext.GetScriptStruct(),TargetData->AbilityContext.GetMutableMemory());
	StartAbility(InstancedStruct);
} 

void UContextGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	 
	
}

void UContextGameplayAbility::StartAbility_Implementation(const TInstancedStruct<FAbilityContext>& Context)
{
}
