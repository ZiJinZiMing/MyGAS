// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"

#include "AbilitySystemLog.h"
#include "AbilitySystemStats.h"
#include "MyGameplayAbilityTargetData.h"
#include "MyGameplayAbilityTypes.h"
#include "Abilities/MyBaseGameplayAbility.h"
#include "Context/AbilityContext.h"
#include "Context/ContextGameplayAbility.h"

DECLARE_CYCLE_STAT(TEXT("AbilitySystemComp ServerTryActivate"), STAT_AbilitySystemComp_ServerTryActivate, STATGROUP_AbilitySystem);

#if !UE_BUILD_SHIPPING
extern int32 DenyClientActivation = 0;
/*static FAutoConsoleVariableRef CVarDenyClientActivation(
TEXT("AbilitySystem.DenyClientActivations"),
	DenyClientActivation,
	TEXT("Make server deny the next X ability activations from clients. For testing misprediction."),
	ECVF_Default
	);*/
#endif // !UE_BUILD_SHIPPING

// Sets default values for this component's properties
UMyAbilitySystemComponent::UMyAbilitySystemComponent()
{
}


void UMyAbilitySystemComponent::InternalServerTryActivateAbility(FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData)
{
	// Super::InternalServerTryActivateAbility(AbilityToActivate, InputPressed, PredictionKey, TriggerEventData);

#if WITH_SERVER_CODE
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DenyClientActivation > 0)
	{
		DenyClientActivation--;
		ClientActivateAbilityFailed(Handle, PredictionKey.Current);

		return;
	}
#endif

	ABILITYLIST_SCOPE_LOCK();

	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		// Can potentially happen in race conditions where client tries to activate ability that is removed server side before it is received.
		UE_VLOG_UELOG(GetOwner(), LogAbilitySystem, Display, TEXT("%hs: Rejecting ClientActivation of ability with invalid SpecHandle!"), __func__);
		ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	const UGameplayAbility* AbilityToActivate = Spec->Ability;

	if (!ensure(AbilityToActivate))
	{
		UE_VLOG_UELOG(GetOwner(), LogAbilitySystem, Error, TEXT("%hs: Rejecting ClientActivation of unconfigured spec ability!"), __func__);
		ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	// Ignore a client trying to activate an ability requiring server execution
	if (AbilityToActivate->GetNetSecurityPolicy() == EGameplayAbilityNetSecurityPolicy::ServerOnlyExecution ||
		AbilityToActivate->GetNetSecurityPolicy() == EGameplayAbilityNetSecurityPolicy::ServerOnly)
	{
		UE_VLOG_UELOG(GetOwner(), LogAbilitySystem, Display, TEXT("%hs: Rejecting ClientActivation of %s due to security policy violation."), __func__, *GetNameSafe(AbilityToActivate));
		ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	// Consume any pending target info, to clear out cancels from old executions
	ConsumeAllReplicatedData(Handle, PredictionKey);

	FScopedPredictionWindow ScopedPredictionWindow(this, PredictionKey);

	ensure(AbilityActorInfo.IsValid());

	SCOPE_CYCLE_COUNTER(STAT_AbilitySystemComp_ServerTryActivate);
	SCOPE_CYCLE_UOBJECT(Ability, AbilityToActivate);

	UGameplayAbility* InstancedAbility = nullptr;
	Spec->InputPressed = true;

	// Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
	if (InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
	{
		// TryActivateAbility handles notifying the client of success, but let's still log it
		UE_VLOG_UELOG(GetOwner(), LogAbilitySystem, Verbose, TEXT("%hs: Accepted ClientActivation of %s with PredictionKey %s."), __func__, *GetNameSafe(Spec->Ability), *PredictionKey.ToString());
	}
	else
	{
		UE_VLOG_UELOG(GetOwner(), LogAbilitySystem, Display, TEXT("%hs: Rejecting ClientActivation of %s with PredictionKey %s. InternalTryActivateAbility failed: %s"),
			__func__, *GetNameSafe(Spec->Ability), *PredictionKey.ToString(), *InternalTryActivateAbilityFailureTags.ToStringSimple() );

		ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;

		//ZhangJinming Modifier, processing for ability rejected
		ServerTryActivateAbilityRejected(Handle, TriggerEventData);
		//ZhangJinming Modifier
		
		MarkAbilitySpecDirty(*Spec);
	}
#endif
}

void UMyAbilitySystemComponent::ServerTryActivateAbilityRejected(FGameplayAbilitySpecHandle Handle, const FGameplayEventData* EventData)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		ABILITY_LOG(Warning, TEXT("InternalTryActivateAbility called with a valid handle but no matching ability was found. Handle: %s ASC: %s. AvatarActor: %s"), *Handle.ToString(), *GetPathName(), *GetNameSafe(GetAvatarActor_Direct()));
		return;
	}


	//CDO
	UGameplayAbility* Ability = Spec->Ability;
	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

	//only LocalPredicted policy
	check(Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted);

	//
	// If it's an instanced one, the instanced ability will be set, otherwise it will be null
	UGameplayAbility* InstancedAbility = Spec->GetPrimaryInstance();
	UGameplayAbility* AbilitySource = InstancedAbility ? InstancedAbility : Ability;
	//
	if (UMyBaseGameplayAbility* BaseAbility = Cast<UMyBaseGameplayAbility>(AbilitySource))
	{
		BaseAbility->OnActivateAbilityRejected(*ActorInfo, *EventData);
	}
	
}

bool UMyAbilitySystemComponent::TryActivateContextAbility(const TInstancedStruct<FAbilityContext>& Payload)
{
	const FAbilityContext& AbilityContext = Payload.Get<FAbilityContext>();
	UContextGameplayAbility* InAbilityCDO = AbilityContext.AbilityClass.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability.Get() == InAbilityCDO)
		{
			// bSuccess |= TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
			FScopedPredictionWindow NewScopedWindow(this, true);


			
			FGameplayEventData EventData;
			EventData.ContextHandle = MakeEffectContext();
			FMyGameplayEffectContext* MyGEContext = static_cast<FMyGameplayEffectContext*>(EventData.ContextHandle.Get());

			FGameplayAbilityTargetData_AbilityContext* TargetData = new FGameplayAbilityTargetData_AbilityContext();
			TargetData->AbilityContext.InitializeAs(Payload.GetScriptStruct(), Payload.GetMemory());
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

bool UMyAbilitySystemComponent::TryActivateAbilityByClassWithTargetData(TSubclassOf<UGameplayAbility> InAbilityToActivate, const FGameplayAbilityTargetDataHandle& TargetData)
{
	UGameplayAbility* InAbilityCDO = InAbilityToActivate.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability.Get() == InAbilityCDO)
		{
			//保持和SendGameplayEventToActor中逻辑一致,
			FScopedPredictionWindow NewScopedWindow(this, true);
			FGameplayEventData EventData;
			EventData.EventTag = UE::MyGAS::Tag_TryActivateAbility;
			// EventData.EventTag = FGameplayTag::EmptyTag;
			EventData.TargetData = TargetData;
			return InternalTryActivateAbility(Spec.Handle, ScopedPredictionKey, nullptr, nullptr, &EventData);
		}
	}
	return false;
}

void UMyAbilitySystemComponent::ClientActivateAbilitySucceedWithEventData_Implementation(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey PredictionKey, FGameplayEventData TriggerEventData)
{
	Super::ClientActivateAbilitySucceedWithEventData_Implementation(AbilityToActivate, PredictionKey, TriggerEventData);
}

