// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterAttributeSet.h"
#include "DiamondAbilitySystemComponent.h"
#include "WeaponAttributeSet.h"
#include "DiamondBlueprintFunctionLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ADiamondCharacter

ADiamondCharacter::ADiamondCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	AbilitySystemComponent = CreateDefaultSubobject<UDiamondAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("Character Attribute Set"));
	WeaponAttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("Weapon Attribute Set"));

	//AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ADiamondCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &ADiamondCharacter::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ADiamondCharacter::MaxHealthChanged);
		
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetManaAttribute()).AddUObject(this, &ADiamondCharacter::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ADiamondCharacter::MaxManaChanged);

		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetStaminaAttribute()).AddUObject(this, &ADiamondCharacter::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharacterAttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &ADiamondCharacter::MaxStaminaChanged);
	}

	OnEnemyHitDelegate.AddDynamic(this, &ADiamondCharacter::OnEnemyHit);
	OnHitByEnemyDelegate.AddDynamic(this, &ADiamondCharacter::OnHitByEnemy);
	OnBlockedAttackDelegate.AddDynamic(this, &ADiamondCharacter::OnBlockedAttack);
	OnParriedAttackDelegate.AddDynamic(this, &ADiamondCharacter::OnParriedAttack);
	OnStanceBrokenDelegate.AddDynamic(this, &ADiamondCharacter::OnStanceBroken);
}

float ADiamondCharacter::GetHealth() const
{
	return CharacterAttributeSet->GetHealth();
}

float ADiamondCharacter::GetMaxHealth() const
{
	return CharacterAttributeSet->GetMaxHealth();
}

float ADiamondCharacter::GetMana() const
{
	return CharacterAttributeSet->GetMana();
}

float ADiamondCharacter::GetMaxMana() const
{
	return CharacterAttributeSet->GetMaxMana();
}

float ADiamondCharacter::GetStamina() const
{
	return CharacterAttributeSet->GetStamina();
}

float ADiamondCharacter::GetMaxStamina() const
{
	return CharacterAttributeSet->GetMaxStamina();
}

void ADiamondCharacter::SetHealth(float Value)
{
	CharacterAttributeSet->SetHealth(Value);
}

void ADiamondCharacter::SetMaxHealth(float Value)
{
	CharacterAttributeSet->SetMaxHealth(Value);
}

void ADiamondCharacter::SetMana(float Value)
{
	CharacterAttributeSet->SetMana(Value);
}

void ADiamondCharacter::SetMaxMana(float Value)
{
	CharacterAttributeSet->SetMaxMana(Value);
}

void ADiamondCharacter::SetStamina(float Value)
{
	CharacterAttributeSet->SetStamina(Value);
}

void ADiamondCharacter::SetMaxStamina(float Value)
{
	CharacterAttributeSet->SetMaxStamina(Value);
}

UAbilitySystemComponent* ADiamondCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* ADiamondCharacter::GetCharacterAttributeSet() const
{
	return CharacterAttributeSet;
}

UWeaponAttributeSet* ADiamondCharacter::GetWeaponAttributeSet() const
{
	return WeaponAttributeSet;
}

void ADiamondCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	//UE_LOG(LogTemp, Warning, TEXT("health is , %f"), Health);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("cpp hp changed %f"), Health));
}

void ADiamondCharacter::MaxHealthChanged(const FOnAttributeChangeData& Data)
{

}

void ADiamondCharacter::ManaChanged(const FOnAttributeChangeData& Data)
{

}

void ADiamondCharacter::MaxManaChanged(const FOnAttributeChangeData& Data)
{

}

void ADiamondCharacter::StaminaChanged(const FOnAttributeChangeData& Data)
{

}

void ADiamondCharacter::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{

}

void ADiamondCharacter::OnHitShield(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, AWeapon* Weapon)
{
}

void ADiamondCharacter::OnEnemyHit(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, AWeapon* Weapon)
{
	if (PoiseDamage == 0.f)
	{
		// Weapon is Shield
		OnHitShield(Character, HitComponent, ImpactPoint, PoiseDamage, Weapon);
	}
}

void ADiamondCharacter::OnHitByEnemy(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, AWeapon* Weapon)
{	
	// check for hyper armor
	if (AbilitySystemComponent->HasMatchingGameplayTag(HyperArmorTag)) return;

	const float MaxStamina = CharacterAttributeSet->GetMaxStamina();
	const float CurrentStamina = CharacterAttributeSet->GetStamina();
	const float MaxPoise = CharacterAttributeSet->GetMaxPoise();

	// Don't react
	if (CharacterAttributeSet->GetPoise() == 0.f) return;

	const float BonusPoiseDmg = CurrentStamina < MaxStamina * 0.25f ? 0.05f : 0.0f;

	int ReactRes = 0;
#define REACT_HIT 1
#define REACT_FALL 2
#define REACT_STAGGER 3

	if (PoiseDamage > (PoisePrecentageForWeaponHit - BonusPoiseDmg) * MaxPoise)
	{
		if (PoiseDamage > (PoisePrecentageForFall)*MaxPoise)
		{
			ReactRes = bShouldReactToHit ? REACT_FALL : 0;
		}
		else
		{
			ReactRes = bShouldReactToHit ? REACT_HIT : 0;
		}
	}
	// use Poise Effect (Reduce Poise)
	UDiamondBlueprintFunctionLibrary::ReducePoise(AbilitySystemComponent, PoiseDamage);

	if (CharacterAttributeSet->GetPoise() == 0.f)
	{
		ReactRes = bCanGetStaggered && ReactRes != REACT_FALL ? REACT_STAGGER : ReactRes;
	}

	// Deal with res
	switch (ReactRes)
	{
	case REACT_HIT:
		if (CharacterAnimState != ECharacterAnimState::Hit)
		{
			StopAnimMontage();
			CharacterAnimState = ECharacterAnimState::Hit;
			GetWorldTimerManager().SetTimer(OnHitAnimationTimerHandle, this, &ADiamondCharacter::ResetOnHitStateFromTimerHandle, HitImunityTime, false);
		}
		break;
	case REACT_FALL:
		break;
	case REACT_STAGGER:
		OnStanceBroken(Character, HitComponent, ImpactPoint, Weapon);
		break;
	default: // Nothing
		break;
	}
}

void ADiamondCharacter::OnBlockedAttack(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const float PoiseDamage, AWeapon* Weapon)
{
	// nothing (knockback?)
}

void ADiamondCharacter::OnParriedAttack(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, AWeapon* Weapon)
{
	// NOTHING
}

// Will be called if bCanGetStaggered = true
void ADiamondCharacter::OnStanceBroken(ADiamondCharacter* Character, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, AWeapon* Weapon)
{
	// char's poise is zero or got parried (if poise = zero and fall then not called)
	// play Stagger Ability (Anim Montage can be customized)


}

bool ADiamondCharacter::ShouldStaggerOnThisParry_Implementation()
{
	return true;
}


void ADiamondCharacter::ResetOnHitStateFromTimerHandle()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Can get hit again"));

	CharacterAnimState = ECharacterAnimState::Standard;
}



