// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "ShieldWeapon.generated.h"

UCLASS()
class DIAMOND_API AShieldWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShieldWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Shield")
	class UShieldItemDataAsset* ShieldDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Shield")
	class USphereComponent* ShieldCollision;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Shield")
	virtual void EnableShieldCollision(bool IsParry) override;
	UFUNCTION(BlueprintCallable, Category = "Weapon|Shield")
	virtual void DisableShieldCollision() override;

	virtual void OnHit(ADiamondCharacter* Character, AWeapon* Weapon, const float PoiseDamage, const FHitResult& HItResult);

private:
	FCollisionQueryParams Params;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Weapon|Shield")
		bool bIsParry = false;
};
