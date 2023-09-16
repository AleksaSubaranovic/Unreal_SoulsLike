// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(100.f);

	InitMaxMana(100.f);
	InitMana(100.f);

	InitMaxStamina(100.f);
	InitStamina(100.f);

	InitMoveSpeed(400.f);

	InitAttackSpeed(1.f);

	InitCastSpeed(1.f);

	InitPhysicalDamageModifier(1.f);
	InitMagicalDamageModifier(1.f);

	InitPhysicalArmor(0.f);
	InitMagicalArmor(0.f);

	InitMaxPoise(100.f);
	InitPoise(100.f);
	
	InitFireResistance(0.f);
	InitFrostResistance(0.f);
	
	InitFireDamageModifier(1.f);
	InitFrostDamageModifier(1.f);
	InitHolyDamageModifier(1.f);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetPoiseAttribute())
	{
		SetPoise(FMath::Clamp(GetPoise(), 0.0f, GetMaxPoise()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Clamp(GetMaxHealth(), 0.0f, MaxVal));

		if (GetMaxHealth() < GetHealth())
			GetOwningAbilitySystemComponent()->ApplyModToAttributeUnsafe(GetHealthAttribute(),
				EGameplayModOp::Additive, GetMaxHealth() - GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMaxMana(FMath::Clamp(GetMaxMana(), 0.0f, MaxVal));

		if (GetMaxMana() < GetMana())
			GetOwningAbilitySystemComponent()->ApplyModToAttributeUnsafe(GetManaAttribute(),
				EGameplayModOp::Additive, GetMaxMana() - GetMana());
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		SetMaxStamina(FMath::Clamp(GetMaxStamina(), 0.0f, MaxVal));

		if (GetMaxStamina() < GetStamina())
			GetOwningAbilitySystemComponent()->ApplyModToAttributeUnsafe(GetStaminaAttribute(),
				EGameplayModOp::Additive, GetMaxStamina() - GetStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetMaxPoiseAttribute())
	{
		SetMaxPoise(FMath::Clamp(GetMaxPoise(), 0.0f, MaxVal));

		if (GetMaxPoise() < GetPoise())
			GetOwningAbilitySystemComponent()->ApplyModToAttributeUnsafe(GetPoiseAttribute(),
				EGameplayModOp::Additive, GetMaxPoise() - GetPoise());
	}
}
