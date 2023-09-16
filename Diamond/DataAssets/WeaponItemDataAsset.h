// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "Diamond/DiamondObjectAndStuff.h" // WeaponEnum is defined here
#include "WeaponItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UWeaponItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TEnumAsByte<EWeaponType> WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float PhysicalDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float MagicalDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float HolyDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FireDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FrostDamage = 0.f;

	// Radius of Sphere Trace from Start Socket to End Socket of weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float DamageRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FRotator RelativeRotation;

	// Effects that can be granted to weapon or characters that interact with weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TMap<TEnumAsByte<EWeaponEffectApplicationType>, TSubclassOf<class UGameplayEffect>> GameplayEffects;

	/*UWeaponItemDataAsset();*/
};
