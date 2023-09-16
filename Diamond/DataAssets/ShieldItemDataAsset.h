// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItemDataAsset.h"
#include "ShieldItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UShieldItemDataAsset : public UWeaponItemDataAsset
{
	GENERATED_BODY()
	
public:
	// How much poise Damage can be blocked (Attack with greater poise will break shield stance)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float BlockPoise;
	// How much stamina is lost when shield blocks attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float BlockStamina;
	// Radius of Parry from Center of Shield (CenterSocket for shields)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float ParryRadius;
	// Radius of Blocking collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float BlockRadius;
};
