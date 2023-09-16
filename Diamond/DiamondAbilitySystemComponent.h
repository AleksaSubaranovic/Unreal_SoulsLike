// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DiamondAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UDiamondAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UDiamondAbilitySystemComponent();

	void SetCharacterStateOnActivation(UGameplayAbility* Ability);
	void SetCharacterStateOnDeactivation(UGameplayAbility* Ability);

	void UpdatePrimary(UGameplayAbility* Ability);
	void CheckForPrimaryOnEnd(UGameplayAbility* Ability);

	friend class UDiamondAbility;

protected:

	void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	class ADiamondCharacter* CharacterOwner;

	UPROPERTY(VisibleInstanceOnly)
	class UDiamondAbility* PrimaryAbility;
};
