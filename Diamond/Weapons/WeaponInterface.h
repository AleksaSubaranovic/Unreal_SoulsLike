// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Diamond/DiamondObjectAndStuff.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIAMOND_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual EWeaponType GetWeaponType();

	virtual void EnableWeaponCollision(EMeleeAttackType AttackType);

	virtual void DisableWeaponCollision();

	virtual void WeaponCollisionTick();

	virtual void EnableShieldCollision(bool IsParry);

	virtual void DisableShieldCollision();
};
