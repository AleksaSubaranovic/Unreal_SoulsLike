// Fill out your copyright notice in the Description page of Project Settings.


#include "HealInstantEffect.h"
#include "Diamond/HealEffectExecutionCalculation.h"

UHealInstantEffect::UHealInstantEffect()
{
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UHealEffectExecutionCalculation::StaticClass();
	Executions.Add(ExecDef);
	DurationPolicy = EGameplayEffectDurationType::Instant;

	this->ApplicationTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.CanHeal")));
	this->InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.IsHeal")));
	this->InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.IsHelpful")));
}

