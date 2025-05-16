// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"

#include "AbilitySystemLog.h"
#include "MyGameplayAbilityTargetData.h"
#include "MyGameplayAbilityTypes.h"
#include "Abilities/MyMontageGameplayAbility.h"
#include "Context/AbilityContext.h"
#include "Context/ContextGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"


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

bool UMyAbilitySystemComponent::TryActivateContextAbility(const TInstancedStruct<FAbilityContext>& Payload)
{
	// FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	const FAbilityContext& AbilityContext = Payload.Get<FAbilityContext>();
	UContextGameplayAbility* InAbilityCDO = AbilityContext.AbilityClass.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability.Get() == InAbilityCDO)
		{
			// bSuccess |= TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
			FScopedPredictionWindow NewScopedWindow(this, true);

			FGameplayAbilityTargetData_AbilityContext* TargetData = new FGameplayAbilityTargetData_AbilityContext();
			// TargetData->AbilityContext = Payload;

			// TargetData->AbilityContext = Payload.Get<FAbilityContext>();

			TargetData->AbilityContext.InitializeAs(Payload.GetScriptStruct(), Payload.GetMemory());

			//一定要这样写，直接操作EventData.ContextHandle

			FGameplayEventData EventData;
			//写法一
			// FGameplayEffectContextHandle GEContext = MakeEffectContext();
			// EventData.ContextHandle = GEContext;
			//写法二
			EventData.ContextHandle = MakeEffectContext();
			FMyGameplayEffectContext* MyGEContext = static_cast<FMyGameplayEffectContext*>(EventData.ContextHandle.Get());
			MyGEContext->AddTargetData(TargetData);

			return InternalTryActivateAbility(Spec.Handle, ScopedPredictionKey, nullptr, nullptr, &EventData);
		}
	}
	return false;
}

bool UMyAbilitySystemComponent::TryActivateAbilityByClassWithPayload(TSubclassOf<UGameplayAbility> InAbilityToActivate, FGameplayEventData Payload)
{
	UGameplayAbility* InAbilityCDO = InAbilityToActivate.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability.Get() == InAbilityCDO)
		{
			//保持和SendGameplayEventToActor中逻辑一致,
			FScopedPredictionWindow NewScopedWindow(this, true);

			return InternalTryActivateAbility(Spec.Handle, ScopedPredictionKey, nullptr, nullptr, &Payload);
		}
	}
	return false;
}

void UMyAbilitySystemComponent::CallClientSetReplicatedTargetData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey)
{
	ClientSetReplicatedTargetData(AbilityHandle, AbilityOriginalPredictionKey, ReplicatedTargetDataHandle, ApplicationTag, CurrentPredictionKey);
}

void UMyAbilitySystemComponent::ClientSetReplicatedTargetData_Implementation(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey)
{
	FScopedPredictionWindow ScopedPrediction(this, CurrentPredictionKey);

	// Always adds to cache to store the new data
	TSharedRef<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.FindOrAdd(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, AbilityOriginalPredictionKey));

	if (ReplicatedData->TargetData.Num() > 0)
	{
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityHandle);
		if (Spec && Spec->Ability)
		{
			// Can happen under normal circumstances if ServerForceClientTargetData is hit
			ABILITY_LOG(Display, TEXT("Ability %s is overriding pending replicated target data."), *Spec->Ability->GetName());
		}
	}

	ReplicatedData->TargetData = ReplicatedTargetDataHandle;
	ReplicatedData->ApplicationTag = ApplicationTag;
	ReplicatedData->bTargetConfirmed = true;
	ReplicatedData->bTargetCancelled = false;
	ReplicatedData->PredictionKey = CurrentPredictionKey;

	ReplicatedData->TargetSetDelegate.Broadcast(ReplicatedTargetDataHandle, ReplicatedData->ApplicationTag);
}

bool UMyAbilitySystemComponent::ClientSetReplicatedTargetData_Validate(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey)
{
	// check the data coming from the client to ensure it's valid
	for (const TSharedPtr<FGameplayAbilityTargetData>& TgtData : ReplicatedTargetDataHandle.Data)
	{
		if (!ensure(TgtData.IsValid()))
		{
			return false;
		}
	}

	return true;
}


void UMyAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}
