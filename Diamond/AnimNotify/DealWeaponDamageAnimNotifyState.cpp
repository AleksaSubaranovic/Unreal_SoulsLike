// Fill out your copyright notice in the Description page of Project Settings.


#include "DealWeaponDamageAnimNotifyState.h"
#include "Diamond/DiamondCharacter.h"
#include "Diamond/Weapons/MeleeWeapon.h"
#include "Diamond/DataAssets/WeaponItemDataAsset.h"

UDealWeaponDamageAnimNotifyState::UDealWeaponDamageAnimNotifyState()
{
	bShouldFireInEditor = false;
}

void UDealWeaponDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && Animation)
	{
		Character = Cast<ADiamondCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Weapon = Character->GetWeapon(bIsLeft);

			// func -> returns All friendlys
			Weapon->EnableWeaponCollision(AttackType); // doesn't really turn collision but sets up params
		}
	}
}

void UDealWeaponDamageAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Character)
	{
		Weapon->WeaponCollisionTick();
	}
}

void UDealWeaponDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (Character && MeshComp)
	{
		Weapon->DisableWeaponCollision();
	}
}
