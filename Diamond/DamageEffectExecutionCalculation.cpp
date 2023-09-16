// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffectExecutionCalculation.h"
#include "DiamondAbilitySystemComponent.h"
#include "CharacterAttributeSet.h"
#include "WeaponAttributeSet.h"
#include "DiamondObjectAndStuff.h"

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); // target
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalArmor); // target
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor); // target
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance); // target
	DECLARE_ATTRIBUTE_CAPTUREDEF(FrostResistance); // target
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamageModifier); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalDamageModifier); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireDamageModifier); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(FrostDamageModifier); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(HolyDamageModifier); // source

	// source's weapons
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponPhysicalDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponMagicalDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponFireDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponFrostDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(LeftWeaponHolyDamage); // source
	//
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponPhysicalDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponMagicalDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponFireDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponFrostDamage); // source
	DECLARE_ATTRIBUTE_CAPTUREDEF(RightWeaponHolyDamage); // source

	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, PhysicalDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, MagicalDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FrostDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, HolyDamageModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponPhysicalDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponMagicalDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponFireDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponFrostDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, LeftWeaponHolyDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponPhysicalDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponMagicalDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponFireDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponFrostDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, RightWeaponHolyDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, PhysicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, MagicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FrostResistance, Target, false);
	}
};

static const DamageStatics& GetDamageStatics()
{
	static DamageStatics Statics;
	return Statics;
}

UDamageEffectExecutionCalculation::UDamageEffectExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FrostResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().PhysicalDamageModifierDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalDamageModifierDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireDamageModifierDef);
	RelevantAttributesToCapture.Add(DamageStatics().FrostDamageModifierDef);
	RelevantAttributesToCapture.Add(DamageStatics().HolyDamageModifierDef);

	RelevantAttributesToCapture.Add(DamageStatics().LeftWeaponPhysicalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().LeftWeaponMagicalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().LeftWeaponFireDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().LeftWeaponFrostDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().LeftWeaponHolyDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().RightWeaponPhysicalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().RightWeaponMagicalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().RightWeaponFireDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().RightWeaponFrostDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().RightWeaponHolyDamageDef);
}

// no tags required calc dmg
float CalcPhysicalDamage(float pDmg, float pDmgMod, float pArmor)
{
	return FMath::Clamp(pDmg * pDmgMod - pArmor, 0.f, ATT_MAX_VAL);
}

#define WEAK_TO_MAGIC 1.3f
float CalcMagicalDamage(float mDmg, float mDmgMod, float mArmor, bool isWeakToMagic)
{
	return FMath::Clamp((isWeakToMagic ? mDmg * mDmgMod * WEAK_TO_MAGIC : mDmg * mDmgMod) - mArmor, 0.f, ATT_MAX_VAL);
}

#define WEAK_TO_HOLY 2.5f
float CalcHolyDamage(float hDmg, float hDmgMod, bool isWeakToHoly)
{
	return FMath::Clamp(isWeakToHoly ? hDmg * hDmgMod * WEAK_TO_HOLY : hDmg * hDmgMod, 0.f, ATT_MAX_VAL);
}

#define WEAK_TO_FIRE 1.5f
float CalcFireDamage(float fDmg, float fDmgMod, float fRes, bool isWeakToFire)
{
	if ((isWeakToFire ? fDmg * fDmgMod * WEAK_TO_FIRE : fDmg * fDmgMod) > fRes)
	{
		// apply fire effect
	}
	return 0.f; // only debuff dmg is applied
}

// NOT IMPLEMENTED
float CalcFrostDamage()
{
	return 0.f;
}

#define CHAOS_DMG_MIN_MULT 1.f
#define CHAOS_DMG_MAX_MULT 10.f
float CalcChaosDamage(float dmg)
{
	return FMath::RandRange(CHAOS_DMG_MIN_MULT, CHAOS_DMG_MAX_MULT) * dmg;
}

// NOT IMPLEMENTED ? NO NEED 
float CalcPoisonDamage()
{
	return 0.f;
}

#define GetTag(name) FGameplayTag::RequestGameplayTag(FName(name))

void UDamageEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	UE_LOG(LogTemp, Warning, TEXT("START Effect Calc Dmg"));

#define CAPTURE_CUSTOM(name, defName) float name = 0.f; \
ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics(). ## defName ## Def, EvaluationParameters, name);\
name = FMath::Max<float>(name, 0.0f);

	CAPTURE_CUSTOM(Health, Health);
	CAPTURE_CUSTOM(ArmorP, PhysicalArmor);
	CAPTURE_CUSTOM(ArmorM, MagicalArmor);
	CAPTURE_CUSTOM(FireRes, FireResistance);
	CAPTURE_CUSTOM(FrostRes, FrostResistance);

	CAPTURE_CUSTOM(DamageModP, PhysicalDamageModifier);
	CAPTURE_CUSTOM(DamageModM, MagicalDamageModifier);
	CAPTURE_CUSTOM(DamageModFire, HolyDamageModifier);
	CAPTURE_CUSTOM(DamageModFrost, FireDamageModifier);
	CAPTURE_CUSTOM(DamageModH, FrostDamageModifier);

	float fire_dmg, frost_dmg, holy_dmg, mag_dmg, phys_dmg, chaos_dmg;

#define CAPTURE_DMG(name) Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(name)), false, 0.f)
	// dmg = CAPTURE_DMG("Damage");
	fire_dmg = CAPTURE_DMG("Damage.Fire");
	frost_dmg = CAPTURE_DMG("Damage.Frost");
	holy_dmg = CAPTURE_DMG("Damage.Holy");
	mag_dmg = CAPTURE_DMG("Damage.Magical");
	phys_dmg = CAPTURE_DMG("Damage.Physical");
	chaos_dmg = CAPTURE_DMG("Damage.Chaos");
	// poison_dmg = CAPTURE_DMG("Damage.Poison");

	static FGameplayTag WeakToMagic = GetTag("Character.IsWeak.ToMagic");
	static FGameplayTag ImuneToHoly = GetTag("Character.IsWeak.ToMagic");
	static FGameplayTag WeakToHoly1 = GetTag("Character.IsWeak.ToHoly");
	static FGameplayTag Undead = GetTag("Undead");
	static FGameplayTag ImuneToFire = GetTag("Character.IsImune.ToFire");
	static FGameplayTag WeakToFire = GetTag("Character.IsWeak.ToFire");
	static FGameplayTag HpImune = GetTag("Character.IsImune.Health");

	bool isWeakToMagic = TargetTags->HasTagExact(WeakToMagic);
	bool isImuneToHoly = TargetTags->HasTagExact(ImuneToHoly);
	bool isWeakToHoly = TargetTags->HasTagExact(WeakToHoly1) || TargetTags->HasTagExact(Undead);
	bool isImuneToFire = TargetTags->HasTagExact(ImuneToFire);
	bool isWeakToFire = TargetTags->HasTagExact(WeakToFire);

	// NEEDS TO BE HERE
	if (TargetTags->HasTagExact(HpImune)) return;

	if (Context->bIsWeapon && !Context->bIsSpell) // hit by weapon (not bow arrow) and not with spell
	{
		CAPTURE_CUSTOM(LWeaponP, LeftWeaponPhysicalDamage);
		CAPTURE_CUSTOM(LWeaponM, LeftWeaponMagicalDamage);
		CAPTURE_CUSTOM(LWeaponFire, LeftWeaponFireDamage);
		CAPTURE_CUSTOM(LWeaponFrost, LeftWeaponFrostDamage);
		CAPTURE_CUSTOM(LWeaponH, LeftWeaponHolyDamage);

		CAPTURE_CUSTOM(RWeaponP, RightWeaponPhysicalDamage);
		CAPTURE_CUSTOM(RWeaponM, RightWeaponMagicalDamage);
		CAPTURE_CUSTOM(RWeaponFire, RightWeaponFireDamage);
		CAPTURE_CUSTOM(RWeaponFrost, RightWeaponFrostDamage);
		CAPTURE_CUSTOM(RWeaponH, RightWeaponHolyDamage);

		if (Context->bIsLeftWeapon)
		{
			//UE_LOG(LogTemp, Warning, TEXT("AAA, %f %f"), phys_dmg, DamageModP);

			float dmgToDeal =
				CalcPhysicalDamage(LWeaponP + phys_dmg, DamageModP, ArmorP) +
				CalcMagicalDamage(LWeaponM + mag_dmg, DamageModM, ArmorM, isWeakToMagic) +
				(isImuneToHoly ? 0.f : CalcHolyDamage(LWeaponH + holy_dmg, DamageModH, isWeakToHoly)) +
				(isImuneToFire ? 0.f : CalcFireDamage(LWeaponFire + fire_dmg, DamageModFire, FireRes, isWeakToFire)) +
				// frost
				CalcChaosDamage(chaos_dmg);
			
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().HealthProperty,
				EGameplayModOp::Additive, -dmgToDeal));

		}
		else
		{
			float dmgToDeal =
				CalcPhysicalDamage(RWeaponP + phys_dmg, DamageModP, ArmorP) +
				CalcMagicalDamage(RWeaponM + mag_dmg, DamageModM, ArmorM, isWeakToMagic) +
				(isImuneToHoly ? 0.f : CalcHolyDamage(RWeaponH + holy_dmg, DamageModH, isWeakToHoly)) +
				(isImuneToFire ? 0.f : CalcFireDamage(RWeaponFire + fire_dmg, DamageModFire, FireRes, isWeakToFire)) +
				// frost
				CalcChaosDamage(chaos_dmg);

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().HealthProperty,
				EGameplayModOp::Additive, -dmgToDeal));

		}
	}
	else if (Context->bIsSpell) // hit by spell, also check if weapon was used (staff empowered spell)
	{
	}
	else // Rare call (there will always be weapons/skill snapshoting attributes at creation)
	{
		float dmgToDeal =
			CalcPhysicalDamage(phys_dmg, 1.f, ArmorP) +
			CalcMagicalDamage(mag_dmg, 1.f, ArmorM, isWeakToMagic) +
			(isImuneToHoly ? 0.f : CalcHolyDamage(holy_dmg, 1.f, isWeakToHoly)) +
			(isImuneToFire ? 0.f : CalcFireDamage(fire_dmg, 1.f, FireRes, isWeakToFire)) +
			// frost
			CalcChaosDamage(chaos_dmg);

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().HealthProperty,
			EGameplayModOp::Additive, -dmgToDeal));
	}
}