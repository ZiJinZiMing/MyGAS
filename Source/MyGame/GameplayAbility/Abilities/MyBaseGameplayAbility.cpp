// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"
#include "MyGame/GameplayAbility/MyAbilitySystemComponent.h"

UMyBaseGameplayAbility::UMyBaseGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UMyBaseGameplayAbility::SendClientTargetData(const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag)
{
	const FGameplayAbilityActivationInfo& ActivationInfo = GetCurrentActivationInfo();
	FPredictionKey ScopedPredictionKey = ActivationInfo.GetActivationPredictionKey();
	if (!HasAuthorityOrPredictionKey(GetCurrentActorInfo(), &ActivationInfo)) return;
	if (UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		ASC->CallClientSetReplicatedTargetData(GetCurrentAbilitySpecHandle(), ScopedPredictionKey, ReplicatedTargetDataHandle, ApplicationTag, ScopedPredictionKey);
	}
}

/*
void UMyBaseGameplayAbility::OnClientActivateAbilityRejected() const
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnClientActivateAbilityRejected: %s"), *GetName()));
}

void UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp() const
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnClientActivateAbilityCaughtUp: %s"), *GetName()));
}
*/

void UMyBaseGameplayAbility::OnClientActivateAbilityRejected_Implementation() const
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnClientActivateAbilityRejected: %s"), *GetName()));
}

void UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp_Implementation() const
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnClientActivateAbilityCaughtUp: %s"), *GetName()));
}

void UMyBaseGameplayAbility::OnClientTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnTargetDataReplicatedCallback::%s"), *GameplayTag.ToString()));

	if (UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		const FGameplayAbilityActivationInfo& ActivationInfo = GetCurrentActivationInfo();
		FPredictionKey ScopedPredictionKey = ActivationInfo.GetActivationPredictionKey();

		//consume targetdata
		ASC->ConsumeClientReplicatedTargetData(GetCurrentAbilitySpecHandle(), ScopedPredictionKey);

		OnClientReceiveTargetData(GameplayAbilityTargetDataHandle, GameplayTag);
	}
}

void UMyBaseGameplayAbility::OnClientReceiveTargetData_Implementation(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag)
{
}

void UMyBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (IsPredictingClient())
	{
		FPredictionKey ScopedPredictionKey = GetCurrentActivationInfo().GetActivationPredictionKey();
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ActivateAbility: PredictionKey：%s"), *ScopedPredictionKey.ToString()));
		UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		if (ASC)
		{
			//InstancedPerExecution的GA会无法触发OnClientActivateAbilityRejected，因为当Reject下发时，GA已经被标记为Garbage
			//InstancePerExecution的GA应该使用EndAbility作为结束
			ScopedPredictionKey.NewRejectedDelegate().BindUObject(this, &UMyBaseGameplayAbility::OnClientActivateAbilityRejected);

			ScopedPredictionKey.NewRejectedDelegate().BindUObject(ASC, &UMyAbilitySystemComponent::OnLocalPredictionAbilityRejected, ScopedPredictionKey, static_cast<UGameplayAbility*>(this));
		}

		ScopedPredictionKey.NewCaughtUpDelegate().BindUObject(this, &UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp);

		//TargetDataDelegate
		//Since multifire is supported, we still need to hook up the callbacks
		ASC->AbilityTargetDataSetDelegate(Handle, ScopedPredictionKey).AddUObject(this, &ThisClass::OnClientTargetDataReplicatedCallback);
	}
}


void UMyBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		//dispose
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EndAbility: %s"), *GetName()));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMyBaseGameplayAbility::ConfirmActivateSucceed()
{
	Super::ConfirmActivateSucceed();
	UKismetSystemLibrary::PrintString(this,TEXT("ConfirmActivateSucceed"));
	OnConfirmActivateSucceed();
}
