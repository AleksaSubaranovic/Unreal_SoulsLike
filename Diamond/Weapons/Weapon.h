// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "Weapon.generated.h"

UCLASS()
class DIAMOND_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UWeaponItemDataAsset* WeaponDataAsset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Weapon)
		class ADiamondCharacter* WeaponOwner;

	// Returns True if Weapon is equiped and in left hand, False if right hand, Error if not equiped
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Weapon)
		bool bIsLeftWeapon;

	EWeaponType GetWeaponType() override;

	virtual void EnableWeaponCollision(EMeleeAttackType AttackType) override;

	virtual void DisableWeaponCollision() override;

	virtual void WeaponCollisionTick() override;

};
