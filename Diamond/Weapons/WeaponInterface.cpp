// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInterface.h"

// Add default functionality here for any IWeaponInterface functions that are not pure virtual.

EWeaponType IWeaponInterface::GetWeaponType()
{
    return EWeaponType::CUSTOM;
}

void IWeaponInterface::EnableWeaponCollision(EMeleeAttackType AttackType)
{
}

void IWeaponInterface::DisableWeaponCollision()
{
}

void IWeaponInterface::WeaponCollisionTick()
{
}

void IWeaponInterface::EnableShieldCollision(bool IsParry)
{
}

void IWeaponInterface::DisableShieldCollision()
{

}
