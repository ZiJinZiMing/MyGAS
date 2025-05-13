// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMontageGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyGame/GameplayAbility/MyGameplayAbilityTargetData.h"


UMyMontageGameplayAbility::UMyMontageGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyMontageGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	const FGameplayAbilityTargetData_Montage* TargetData = static_cast<const FGameplayAbilityTargetData_Montage*>(TriggerEventData->TargetData.Get(0));
	if (!TargetData)
	{
		K2_EndAbility();
		return;
	}
	AbilityTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,TEXT("MontageAbility"), TargetData->Montage);
	AbilityTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->ReadyForActivation();
}


void UMyMontageGameplayAbility::OnMontageEnded()
{
	K2_EndAbility();
}

void UMyMontageGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityTask)
	{
		AbilityTask->EndTask();
		AbilityTask = nullptr;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("MontageAbility End")));
}
