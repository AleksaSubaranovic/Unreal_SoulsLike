// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DiamondAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UDiamondAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
protected:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
