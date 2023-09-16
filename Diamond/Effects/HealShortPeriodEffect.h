// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "HealShortPeriodEffect.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UHealShortPeriodEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UHealShortPeriodEffect();

	UPROPERTY(VisibleAnywhere)
		FScalableFloat Heal_Duration;

	UPROPERTY(VisibleAnywhere)
		FScalableFloat Heal_EveryXSec;
};
