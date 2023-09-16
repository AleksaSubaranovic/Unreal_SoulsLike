// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Weapons/WeaponInterface.h"
#include "DiamondObjectAndStuff.h"
#include "DiamondCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnEnemyHitDelegate, class ADiamondCharacter*, Character, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, const float, PoiseDamage, class AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHitByEnemyDelegate, class ADiamondCharacter*, Character, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, const float, PoiseDamage, class AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnBlockedAttackDelegate, class ADiamondCharacter*, Character, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, const float, PoiseDamage, class AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnParriedAttackDelegate, class ADiamondCharacter*, Character, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, class AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStanceBrokenDelegate, class ADiamondCharacter*, Character, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, class AWeapon*, Weapon);

UCLASS(config=Game)
class ADiamondCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADiamondCharacter();

protected:
	// To add mapping context
	virtual void BeginPlay();

public:

	//
	// Ability Components
	//

	UFUNCTION(BlueprintCallable, Category = AbilitySystemComponent)
		class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = AttributeSet)
		class UCharacterAttributeSet* GetCharacterAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = AttributeSet)
		class UWeaponAttributeSet* GetWeaponAttributeSet() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UDiamondAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCharacterAttributeSet* CharacterAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWeaponAttributeSet* WeaponAttributeSet;

	//
	// Attribute Get/Set, for basic common Attributes, others can be obtianed with Attribute
	// Can get AbilitySystemComponent and Try to find Attribute value
	//
public:
	UFUNCTION(BlueprintCallable)
		float GetHealth() const;
	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
		float GetMana() const;
	UFUNCTION(BlueprintCallable)
		float GetMaxMana() const;
	UFUNCTION(BlueprintCallable)
		float GetStamina() const;
	UFUNCTION(BlueprintCallable)
		float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable)
		void SetHealth(float Value);
	UFUNCTION(BlueprintCallable)
		void SetMaxHealth(float Value);
	UFUNCTION(BlueprintCallable)
		void SetMana(float Value);
	UFUNCTION(BlueprintCallable)
		void SetMaxMana(float Value);
	UFUNCTION(BlueprintCallable)
		void SetStamina(float Value);
	UFUNCTION(BlueprintCallable)
		void SetMaxStamina(float Value);

	// Delegates (most of other attributes need delegates for Player only or sometimes AI Characters)
	// TODO: add more if needed

	FDelegateHandle HealthChangedDelegateHandle;
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	FDelegateHandle MaxHealthChangedDelegateHandle;
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	FDelegateHandle ManaChangedDelegateHandle;
	virtual void ManaChanged(const FOnAttributeChangeData& Data);

	FDelegateHandle MaxManaChangedDelegateHandle;
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

	FDelegateHandle StaminaChangedDelegateHandle;
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);

	FDelegateHandle MaxStaminaChangedDelegateHandle;
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);


	//
	// Weapons
	//
protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Weapon)
	class AWeapon* LeftWeapon;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Weapon)
	class AWeapon* RightWeapon;

public:
	
	AWeapon* GetWeapon(bool IsLeft) const { return IsLeft ? LeftWeapon : RightWeapon; }
	TArray<AWeapon*> GetBothWeapons() const { return TArray{ LeftWeapon, RightWeapon }; }
	void SetWeapon(AWeapon* Weapon, bool IsLeft) { IsLeft ? LeftWeapon = Weapon : RightWeapon = Weapon; }


	UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
	void OnWeaponEquiped();
	UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
	void OnWeaponUnequiped();

	UPROPERTY(BlueprintAssignable, Category = Weapon)
		FOnEnemyHitDelegate OnEnemyHitDelegate;

	UPROPERTY(BlueprintAssignable, Category = Weapon)
		FOnHitByEnemyDelegate OnHitByEnemyDelegate;

	UPROPERTY(BlueprintAssignable, Category = Weapon)
		FOnBlockedAttackDelegate OnBlockedAttackDelegate;

	UPROPERTY(BlueprintAssignable, Category = Weapon)
		FOnParriedAttackDelegate OnParriedAttackDelegate;

	UPROPERTY(BlueprintAssignable, Category = Weapon)
		FOnStanceBrokenDelegate OnStanceBrokenDelegate;

private:
		void OnHitShield(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, class AWeapon* Weapon);

	// When Enemy is hit
	UFUNCTION()
		void OnEnemyHit(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, class AWeapon* Weapon);

	// When hit by enemy
	UFUNCTION()
		void OnHitByEnemy(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, class AWeapon* Weapon);

	// blocked enemy attack
	UFUNCTION()
		void OnBlockedAttack(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, class AWeapon* Weapon);
	
	// parried enemy attack
	UFUNCTION()
		void OnParriedAttack(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, class AWeapon* Weapon);

	// poise is zero after attack that didn't have character fall, or parried
	UFUNCTION()
		void OnStanceBroken(class ADiamondCharacter* Character, class UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, class AWeapon* Weapon);

public:
	// TODO:
	//const class TArray<AActor*>& GetAllFriendlyActors() const { return TArray<AActor*>(); };

	//
	// Poise
	//

	// Between 0.0 and 1.f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise, meta = (ClampMin="0.0", ClampMax="1.0"))
	float PoisePrecentageForWeaponHit = 0.2f;

	// if character can fall from explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PoisePrecentageForFall = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
	bool bShouldReactToHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
	bool bShouldReactToExplosion = true;

	// If poise is zero should character use StaggerAbility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
	bool bCanGetStaggered = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise, meta=(ClampMin="0.1"))
	float StaggerTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
	bool bShouldGetStaggeredOnShieldBreak = true;

	UFUNCTION(BlueprintNativeEvent, Category = Poise)
	bool ShouldStaggerOnThisParry();

	// How long character will not be able to Play OnHitAnimations if already playing one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Poise)
	float HitImunityTime = 0.33f;

	const FGameplayTag HyperArmorTag = FGameplayTag::RequestGameplayTag(FName("Character.HasHyperArmor"));


	//
	// Anim State
	//
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Animation")
	ECharacterAnimState CharacterAnimState = ECharacterAnimState::Standard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	bool bIsUpeerBodyAnim = false;

	//
	// Timers
	//

private:
	FTimerHandle OnHitAnimationTimerHandle;
	UFUNCTION()
		void ResetOnHitStateFromTimerHandle();
};