// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

UMyBaseGameplayAbility::UMyBaseGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UMyBaseGameplayAbility::OnClientActivateAbilityRejected() const
{
	UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("OnClientActivateAbilityRejected: %s"),*GetName()));
}

void UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp() const
{
	UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("OnClientActivateAbilityCaughtUp: %s"),*GetName()));
	
}

/*
void UMyBaseGameplayAbility::OnClientActivateAbilityRejected_Implementation() const
{
	UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("OnClientActivateAbilityRejected: %s"),*GetName()));
}

void UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp_Implementation() const
{
	UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("OnClientActivateAbilityCaughtUp: %s"),*GetName()));
}*/

void UMyBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (IsPredictingClient())
	{
		FPredictionKey ScopedPredictionKey = GetCurrentActivationInfo().GetActivationPredictionKey();
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ActivateAbility: PredictionKey：%s"), *ScopedPredictionKey.ToString()));
		
		ScopedPredictionKey.NewRejectedDelegate().BindUObject(this, &UMyBaseGameplayAbility::OnClientActivateAbilityRejected);
		ScopedPredictionKey.NewCaughtUpDelegate().BindUObject(this, &UMyBaseGameplayAbility::OnClientActivateAbilityCaughtUp);
	}
}

void UMyBaseGameplayAbility::ConfirmActivateSucceed()
{
	Super::ConfirmActivateSucceed();
	UKismetSystemLibrary::PrintString(this,TEXT("ConfirmActivateSucceed"));
	OnConfirmActivateSucceed();
}
