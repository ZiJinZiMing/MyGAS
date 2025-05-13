// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"

#include "MyGameplayAbilityTargetData.h"
#include "MyGameplayAbilityTypes.h"
#include "Abilities/MyMontageGameplayAbility.h"


// Sets default values for this component's properties
UMyAbilitySystemComponent::UMyAbilitySystemComponent()
{
}

bool UMyAbilitySystemComponent::PlayMontageAbility(UAnimMontage* Montage, TSubclassOf<UMyMontageGameplayAbility> MontageAbility)
{
	// FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	UMyMontageGameplayAbility* InAbilityCDO = MontageAbility.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			// bSuccess |= TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
			FScopedPredictionWindow NewScopedWindow(this, true);

			FGameplayEventData EventData;
			FGameplayAbilityTargetData_Montage* TargetData = new FGameplayAbilityTargetData_Montage();
			TargetData->Montage = Montage;

			//一定要这样写，直接操作EventData.ContextHandle
			EventData.ContextHandle = MakeEffectContext();
			FMyGameplayEffectContext* MyGEContext = static_cast<FMyGameplayEffectContext*>(EventData.ContextHandle.Get());
			MyGEContext->AddTargetData(TargetData);
			
			return InternalTryActivateAbility(Spec.Handle, ScopedPredictionKey, nullptr, nullptr, &EventData);
		}
	}
	return false;
}

void UMyAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}
