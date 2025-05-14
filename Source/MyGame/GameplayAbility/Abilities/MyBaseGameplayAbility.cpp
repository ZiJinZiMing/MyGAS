// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

UMyBaseGameplayAbility::UMyBaseGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UMyBaseGameplayAbility::OnClientActivateAbilityRejected() const
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("ClientActivateAbilityRejected"));
}

void UMyBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (IsPredictingClient())
	{
		FPredictionKey ScopedPredictionKey = GetCurrentActivationInfo().GetActivationPredictionKey();
		ScopedPredictionKey.NewRejectedDelegate().BindUObject(this, &ThisClass::OnClientActivateAbilityRejected);
	}
}

void UMyBaseGameplayAbility::ConfirmActivateSucceed()
{
	Super::ConfirmActivateSucceed();
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("ConfirmActivateSucceed"));
}
