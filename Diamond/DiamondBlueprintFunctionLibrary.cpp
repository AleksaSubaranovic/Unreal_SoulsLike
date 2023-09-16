// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondBlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "DamageEffectExecutionCalculation.h"
#include "Effects/DamageEffect.h"
#include "HealEffectExecutionCalculation.h"
#include "Effects/HealInstantEffect.h"
#include "Effects/HealShortPeriodEffect.h"
#include "Weapons/MeleeWeapon.h"
#include "Weapons/ShieldWeapon.h"
#include "DataAssets/WeaponItemDataAsset.h"
#include "DiamondCharacter.h"
#include "WeaponAttributeSet.h"
#include "DiamondAbilitySystemComponent.h"
#include "DataAssets/ShieldItemDataAsset.h"
#include "DiamondObjectAndStuff.h"

void UDiamondBlueprintFunctionLibrary::SetContextValues(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool isWeapon, bool isSpell, bool isLeftWeapon)
{
	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		Context->bIsWeapon = isWeapon;
		Context->bIsSpell = isSpell;
		Context->bIsLeftWeapon = isLeftWeapon;
	}
}

bool UDiamondBlueprintFunctionLibrary::IsWeaponEffect(const FGameplayEffectContextHandle& EffectContext)
{
	const FDiamondGameplayEffectContext* Context = static_cast<const FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		return Context->bIsWeapon;
	}
	return false;
}

bool UDiamondBlueprintFunctionLibrary::IsLeftWeaponEffect(const FGameplayEffectContextHandle& EffectContext)
{
	const FDiamondGameplayEffectContext* Context = static_cast<const FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		return Context->bIsLeftWeapon;
	}
	return false;
}

bool UDiamondBlueprintFunctionLibrary::IsSpellEffect(const FGameplayEffectContextHandle& EffectContext)
{
	const FDiamondGameplayEffectContext* Context = static_cast<const FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		return Context->bIsSpell;
	}
	return false;
}

void UDiamondBlueprintFunctionLibrary::SetWeaponEffect(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, bool Value)
{
	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		Context->bIsWeapon = Value;
	}
}

void UDiamondBlueprintFunctionLibrary::SetLeftWeaponEffect(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, bool Value)
{
	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		Context->bIsLeftWeapon = Value;
	}
}

void UDiamondBlueprintFunctionLibrary::SetSpellEffect(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, bool Value)
{
	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		Context->bIsSpell = Value;
	}
}

void UDiamondBlueprintFunctionLibrary::SetContextValuesForHeals(UPARAM(ref)FGameplayEffectContextHandle& EffectContext)
{
	FDiamondGameplayEffectContext* Context = static_cast<FDiamondGameplayEffectContext*>(EffectContext.Get());
	if (Context)
	{
		Context->bIsHeal = true;
	}
}

#define GetTag(name) FGameplayTag::RequestGameplayTag(FName(name))

void UDiamondBlueprintFunctionLibrary::ApplySingleDamageTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, const FGameplayTag DamageTag, float Value)
{
	if (Source == nullptr || Target == nullptr) return;

	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UDamageEffect::StaticClass(), 0.f, Source->MakeEffectContext());
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageTag, Value);
	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

FGameplayEffectSpecHandle UDiamondBlueprintFunctionLibrary::GetDamageEffectSpecHandle(UAbilitySystemComponent* Source, bool isWeapon, bool isLeftWeapon, bool isSpell)
{
	if (Source == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("GetDamageEffectSpecHandle is broken")));
		return FGameplayEffectSpecHandle();
	}
	auto Context = Source->MakeEffectContext();
	SetContextValues(Context, isWeapon, isSpell, isLeftWeapon);
	return Source->MakeOutgoingSpec(UDamageEffect::StaticClass(), 0.f, Context);
}

void UDiamondBlueprintFunctionLibrary::ApplyDamageTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, const TMap<FGameplayTag, float>& DamageMap)
{
	if (Source == nullptr || Target == nullptr) return;

	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UDamageEffect::StaticClass(), 0.f, Source->MakeEffectContext());
	
	for (auto& element : DamageMap)
	{
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(element.Key, element.Value);
	}

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyWeaponDamageWithOptionalTo(UAbilitySystemComponent* Source, bool isLeftWeapon, UAbilitySystemComponent* Target, const TMap<FGameplayTag, float>& DamageMap)
{
	if (Source == nullptr || Target == nullptr) return;

	FGameplayEffectSpecHandle SpecHandle = GetDamageEffectSpecHandle(Source, true, isLeftWeapon, false);

	for (auto& element : DamageMap)
	{
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(element.Key, element.Value);
	}

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyWeaponDamageTo(UAbilitySystemComponent* Source, bool isLeftWeapon, UAbilitySystemComponent* Target)
{
	if (Source == nullptr || Target == nullptr) return;

	FGameplayEffectSpecHandle SpecHandle = GetDamageEffectSpecHandle(Source, true, isLeftWeapon, false);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyHealTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Heal)
{
	if (Source == nullptr || Target == nullptr) return;

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealInstantEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyHealWithOptionalTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Heal, bool ShouldDouble, bool ShouldDoubleIfWeak, float ReductionMod)
{
	if (Source == nullptr || Target == nullptr) return;

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealInstantEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Double"), ShouldDouble);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.DoubleIfWeak"), ShouldDoubleIfWeak);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Reduced"), ReductionMod);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

FGameplayEffectSpecHandle UDiamondBlueprintFunctionLibrary::GetSpecForHealEffect(UAbilitySystemComponent* Source, float Heal, bool ShouldDouble, bool ShouldDoubleIfWeak, bool ShouldUseHolyMod, float ReductionMod)
{
	if (Source == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("GetHealEffectSpecHandle is broken")));
		return FGameplayEffectSpecHandle();
	}

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealInstantEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Double"), ShouldDouble);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.DoubleIfWeak"), ShouldDoubleIfWeak);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Reduced"), ReductionMod);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Holy"), ShouldUseHolyMod);

	return SpecHandle;
}

void UDiamondBlueprintFunctionLibrary::ApplyHolyHealTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Heal, bool IsLeftWeapon, bool ShouldDouble, bool ShouldDoubleIfWeak, float ReductionMod)
{
	if (Source == nullptr || Target == nullptr) return;

	auto Context = Source->MakeEffectContext();
	auto DiamondContext = static_cast<FDiamondGameplayEffectContext*>(Context.Get());
	DiamondContext->bIsWeapon = DiamondContext->bIsSpell = DiamondContext->bIsHeal = true;
	DiamondContext->bIsLeftWeapon = IsLeftWeapon;

	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealInstantEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Double"), ShouldDouble);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.DoubleIfWeak"), ShouldDoubleIfWeak);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Reduced"), ReductionMod);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Holy"), 1.f);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyShortPeriodicHealTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Heal)
{
	if (Source == nullptr || Target == nullptr) return;

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealShortPeriodEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void UDiamondBlueprintFunctionLibrary::ApplyShortPeriodicHolyHealTo(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, float Heal, bool IsLeftWeapon, bool ShouldDouble, bool ShouldDoubleIfWeak, float ReductionMod)
{
	if (Source == nullptr || Target == nullptr) return;

	auto Context = Source->MakeEffectContext();
	auto DiamondContext = static_cast<FDiamondGameplayEffectContext*>(Context.Get());
	DiamondContext->bIsWeapon = DiamondContext->bIsSpell = DiamondContext->bIsHeal = true;
	DiamondContext->bIsLeftWeapon = IsLeftWeapon;

	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealShortPeriodEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Double"), ShouldDouble);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.DoubleIfWeak"), ShouldDoubleIfWeak);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Reduced"), ReductionMod);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Holy"), 1.f);

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

FGameplayEffectSpecHandle UDiamondBlueprintFunctionLibrary::GetSpecForShortPeriodicHealEffect(UAbilitySystemComponent* Source, float Heal, bool ShouldDouble, bool ShouldDoubleIfWeak, bool ShouldUseHolyMod, float ReductionMod)
{
	if (Source == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("GetHeal Short Periodic EffectSpecHandle is broken")));
		return FGameplayEffectSpecHandle();
	}

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealShortPeriodEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Double"), ShouldDouble);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.DoubleIfWeak"), ShouldDoubleIfWeak);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Reduced"), ReductionMod);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Holy"), ShouldUseHolyMod);

	return SpecHandle;
}

void UDiamondBlueprintFunctionLibrary::AddSingleDamageToSpec(UPARAM(ref) FGameplayEffectSpecHandle& SpecHandle, const FGameplayTag DamageTag, float Value)
{
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageTag, Value);
}

void UDiamondBlueprintFunctionLibrary::ApplyHealToSelf(UAbilitySystemComponent* Source, float Heal)
{
	if (Source == nullptr) return;

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealInstantEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	Source->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UDiamondBlueprintFunctionLibrary::ApplyShortPeriodicHealToSelf(UAbilitySystemComponent* Source, float Heal)
{
	if (Source == nullptr) return;

	auto Context = Source->MakeEffectContext();
	(static_cast<FDiamondGameplayEffectContext*>(Context.Get()))->bIsHeal = true;
	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(UHealShortPeriodEffect::StaticClass(), 0.f, Context);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(GetTag("Heal.Value"), Heal);

	Source->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UDiamondBlueprintFunctionLibrary::ReduceHealth(UAbilitySystemComponent* Source, float ToReduce)
{
	static FGameplayTag ImuneHp = FGameplayTag::RequestGameplayTag(FName("Character.IsImune.Health"));
	if (Source->HasMatchingGameplayTag(ImuneHp)) return;

	UGameplayEffect* Eff = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Hp Reduction")));
	Eff->DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo HpModifier;
	HpModifier.Attribute = UCharacterAttributeSet::GetHealthAttribute();
	HpModifier.ModifierOp = EGameplayModOp::Additive;
	Eff->Modifiers.Add(HpModifier);
	Eff->Modifiers[0].ModifierMagnitude = FScalableFloat(ToReduce * -1.f);

	Source->ApplyGameplayEffectToSelf(Eff, 0.f, Source->MakeEffectContext());
}

void UDiamondBlueprintFunctionLibrary::ReduceMana(UAbilitySystemComponent* Source, float ToReduce)
{
	static FGameplayTag ImuneMana = FGameplayTag::RequestGameplayTag(FName("Character.IsImune.Mana"));
	if (Source->HasMatchingGameplayTag(ImuneMana)) return;

	UGameplayEffect* Eff = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Mana Reduction")));
	Eff->DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo ManaModifier;
	ManaModifier.Attribute = UCharacterAttributeSet::GetManaAttribute();
	ManaModifier.ModifierOp = EGameplayModOp::Additive;
	Eff->Modifiers.Add(ManaModifier);
	Eff->Modifiers[0].ModifierMagnitude = FScalableFloat(ToReduce * -1.f);

	Source->ApplyGameplayEffectToSelf(Eff, 0.f, Source->MakeEffectContext());
}

void UDiamondBlueprintFunctionLibrary::ReduceStamina(UAbilitySystemComponent* Source, float ToReduce)
{
	static FGameplayTag ImuneStamina = FGameplayTag::RequestGameplayTag(FName("Character.IsImune.Stamina"));
	if (Source->HasMatchingGameplayTag(ImuneStamina)) return;

	UGameplayEffect* Eff = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Stamina Reduction")));
	Eff->DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifier.ModifierOp = EGameplayModOp::Additive;
	Eff->Modifiers.Add(StaminaModifier);
	Eff->Modifiers[0].ModifierMagnitude = FScalableFloat(ToReduce * -1.f);

	Source->ApplyGameplayEffectToSelf(Eff, 0.f, Source->MakeEffectContext());
}

void UDiamondBlueprintFunctionLibrary::ReducePoise(UAbilitySystemComponent* Source, float ToReduce)
{
	static FGameplayTag ImunePoise = FGameplayTag::RequestGameplayTag(FName("Character.HasHyperArmor"));
	if (Source->HasMatchingGameplayTag(ImunePoise)) return;

	UGameplayEffect* Eff = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Poise Reduction")));
	Eff->DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo PoiseModifier;
	PoiseModifier.Attribute = UCharacterAttributeSet::GetPoiseAttribute();
	PoiseModifier.ModifierOp = EGameplayModOp::Additive;
	Eff->Modifiers.Add(PoiseModifier);
	Eff->Modifiers[0].ModifierMagnitude = FScalableFloat(ToReduce * -1.f);

	Source->ApplyGameplayEffectToSelf(Eff, 0.f, Source->MakeEffectContext());
}

#define SET_WEAPON_ATT(l, dmgT) WeaponAttSet->Set ## l ## Weapon ## dmgT ## Damage(Weapon->WeaponDataAsset-> ## dmgT ## Damage)

void UDiamondBlueprintFunctionLibrary::SpawnAndEquipWeaponFromClass(TSubclassOf<class AMeleeWeapon> WeaponClass, ADiamondCharacter* WeaponOwner, bool ShouldUpdateWeaponAttributes, bool IsLeft)
{
	static FName LeftSocket = FName("LeftWeaponSocket");
	static FName RightSocket = FName("RightWeaponSocket");

	if (WeaponOwner != nullptr)
	{
		auto World = WeaponOwner->GetWorld();

		auto SocketTransform = WeaponOwner->GetMesh()->GetSocketTransform(IsLeft ? LeftSocket : RightSocket);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = WeaponOwner;

		AMeleeWeapon* Weapon = static_cast<AMeleeWeapon*>(World->SpawnActor(WeaponClass.Get(), &SocketTransform, SpawnInfo));

		// attach
		FAttachmentTransformRules AttachTransform(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		Weapon->AttachToActor(WeaponOwner, AttachTransform, IsLeft ? LeftSocket : RightSocket);

		Weapon->WeaponOwner = WeaponOwner; // same as Owner

		if (ShouldUpdateWeaponAttributes) // Change player attributes
		{
			auto WeaponAttSet = WeaponOwner->GetWeaponAttributeSet();
			if (IsLeft)
			{
				SET_WEAPON_ATT(Left, Physical);
				SET_WEAPON_ATT(Left, Magical);
				SET_WEAPON_ATT(Left, Holy);
				SET_WEAPON_ATT(Left, Fire);
				SET_WEAPON_ATT(Left, Frost);
			}
			else
			{
				SET_WEAPON_ATT(Right, Physical);
				SET_WEAPON_ATT(Right, Magical);
				SET_WEAPON_ATT(Right, Holy);
				SET_WEAPON_ATT(Right, Fire);
				SET_WEAPON_ATT(Right, Frost);
			}
		}
	}
	// TODO: ADD FOR OTHER WEAPON TYPES
}

void UDiamondBlueprintFunctionLibrary::SpawnAndEquipWeapon(UWeaponItemDataAsset* Data, ADiamondCharacter* Owner, bool IsLeft)
{
	static FName LeftSocket = FName("LeftWeaponSocket");
	static FName RightSocket = FName("RightWeaponSocket");
	if (Owner && Data && Data->WeaponMesh)
	{
		auto World = Owner->GetWorld();

		auto SocketTransform = Owner->GetMesh()->GetSocketTransform(IsLeft ? LeftSocket : RightSocket);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = Owner;

		// TODO:
		
		// spawn (dynamic creation of object)
		AWeapon* Weapon;
		AShieldWeapon* Shield;
		switch (Data->WeaponType)
		{
		case EWeaponType::Sword:
			Weapon = static_cast<AMeleeWeapon*>(World->SpawnActor(AMeleeWeapon::StaticClass(), &SocketTransform, SpawnInfo));
			break;
		case EWeaponType::Shield:
			Shield = Cast<AShieldWeapon>(World->SpawnActor(AShieldWeapon::StaticClass(), &SocketTransform, SpawnInfo));
			Shield->ShieldDataAsset = Cast<UShieldItemDataAsset>(Data);
			check(Shield->ShieldDataAsset);
			Weapon = Shield;
			break;
		default:
			Weapon = nullptr;
			Shield = nullptr;
		}
		if (Weapon == nullptr) return;

		Weapon->SetActorRotation(Data->RelativeRotation);

		// set static mesh
		Weapon->WeaponMesh->SetStaticMesh(Data->WeaponMesh);
		// set ptr to DataAsset
		Weapon->WeaponDataAsset = Data;
		// set ptr to Owner
		Weapon->WeaponOwner = Owner;
		// set is in left hand
		Weapon->bIsLeftWeapon = IsLeft;

		// attach
		FAttachmentTransformRules AttachTransform(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(Owner->GetMesh(), AttachTransform, IsLeft ? LeftSocket : RightSocket);

		// handle Weapon Attributes
		// remove all Weapon bound effects, add new effects after setting Attribute values

		UnequipAndDestroyWeapon(Owner, IsLeft); // CALLS Remove Effects

		auto WeaponAttSet = Owner->GetWeaponAttributeSet();
		if (IsLeft)
		{
			SET_WEAPON_ATT(Left, Physical);
			SET_WEAPON_ATT(Left, Magical);
			SET_WEAPON_ATT(Left, Holy);
			SET_WEAPON_ATT(Left, Fire);
			SET_WEAPON_ATT(Left, Frost);
		}
		else
		{
			SET_WEAPON_ATT(Right, Physical);
			SET_WEAPON_ATT(Right, Magical);
			SET_WEAPON_ATT(Right, Holy);
			SET_WEAPON_ATT(Right, Fire);
			SET_WEAPON_ATT(Right, Frost);
		}

		// Add weapon eff
		auto Eff = Data->GameplayEffects.Find(EWeaponEffectApplicationType::OnEquip);
		if (Eff) AddWeaponEffect(Owner->GetAbilitySystemComponent(), Eff->Get(), IsLeft);

		Owner->SetWeapon(Weapon, IsLeft);
	}
}

void UDiamondBlueprintFunctionLibrary::UnequipAndDestroyWeapon(ADiamondCharacter* WeaponOwner, bool IsLeft)
{
	if (WeaponOwner == nullptr) return;
	auto Weapon = WeaponOwner->GetWeapon(IsLeft);
	if (Weapon != nullptr)
	{
		RemoveWeaponEffects(WeaponOwner->GetAbilitySystemComponent(), IsLeft);

		Weapon->Destroy();
	}
}

void UDiamondBlueprintFunctionLibrary::AddWeaponEffect(UAbilitySystemComponent* Source, TSubclassOf<class UGameplayEffect> EffectClass, bool IsLeft)
{
	if (Source == nullptr) return;
	// Apply Effect and Add it to WeaponEffects Array
	auto Context = Source->MakeEffectContext();
	auto Spec = Source->MakeOutgoingSpec(EffectClass, 0.f, Context);

	if (IsLeft)
		Spec.Data.Get()->DynamicAssetTags.AddTag(GetTag("Effect.IsForWeapon.Left"));
	else
		Spec.Data.Get()->DynamicAssetTags.AddTag(GetTag("Effect.IsForWeapon.Right"));

	Source->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UDiamondBlueprintFunctionLibrary::RemoveWeaponEffects(class UAbilitySystemComponent* Target, bool IsLeft)
{
	static FGameplayTag LeftTag = GetTag("Effect.IsForWeapon.Left");
	static FGameplayTag RightTag = GetTag("Effect.IsForWeapon.Right");
	if (Target == nullptr) return;
	Target->RemoveActiveEffectsWithTags(FGameplayTagContainer(IsLeft ? LeftTag : RightTag));
}

void UDiamondBlueprintFunctionLibrary::HitCharacterWithWeapon(ADiamondCharacter* SourceCharacter, AMeleeWeapon* Weapon, ADiamondCharacter* TargetCharacter, float PoiseDamage, const FHitResult& HitResult)
{
	ApplyWeaponDamageTo(
		SourceCharacter->GetAbilitySystemComponent(),
		Weapon->bIsLeftWeapon,
		TargetCharacter->GetAbilitySystemComponent());

	// knockback
	FVector LaunchVelocity;
	switch (Weapon->WeaponDataAsset->WeaponType)
	{
	case EWeaponType::CUSTOM:
		LaunchVelocity = SourceCharacter->GetActorForwardVector() * 10.f;
		break;
	default:
		LaunchVelocity = SourceCharacter->GetActorForwardVector() * 10.f;
	}
	TargetCharacter->LaunchCharacter(LaunchVelocity, false, false);

	//
	// Call events 
	//
	auto WeaponDataAsset = Weapon->WeaponDataAsset;

	// Call Weapon Effect applied to Target
	auto EffTarget = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnHitTargetCharacter);
	if (EffTarget)
		TargetCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffTarget->GetDefaultObject(),
			0.f,
			TargetCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call Target's events
	TargetCharacter->OnHitByEnemyDelegate.Broadcast(SourceCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, PoiseDamage, Weapon);

	// Call Weapon Effect applied to Source
	auto EffSource = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnHitSourceCharacter);
	if (EffSource)
		SourceCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffSource->GetDefaultObject(),
			0.f,
			SourceCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call source's events
	SourceCharacter->OnEnemyHitDelegate.Broadcast(TargetCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, PoiseDamage, Weapon);
}

void UDiamondBlueprintFunctionLibrary::BlockCharacterAttackEvents(ADiamondCharacter* SourceCharacter, AShieldWeapon* Shield,  ADiamondCharacter* TargetCharacter,  AWeapon* AtkWeapon, float PoiseDamage, const FHitResult& HitResult)
{
	auto WeaponDataAsset = Shield->WeaponDataAsset;

	// Call Weapon Effect applied to Source
	auto EffSource = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnBlockedSourceCharacter);
	if (EffSource)
		SourceCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffSource->GetDefaultObject(),
			0.f,
			SourceCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call Source's Events
	SourceCharacter->OnBlockedAttackDelegate.Broadcast(TargetCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, PoiseDamage, AtkWeapon);

	// Call Weapon Effect applied to Target
	auto EffTarget = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnBlockedTargetCharacter);
	if (EffTarget)
		TargetCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffTarget->GetDefaultObject(),
			0.f,
			TargetCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call Target's Events (Target hit source, no poise damage and info about shield)
	TargetCharacter->OnEnemyHitDelegate.Broadcast(SourceCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, 0.f, Shield);
}

void UDiamondBlueprintFunctionLibrary::ParryCharacterAttackEvents(ADiamondCharacter* SourceCharacter, AShieldWeapon* Shield, ADiamondCharacter* TargetCharacter, AWeapon* AtkWeapon, float PoiseDamage, const FHitResult& HitResult)
{
	auto WeaponDataAsset = Shield->WeaponDataAsset;

	// Call Weapon Effect applied to Source
	auto EffSource = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnParriedSourceCharacter);
	if (EffSource)
		SourceCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffSource->GetDefaultObject(),
			0.f,
			SourceCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call Source's Events
	SourceCharacter->OnParriedAttackDelegate.Broadcast(TargetCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, AtkWeapon);

	// Call Weapon Effects applied to Target
	auto EffTarget = WeaponDataAsset->GameplayEffects.Find(EWeaponEffectApplicationType::OnParriedTargetCharacter);
	if (EffTarget)
		TargetCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
			EffTarget->GetDefaultObject(),
			0.f,
			TargetCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	// Call Target's Events (target may not get stagggered => it's on target to call stager ability)
	// Boss may need multiple parrys so character won't get staggered (won't get parried)
	if (TargetCharacter->bCanGetStaggered && TargetCharacter->ShouldStaggerOnThisParry())
		TargetCharacter->OnStanceBrokenDelegate.Broadcast(SourceCharacter, HitResult.GetComponent(), HitResult.ImpactPoint, Shield);
}
