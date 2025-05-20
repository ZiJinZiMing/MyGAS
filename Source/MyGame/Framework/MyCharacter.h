// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UMyAbilitySystemComponent;

UCLASS()
class MYGAME_API AMyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(BlueprintReadOnly, Category=Ability)
	TObjectPtr<UMyAbilitySystemComponent> AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
protected:
	//从PlayerState中初始化AbilitySystemComponent
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnAbilitySystemInitialized();
	
private:
	void InitAbilitySystem();
	
};
