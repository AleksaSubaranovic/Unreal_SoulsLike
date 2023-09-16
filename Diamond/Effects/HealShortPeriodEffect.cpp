// Fill out your copyright notice in the Description page of Project Settings.


#include "HealShortPeriodEffect.h"
#include "Diamond/HealEffectExecutionCalculation.h"

#define SHORT_BUFF_TIME 6.f
#define SHORT_BUFF_APPLY 1.f

UHealShortPeriodEffect::UHealShortPeriodEffect()
{
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UHealEffectExecutionCalculation::StaticClass();
	Executions.Add(ExecDef);
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	this->OngoingTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.CanHeal")));
	this->InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.IsHeal")));
	this->InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.IsHelpful")));

	Heal_Duration.Value = SHORT_BUFF_TIME;
	Heal_EveryXSec.Value = SHORT_BUFF_APPLY;

	this->DurationMagnitude = FGameplayEffectModifierMagnitude(Heal_Duration);

	this->Period = Heal_EveryXSec;
}
