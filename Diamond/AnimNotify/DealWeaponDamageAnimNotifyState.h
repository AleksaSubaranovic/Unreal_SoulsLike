// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Diamond/DiamondObjectAndStuff.h"
#include "DealWeaponDamageAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UDealWeaponDamageAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UDealWeaponDamageAnimNotifyState();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	class ADiamondCharacter* Character;

	UPROPERTY()
	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	bool bIsLeft;

	// Attack type, For poise 
	UPROPERTY(EditAnywhere, Category = Attack)
	EMeleeAttackType AttackType;
};
