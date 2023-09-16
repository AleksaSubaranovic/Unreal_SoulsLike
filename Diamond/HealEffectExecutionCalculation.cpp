// Fill out your copyright notice in the Description page of Project Settings.


#include "HealEffectExecutionCalculation.h"
#include "DiamondAbilitySystemComponent.h"
#include "CharacterAttributeSet.h"
#include "WeaponAttributeSet.h"
#include "DiamondObjectAndStuff.h"

struct HealStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); // target
	DECLARE_ATTRIBUTE_CAPTUREDEF(HolyDamageModifier); // source (for holy heal as holy modifier)

	// source's weapons
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponHolyDamage); // source
	//
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponHolyDamage); // source

	HealStatics()
	{

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, HolyDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponHolyDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponHolyDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);
	}
};

static const HealStatics& GetHealStatics()
{
	static HealStatics Statics;
	return Statics;
}

UHealEffectExecutionCalculation::UHealEffectExecutionCalculation()
{
	RelevantAttributesToCapture.Add(HealStatics().HealthDef);
	RelevantAttributesToCapture.Add(HealStatics().HolyDamageModifierDef);
	RelevantAttributesToCapture.Add(HealStatics().LeftWeaponHolyDamageDef);
	RelevantAttributesToCapture.Add(HealStatics().RightWeaponHolyDamageDef);
}

float CalcHeal(float val, float mod, bool shouldDouble)
{
	return shouldDouble ? val * 2.f * mod : val * mod;
}

// based on holy dmg of weapon chose heal (Should never be 1-1 change)
// example 100holy dmg with 1.f mod is 100 hp taken, should be less in heals and only if spell was used
#define HOLY_DMG_CONVERT_TO_HP 0.25f
float CalcHealOfWeapon(float val)
{
	return val * HOLY_DMG_CONVERT_TO_HP;
}
#define HOLY_CONVERT_TO_HP_UNDEAD(val) -4.f * val 

#define GetTag(name) FGameplayTag::RequestGameplayTag(FName(name))

void UHealEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent(); //We put AbilitySystemComponents into little helper variables. Not necessary, but it helps keeping us from typing so much.

	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr; //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr; //a null pointer.

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); //Some more helper variables: Spec is the spec this execution originated from, and the Source/TargetTags are pointers to the tags granted to source/target actor, respectively.

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(Spec.GetContext().Get());
	// SHOULD ALWAYS BE VALID
	
	if (!Context->bIsHeal) return; // not marked as heal, therefore ignore
	{
		UE_LOG(LogTemp, Warning, TEXT("START Effect Calc Heal FAILED, not marked as heal"));
	}

	UE_LOG(LogTemp, Warning, TEXT("START Effect Calc Heal"));

#define CAPTURE_CUSTOM(name, defName) float name = 0.f; \
ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics(). ## defName ## Def, EvaluationParameters, name);\
name = FMath::Max<float>(name, 0.0f);

	CAPTURE_CUSTOM(Health, Health);
	CAPTURE_CUSTOM(HolyMod, HolyDamageModifier);

	static FGameplayTag Undead = GetTag("Undead");
	static FGameplayTag Hurt = GetTag("Character.isHurt");

	bool isUndead = TargetTags->HasTagExact(Undead);
	bool isHurt = TargetTags->HasTagExact(Hurt);
	// Can Heal checked onGoing

#define CAPTURE_HEAL(name, defVal) Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(name)), false, defVal)

	bool shouldDoubleHeal = CAPTURE_HEAL("Heal.Double", 0.0f) || (isHurt && CAPTURE_HEAL("Heal.DoubleIfWeak", 0.0f));
	bool useHolyMod = (bool) CAPTURE_HEAL("Heal.Holy", 0.0f);
	float healRed = CAPTURE_HEAL("Heal.Reduced", 1.f); // multiply with this at end
	float healVal = CAPTURE_HEAL("Heal.Value", 0.f); // heal value

	if (Context->bIsWeapon && useHolyMod) // usually called, if no weapon then use without weapons, spell or skill (same modifier)
	{
		CAPTURE_CUSTOM(HolyDmgL, LeftWeaponHolyDamage);
		CAPTURE_CUSTOM(HolyDmgR, RightWeaponHolyDamage);

		if (Context->bIsLeftWeapon)
		{
			float hpToAdd = CalcHeal(healVal + CalcHealOfWeapon(HolyDmgL), HolyMod * healRed, shouldDoubleHeal);

			if (isUndead && useHolyMod)
				hpToAdd = HOLY_CONVERT_TO_HP_UNDEAD(hpToAdd);



			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetHealStatics().HealthProperty,
				EGameplayModOp::Additive, hpToAdd));
		}
		else
		{
			float hpToAdd = CalcHeal(healVal + CalcHealOfWeapon(HolyDmgR), HolyMod * healRed, shouldDoubleHeal);

			if (isUndead && useHolyMod)
				hpToAdd = HOLY_CONVERT_TO_HP_UNDEAD(hpToAdd);

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetHealStatics().HealthProperty,
				EGameplayModOp::Additive, hpToAdd));
		}
	}
	else // heal of unknow origin, uses modifier if specified (usually not)
	{
		float hpToAdd = useHolyMod ? CalcHeal(healVal, HolyMod * healRed, shouldDoubleHeal) :
			CalcHeal(healVal, healRed, shouldDoubleHeal);

		if (isUndead && useHolyMod)
			hpToAdd = HOLY_CONVERT_TO_HP_UNDEAD(hpToAdd);

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetHealStatics().HealthProperty,
			EGameplayModOp::Additive, hpToAdd));
	}
}
