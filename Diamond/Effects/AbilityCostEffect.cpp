// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCostEffect.h"
#include "Diamond\CharacterAttributeSet.h"
// DEPRICATED
UAbilityCostEffect::UAbilityCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = UCharacterAttributeSet::GetHealthAttribute();
	HealthModifier.ModifierOp = EGameplayModOp::Additive;
	HealthModifier.ModifierMagnitude = FScalableFloat(0.f);
	Modifiers.Add(HealthModifier); // IMPORTANT TO BE 0 INDEX

	FGameplayModifierInfo ManaModifier;
	ManaModifier.Attribute = UCharacterAttributeSet::GetManaAttribute();
	ManaModifier.ModifierOp = EGameplayModOp::Additive;
	ManaModifier.ModifierMagnitude = FScalableFloat(0.f);
	Modifiers.Add(ManaModifier); // IMPORTANT TO BE 1 INDEX

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifier.ModifierOp = EGameplayModOp::Additive;
	StaminaModifier.ModifierMagnitude = FScalableFloat(0.f);
	Modifiers.Add(StaminaModifier); // IMPORTANT TO BE 2 INDEX
	
	// TODO: Tags for ignoring cost?
}

