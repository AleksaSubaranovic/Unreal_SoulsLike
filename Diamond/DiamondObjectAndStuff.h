// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffectTypes.h"
#include "DiamondObjectAndStuff.generated.h"

USTRUCT(BlueprintType)
struct FDiamondGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
        bool bIsWeapon = false;

    UPROPERTY(BlueprintReadWrite)
        bool bIsSpell = false;

    UPROPERTY(BlueprintReadWrite)
        bool bIsLeftWeapon = false;

    UPROPERTY(BlueprintReadWrite)
        bool bIsHeal = false;

    /** Returns the actual struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return StaticStruct();
    }

    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FDiamondGameplayEffectContext* Duplicate() const override
    {
        FDiamondGameplayEffectContext* NewContext = new FDiamondGameplayEffectContext();
        *NewContext = *this;
        NewContext->AddActors(Actors);
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }
};

template <>
struct TStructOpsTypeTraits<FDiamondGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FDiamondGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = false,
        WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
    };
};

// TODO: add more types

// Weapon Type enum
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword UMETA(DisplayName = "Sword"),
    Shield   UMETA(DisplayName = "Shield"),
    GreatSword      UMETA(DisplayName = "Great Sword"),
    CUSTOM   UMETA(DisplayName = "CUSTOM")
};

// How effects are applied by Weapon 
UENUM(BlueprintType)
enum class EWeaponEffectApplicationType : uint8
{
    // Granted When equiped and removed when unequiped to Owner
    OnEquip UMETA(DisplayName = "Equip Player"),
    // Applied to character using weapon when enemy is hit (MUST HIT ENEMY CHARACTER To Trigger)
    OnHitSourceCharacter UMETA(DisplayName = "On Hit Source Character"),
    // Applied to character hit by weapon
    OnHitTargetCharacter UMETA(DisplayName = "On Hit Target Character"),
    // Applied to character using shield when parrying Succesfully
    OnParriedSourceCharacter UMETA(DisplayName = "On Parried Source Character"),
    // Applied to target chracter when parried
    OnParriedTargetCharacter UMETA(DisplayName = "On Parried Target Character"),
    // Applied When Character with shield blocks attack (not for parry), For Shield User
    OnBlockedSourceCharacter UMETA(DisplayName = "On Blocked Source Character"),
    // Applied When Character's attack is blocked with shield (not for parry), For Attacker
    OnBlockedTargetCharacter UMETA(DisplayName = "On Blocked Target Character")
};

UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
    // Light Attack = basic attack usually (Low Poise Dmg)
    LightAttack UMETA(DisplayName = "Light Attack"),
    // Heavy Attack = stronger attack more pose dmg
    HeavyAttack UMETA(DisplayName = "Heavy Attack"),
    // Charge Attack, calc Poise based on length of pressed button (Implement Special Ability Class)
    ChargeAttack UMETA(DisplayName = "Charge Attack"),
    // Jump Attack, Poise Dmg =? Light Attack
    JumpAttack UMETA(DisplayName = "Jump Attack"),
    // Heavy Jump Attack, Poise dmg > Heavy Attacj and opponent may fall on ground
    JumpHeavyAttack UMETA(DisplayName = "Jump Heavy Attack"),
    // Weapon Skill (USE CUSTOM POISE DAMAGE) Calls Ability
    WeaponSkill UMETA(DisplayName = "Weapon Skill")
};

UENUM(BlueprintType)
enum class ECharacterAnimState : uint8
{
    // Walk/Sprint/Jump
    Standard UMETA(DisplayName = "Standard"),
    // Character is hit
    Hit UMETA(DisplayName = "Hit")
};

/**
 *
 */
UCLASS()
class DIAMOND_API UDiamondObjectAndStuff : public UObject
{
    GENERATED_BODY()

public:
    static float WeaponTypeToPoiseDmg(EWeaponType WeaponType)
    {
        switch (WeaponType)
        {
        case EWeaponType::Sword: 
            return 10.f;
        default:
            return 1.f;
        }
    }
    static float AttackTypeToPoiseMultiplier(EMeleeAttackType AttackType)
    {
        switch (AttackType)
        {
        case EMeleeAttackType::LightAttack:
            return 1.f;
        case EMeleeAttackType::HeavyAttack:
            return 2.f;
        case EMeleeAttackType::JumpAttack:
            return 1.25f;
        case EMeleeAttackType::JumpHeavyAttack:
            return 3.f;
        default: return 1.f;
        }
    }
    // For Melee Poise Damage
    static float CalcMeleePoiseDamage(EWeaponType WeaponType, EMeleeAttackType AttackType)
    {
        return WeaponTypeToPoiseDmg(WeaponType) * AttackTypeToPoiseMultiplier(AttackType);
    }
};