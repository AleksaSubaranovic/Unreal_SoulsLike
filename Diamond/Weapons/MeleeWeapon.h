// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class DIAMOND_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend class ADiamondCharacter; // maybe useful in future

	// Use default effects and character events

	friend class UDealWeaponDamageAnimNotifyState;
protected:

	virtual void EnableWeaponCollision(EMeleeAttackType AttackType) override;

	virtual void DisableWeaponCollision() override;

	virtual void WeaponCollisionTick() override;

	FCollisionQueryParams Params;

	float PoiseDamage;

	void OnHitShield(class AShieldWeapon* Shield, const FHitResult& HitResult);

	FCollisionObjectQueryParams ObjectParams;

	// Characters to ignore (changes as character is hit)
	UPROPERTY(VisibleInstanceOnly, Category = Weapon)
		TSet<ADiamondCharacter*> CharactersToIgnore;
};
