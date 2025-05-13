// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageContextGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetSystemLibrary.h"

void UMontageContextGameplayAbility::StartAbility_Implementation(const TInstancedStruct<FAbilityContext>& Context)
{
	const FAbilityContext_Montage& MontageContext = Context.Get<FAbilityContext_Montage>();

	
	AbilityTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,TEXT("MontageAbility"), MontageContext.Montage);
	AbilityTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	AbilityTask->ReadyForActivation();
}


void UMontageContextGameplayAbility::OnMontageEnded()
{
	K2_EndAbility();
}

void UMontageContextGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AbilityTask)
	{
		AbilityTask->EndTask();
		AbilityTask = nullptr;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("MontageAbility End")));
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
