// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DiamondObjectAndStuff.h"
#include "DiamondBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UDiamondBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

	//
	// Effect Context helper functions
	//
public:
	UFUNCTION(BlueprintCallable, Category = Context)
	static void SetContextValues(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool isWeapon, bool isSpell, bool isLeftWeapon);

	UFUNCTION(BlueprintCallable, Category = Context)
	static bool IsWeaponEffect(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = Context)
	static bool IsLeftWeaponEffect(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = Context)
	static bool IsSpellEffect(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = Context)
	static void SetWeaponEffect(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool Value);

	UFUNCTION(BlueprintCallable, Category = Context)
	static void SetLeftWeaponEffect(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool Value);

	UFUNCTION(BlueprintCallable, Category = Context)
	static void SetSpellEffect(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool Value);

	UFUNCTION(BlueprintCallable, Category = Context)
	static void SetContextValuesForHeals(UPARAM(ref) FGameplayEffectContextHandle& EffectContext);

	//
	// Basic Damage Effects
	//

	/*
	Apply Single Damage Type from Source to Target, modifier = 1.f will be applied
	* 
	*/
	UFUNCTION(BlueprintCallable, Category = Damage)
	static void ApplySingleDamageTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, const FGameplayTag DamageTag, float Value);

	/*
	Get Spec Handle for Damage Effect
	*/
	UFUNCTION(BlueprintCallable, Category = Context)
	static FGameplayEffectSpecHandle GetDamageEffectSpecHandle(class UAbilitySystemComponent* Source, bool isWeapon, bool isLeftWeapon, bool isSpell);

	/*
	Apply 1 or more Damage types from Source to Target, modifier = 1.f will be applied 
	!!! No weapon damage
	*/
	UFUNCTION(BlueprintCallable, Category = Damage)
	static void ApplyDamageTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, const TMap< FGameplayTag, float>& DamageMap);

	/*
	Apply Weapon Damage from Source to Target, with optional damage for any type
	!!! Not for Spells, Use diffrent function for Damage done with weapons that cast spells
	*/
	UFUNCTION(BlueprintCallable, Category = Damage)
	static void ApplyWeaponDamageWithOptionalTo(class UAbilitySystemComponent* Source, bool isLeftWeapon, class UAbilitySystemComponent* Target, const TMap< FGameplayTag, float>& DamageMap);

	/*
	Apply Weapon Damage from Source to Target
	!!! Not for Spells, Use diffrent function for Damage done with weapons that cast spells
	*/
	UFUNCTION(BlueprintCallable, Category = Damage)
	static void ApplyWeaponDamageTo(class UAbilitySystemComponent* Source, bool isLeftWeapon, class UAbilitySystemComponent* Target);

	/*
	Apply Heal to Target, with Source
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyHealTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, float Heal);

	/*
	Apply Heal to Target, with Source
	ShouldDouble If true will double
	ShouldDoubleIfWeak If True will double only if character has tag Character.IsWeak
	ReductionMod, multiply final result with mod (use to decrease any healing effect applied)
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyHealWithOptionalTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, float Heal, bool ShouldDouble = false, bool ShouldDoubleIfWeak = false, float ReductionMod = 1.f);

	/*
	Get Spes, apply elsewhere
	ShouldDouble If true will double
	ShouldDoubleIfWeak If True will double only if character has tag Character.IsWeak
	ShouldUseHolyMod will use HolyDamageModifier and weapons Holy Damage IF Context is set to use weapons
	ReductionMod, multiply final result with mod (use to decrease any healing effect applied)
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static FGameplayEffectSpecHandle GetSpecForHealEffect(class UAbilitySystemComponent* Source, float Heal, bool ShouldDouble = false, bool ShouldDoubleIfWeak = false, bool ShouldUseHolyMod = false, float ReductionMod = 1.f);
	
	/*
	Apply Heal to Target, with Source (Context sets weapons and spell to true) SET WHICH WEAPON (for holy dmg stats)
	ShouldDouble If true will double
	ShouldDoubleIfWeak If True will double only if character has tag Character.IsWeak
	ShouldUseHolyMod is TRUE always
	ReductionMod, multiply final result with mod (use to decrease any healing effect applied)
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyHolyHealTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, float Heal, bool IsLeftWeapon, bool ShouldDouble = false, bool ShouldDoubleIfWeak = false, float ReductionMod = 1.f);

	/*
	Apply Heal buff to target 6seconds, every second, usually Source = Target
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyShortPeriodicHealTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, float Heal);

	/*
	Apply Heal to Target, with Source (Context sets weapons and spell to true) SET WHICH WEAPON (for holy dmg stats)
	6seconds, every second
	ShouldDouble If true will double
	ShouldDoubleIfWeak If True will double only if character has tag Character.IsWeak
	ShouldUseHolyMod is TRUE always
	ReductionMod, multiply final result with mod (use to decrease any healing effect applied)
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyShortPeriodicHolyHealTo(class UAbilitySystemComponent* Source, class UAbilitySystemComponent* Target, float Heal, bool IsLeftWeapon, bool ShouldDouble = false, bool ShouldDoubleIfWeak = false, float ReductionMod = 1.f);

	/*
	Get Spes, apply elsewhere
	ShouldDouble If true will double
	ShouldDoubleIfWeak If True will double only if character has tag Character.IsWeak
	ShouldUseHolyMod will use HolyDamageModifier and weapons Holy Damage IF Context is set to use weapons
	ReductionMod, multiply final result with mod (use to decrease any healing effect applied)
	*/
	UFUNCTION(BlueprintCallable, Category = Heal)
	static FGameplayEffectSpecHandle GetSpecForShortPeriodicHealEffect(class UAbilitySystemComponent* Source, float Heal, bool ShouldDouble = false, bool ShouldDoubleIfWeak = false, bool ShouldUseHolyMod = false, float ReductionMod = 1.f);

	//
	// Add Damage/Heal with tags to EffectSpecs
	//

	UFUNCTION(BlueprintCallable, Category = Damage)
	static void AddSingleDamageToSpec(UPARAM(ref) FGameplayEffectSpecHandle& SpecHandle, const FGameplayTag DamageTag, float Value);

	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyHealToSelf(class UAbilitySystemComponent* Source, float Heal);

	UFUNCTION(BlueprintCallable, Category = Heal)
	static void ApplyShortPeriodicHealToSelf(class UAbilitySystemComponent* Source, float Heal);

	//
	// HP effects
	//

	// To Reduce is positive float
	UFUNCTION(BlueprintCallable, Category = Health)
	static void ReduceHealth(class UAbilitySystemComponent* Source, float ToReduce);

	//
	// Mana Effects
	//

	// To Reduce is positive float
	UFUNCTION(BlueprintCallable, Category = Health)
	static void ReduceMana(class UAbilitySystemComponent* Source, float ToReduce);

	//
	// Stamina Effects
	//
	
	// To Reduce is positive float
	UFUNCTION(BlueprintCallable, Category = Stamina)
	static void ReduceStamina(class UAbilitySystemComponent* Source, float ToReduce);

	//
	// Poise Effects
	//

	// To Reduce is positive float
	UFUNCTION(BlueprintCallable, Category = Poise)
	static void ReducePoise(class UAbilitySystemComponent* Source, float ToReduce);

	//
	// Weapon Equip
	//
	
	// Spawn Weapon (Must be one of Weapon Classes, !!! Check wheter IsLeft should be true or false)
	// After Spawning attach it to Character and call events (OnEquip events)
	UFUNCTION(BlueprintCallable, Category = Weapon, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use SpawnAndEquipWeapon"))
	static void SpawnAndEquipWeaponFromClass(TSubclassOf<class AMeleeWeapon> WeaponClass, class ADiamondCharacter* WeaponOwner, bool ShouldUpdateWeaponAttributes, bool IsLeft);

	// Spawn Weapon (Must be correctly set up DataAsset, !!! Check wheter IsLeft should be true or false BEFORE CALLING)
	// After Spawning attach it to Character and call events (OnEquip events)
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void SpawnAndEquipWeapon(class UWeaponItemDataAsset* Data, class ADiamondCharacter* Owner, bool IsLeft);

	// Destroys Weapon and Calls events, Wreapon Ptr is Null at end
	// IsLeft -> Left Weapon if true or Right Weapon if false
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void UnequipAndDestroyWeapon(class ADiamondCharacter* WeaponOwner, bool IsLeft);

	// MUST BE CALLED IF EFFECTS SHOULD BE REMOVED WHEN WEAPON IS REMOVED, Is Left is set in Weapon
	// ADDS dynamic Tag Effect.IsWeapon . Left / Right
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void AddWeaponEffect(class UAbilitySystemComponent* Source, TSubclassOf<class UGameplayEffect> EffectClass, bool IsLeft);

	// Remove Buffs from Weapon
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void RemoveWeaponEffects(class UAbilitySystemComponent* Target, bool IsLeft);

	//
	// Using Weapons
	//

	// Apply weapon Damage and call Source's and Target's events
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void HitCharacterWithWeapon(class ADiamondCharacter* SourceCharacter, class AMeleeWeapon* Weapon, class ADiamondCharacter* TargetCharacter, float PoiseDamage, const FHitResult& HitResult);

	// Call to trigger Events, not 'Block' some attack
	// MeleeWeapon should hit collision and call Shields OnHit Event and call this function
	// Source is Character with Shield
	// Target is Character with Melee Weapon (Or something else)
	// Calls Source's and Target's events
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void BlockCharacterAttackEvents(class ADiamondCharacter* SourceCharacter, class AShieldWeapon* Shield, class ADiamondCharacter* TargetCharacter, class AWeapon* AtkWeapon, float PoiseDamage, const FHitResult& HitResult);

	// Call to trigger Events, not 'Parry' some attack
	// MeleeWeapon should hit collision and call Shields OnHit Event and call this function
	// Source is Character with Shield
	// Target is Character with Melee Weapon
	// Calls Source's and Target's events
	UFUNCTION(BlueprintCallable, Category = Weapon)
	static void ParryCharacterAttackEvents(class ADiamondCharacter* SourceCharacter, class AShieldWeapon* Shield, class ADiamondCharacter* TargetCharacter, class AWeapon* AtkWeapon, float PoiseDamage, const FHitResult& HitResult);
};
