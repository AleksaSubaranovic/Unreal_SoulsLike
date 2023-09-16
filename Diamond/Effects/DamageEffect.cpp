// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffect.h"
#include "GameplayEffect.h"
#include "Diamond/DamageEffectExecutionCalculation.h"

UDamageEffect::UDamageEffect()
{
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UDamageEffectExecutionCalculation::StaticClass();
	Executions.Add(ExecDef);
	DurationPolicy = EGameplayEffectDurationType::Instant;
}