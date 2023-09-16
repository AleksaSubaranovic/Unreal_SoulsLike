// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAttributeSet.h"

UWeaponAttributeSet::UWeaponAttributeSet()
{
	InitLeftWeaponPhysicalDamage(1.f);
	InitLeftWeaponMagicalDamage(0.f);
	InitLeftWeaponFireDamage(0.f);
	InitLeftWeaponFrostDamage(0.f);
	InitLeftWeaponHolyDamage(0.f);

	InitRightWeaponPhysicalDamage(1.f);
	InitRightWeaponMagicalDamage(0.f);
	InitRightWeaponFireDamage(0.f);
	InitRightWeaponFrostDamage(0.f);
	InitRightWeaponHolyDamage(0.f);
}

void UWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UWeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
