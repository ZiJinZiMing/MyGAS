// © 2025 mrbaconvn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPath.h"
#include "AbilityContext.generated.h"

class UContextGameplayAbility; 

USTRUCT(BlueprintType, meta = (UsesHierarchy))
struct MYGAME_API FAbilityContext
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UContextGameplayAbility> AbilityClass;
};


USTRUCT(BlueprintType)
struct MYGAME_API FAbilityContext_Montage : public FAbilityContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> Montage;
	
};