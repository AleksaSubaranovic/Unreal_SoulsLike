// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldWeapon.h"
#include "Components/SphereComponent.h"
#include "Diamond/DataAssets/ShieldItemDataAsset.h"
#include "GameplayEffect.h"
#include "Diamond/CharacterAttributeSet.h"
#include "Diamond/DiamondCharacter.h"
#include "Diamond/DiamondAbilitySystemComponent.h"
#include "Diamond/DiamondBlueprintFunctionLibrary.h"

#define MeleeWeaponChannel ECC_GameTraceChannel1
#define ShieldCollisionChannel ECC_GameTraceChannel2

// Sets default values
AShieldWeapon::AShieldWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShieldCollision = CreateDefaultSubobject<USphereComponent>("Shield Collision");
	ShieldCollision->SetupAttachment(WeaponMesh, FName("CenterSocket"));
	ShieldCollision->SetGenerateOverlapEvents(true);
	ShieldCollision->SetCollisionProfileName(FName("ShieldCollision"));

	ShieldCollision->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AShieldWeapon::BeginPlay()
{
	Super::BeginPlay();

	ShieldCollision->Deactivate();
}

// Called every frame
void AShieldWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShieldWeapon::EnableShieldCollision(bool IsParry)
{
	ShieldCollision->SetSphereRadius(ShieldDataAsset->BlockRadius);
	bIsParry = IsParry;
	ShieldCollision->Activate();
}

void AShieldWeapon::DisableShieldCollision()
{
	ShieldCollision->Deactivate();
}

using FuncLib = UDiamondBlueprintFunctionLibrary;

// Shield Broke Call Events
void CallOnBrokenEvents(ADiamondCharacter* Character, AWeapon* Weapon, const FHitResult& HitResult, const float PoiseDamage)
{
	// Call Source's Events
	auto WeaponOwner = Weapon->WeaponOwner;
	if (WeaponOwner->bCanGetStaggered && WeaponOwner->bShouldGetStaggeredOnShieldBreak)
		WeaponOwner->OnStanceBrokenDelegate.Broadcast(Character, HitResult.GetComponent(), HitResult.Location, Weapon);
	// OnStanceBroken Should playe Stagger Ability.
}

void AShieldWeapon::OnHit(ADiamondCharacter* Character, AWeapon* Weapon, const float PoiseDamage, const FHitResult& HitResult)
{
	// Shouldn't call this if Shield lost it's Owner or Data Asset didn't load
	// Also if Character hitting shield is destroyed
	// If character dies somehow durring this call it will still be refrenced. (After short period will be removed)
	check(Character && WeaponOwner && WeaponDataAsset);

	// Weapon that hit shield can be null

	if (bIsParry)
	{
		FuncLib::ParryCharacterAttackEvents(WeaponOwner, this, Character, Weapon, PoiseDamage, HitResult);
	}
	else
	{
		FuncLib::ReduceStamina(WeaponOwner->GetAbilitySystemComponent(), ShieldDataAsset->BlockStamina);

		if (PoiseDamage > ShieldDataAsset->BlockPoise
			|| Character->GetCharacterAttributeSet()->GetStamina() == 0.f)
		{
			CallOnBrokenEvents(Character, Weapon, HitResult, PoiseDamage);
		}
		else FuncLib::BlockCharacterAttackEvents(WeaponOwner, this, Character, Weapon, PoiseDamage, HitResult);
	}
}
