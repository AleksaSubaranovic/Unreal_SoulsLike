// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATT_MAX_VAL 99999.f

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class DIAMOND_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharacterAttributeSet();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mana)
		FGameplayAttributeData Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mana)
		FGameplayAttributeData MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
		FGameplayAttributeData MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
		FGameplayAttributeData AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
		FGameplayAttributeData CastSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		FGameplayAttributeData MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FGameplayAttributeData PhysicalDamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FGameplayAttributeData MagicalDamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor)
		FGameplayAttributeData PhysicalArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor)
		FGameplayAttributeData MagicalArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
		FGameplayAttributeData Poise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
		FGameplayAttributeData MaxPoise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
		FGameplayAttributeData FireResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
		FGameplayAttributeData FrostResistance;

	//
	// No armor/resistance to poison/holy/chaos
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FGameplayAttributeData FireDamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FGameplayAttributeData FrostDamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FGameplayAttributeData HolyDamageModifier;
	//
	// chaos and poison can't be enhanced with modifiers, but tags may add more/less dmg
	//

public:
	//
	// Helper Funcs
	//
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CastSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PhysicalDamageModifier);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MagicalDamageModifier);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PhysicalArmor);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MagicalArmor);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Poise);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxPoise);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FireResistance);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FrostResistance);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FireDamageModifier);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FrostDamageModifier);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HolyDamageModifier);
	//
	// Consts
	//
	const float MaxVal = ATT_MAX_VAL;

	// TODO: React to GameplayEvent

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
