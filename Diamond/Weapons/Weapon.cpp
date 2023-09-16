// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Diamond/DiamondCharacter.h"
#include "Diamond/DataAssets/WeaponItemDataAsset.h"
#include "Diamond/DiamondBlueprintFunctionLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Weapon Mesh"));
	SetRootComponent(WeaponMesh);
	//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EWeaponType AWeapon::GetWeaponType()
{
	return WeaponDataAsset->WeaponType;
}

void AWeapon::EnableWeaponCollision(EMeleeAttackType)
{
}

void AWeapon::DisableWeaponCollision()
{
}

void AWeapon::WeaponCollisionTick()
{
}
