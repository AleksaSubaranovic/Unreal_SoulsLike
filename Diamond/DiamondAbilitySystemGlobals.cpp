// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondAbilitySystemGlobals.h"
#include "DiamondObjectAndStuff.h"

FGameplayEffectContext* UDiamondAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FDiamondGameplayEffectContext();
}
