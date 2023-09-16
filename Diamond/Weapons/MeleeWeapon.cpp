// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Diamond/DiamondCharacter.h"
#include "Diamond/DataAssets/WeaponItemDataAsset.h"
#include "Diamond/DiamondBlueprintFunctionLibrary.h"
#include "Diamond/DiamondAbilitySystemComponent.h"
#include "ShieldWeapon.h"

#define MeleeWeaponChannel ECC_GameTraceChannel1
#define ShieldCollisionChannel ECC_GameTraceChannel2

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ShieldCollisionChannel);
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	//Params.bDebugQuery = true;
	PoiseDamage = 0.f;
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeapon::EnableWeaponCollision(EMeleeAttackType AttackType)
{
	Params.AddIgnoredActor(WeaponOwner);
	Params.AddIgnoredActor(WeaponOwner->GetWeapon(true));
	Params.AddIgnoredActor(WeaponOwner->GetWeapon(false));
	//Params.AddIgnoredActors(IgnoredActors);

	// calc poise
	PoiseDamage = UDiamondObjectAndStuff::CalcMeleePoiseDamage(WeaponDataAsset->WeaponType, AttackType);
	CharactersToIgnore.Add(WeaponOwner);
}

void AMeleeWeapon::DisableWeaponCollision()
{
	//HitActors.Empty();
	Params.ClearIgnoredActors();
	PoiseDamage = 0.f;
	CharactersToIgnore.Empty();
}

void AMeleeWeapon::OnHitShield(AShieldWeapon* Shield, const FHitResult& HitResult)
{
	// check if character was already hit
	if (CharactersToIgnore.Contains(Shield->WeaponOwner))
	{
		Params.AddIgnoredActor(Shield);
	}
	else
	{
		Params.AddIgnoredActor(Shield);
		Params.AddIgnoredActor(Shield->WeaponOwner);
		
		CharactersToIgnore.Add(Shield->WeaponOwner); // so character in HitResults is ignored
		// Tell Shield that it was hit by melee weapon
		// This will call other events (OnBlock/OnParry etc.)
		// Will deal with shield and shield owner state
		// will tell meleeWeapon owner what happend
		Shield->OnHit(WeaponOwner, this, PoiseDamage, HitResult);
	}
}

void AMeleeWeapon::WeaponCollisionTick()
{
	static FName StartSocket("StartSocket");
	static FName EndSocket("EndSocket");
	static FGameplayTag ImuneTag = FGameplayTag::RequestGameplayTag(FName("Character.HasIFrames"));

	TArray<FHitResult> OutResults;
	const FVector SweepStart = WeaponMesh->GetSocketLocation(StartSocket);
	const FVector SweepEnd = WeaponMesh->GetSocketLocation(EndSocket);

	if (GetWorld()->SweepMultiByObjectType(OutResults, SweepStart, SweepEnd, FQuat::Identity, ObjectParams,
		FCollisionShape::MakeSphere(WeaponDataAsset->DamageRadius), Params))
	{
		for (auto& Res : OutResults)
		{
			AShieldWeapon* Shield = Cast<AShieldWeapon>(Res.GetActor());
			if (Shield)
			{
				FVector Distance = Shield->WeaponOwner->GetActorLocation() - WeaponOwner->GetActorLocation();
				Distance.Normalize();
				if (FVector::DotProduct(WeaponOwner->GetActorForwardVector(), Distance) >= 0.0) // in front of Attacker
				{
					OnHitShield(Shield, Res);
				}
				else
				{
					Params.AddIgnoredActor(Shield);
				}
			}
			else
			{
				ADiamondCharacter* CharacterHit = Cast<ADiamondCharacter>(Res.GetActor());
				if (CharacterHit != nullptr && !CharactersToIgnore.Contains(CharacterHit))
				{
					// check for imunity
					if (!CharacterHit->GetAbilitySystemComponent()->HasMatchingGameplayTag(ImuneTag))
					{
						UDiamondBlueprintFunctionLibrary::HitCharacterWithWeapon(
							WeaponOwner,
							this,
							CharacterHit,
							PoiseDamage,
							Res);

						Params.AddIgnoredActor(Res.GetActor()); // actor can be hit only once by same attack
						CharactersToIgnore.Add(CharacterHit); // if shield is next

						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SUCC"));
					}
				}
			}
			// MAYBE add to Ignore actors?
		}
	}
	DrawDebugSphere(GetWorld(), SweepEnd, WeaponDataAsset->DamageRadius, 12, FColor::Red);
}

