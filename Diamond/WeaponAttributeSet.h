// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.h"
#include "WeaponAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWeaponAttributeSet();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData LeftWeaponPhysicalDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData LeftWeaponMagicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData LeftWeaponFireDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData LeftWeaponFrostDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData LeftWeaponHolyDamage;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData RightWeaponPhysicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData RightWeaponMagicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData RightWeaponFireDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData RightWeaponFrostDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FGameplayAttributeData RightWeaponHolyDamage;


public:
	//
	// Helper Funcs
	//
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, LeftWeaponPhysicalDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, LeftWeaponMagicalDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, LeftWeaponFireDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, LeftWeaponFrostDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, LeftWeaponHolyDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, RightWeaponPhysicalDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, RightWeaponMagicalDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, RightWeaponFireDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, RightWeaponFrostDamage);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, RightWeaponHolyDamage);

	// TODO: React to GameplayEvent

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
