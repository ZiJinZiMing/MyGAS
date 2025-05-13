// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "MyPlayerState.h"
#include "MyGame/GameplayAbility/MyAbilitySystemComponent.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
}

UAbilitySystemComponent* AMyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystem();
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystem();
}

void AMyCharacter::OnAbilitySystemInitialized_Implementation()
{
}

void AMyCharacter::InitAbilitySystem()
{
	if (AMyPlayerState* PS = GetPlayerState<AMyPlayerState>())
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UMyAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		OnAbilitySystemInitialized();
	}
}
