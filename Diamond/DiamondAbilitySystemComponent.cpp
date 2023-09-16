// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondAbilitySystemComponent.h"
#include "DiamondObjectAndStuff.h"
#include "DiamondCharacter.h"
#include "Abilities/DiamondAbility.h"

UDiamondAbilitySystemComponent::UDiamondAbilitySystemComponent()
{
	this->ReplicationMode = EGameplayEffectReplicationMode::Full;
	// SINGLEPLAYER GAME
	PrimaryAbility = nullptr;
}

void UDiamondAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ADiamondCharacter>(GetOwner());

	/*AbilityActivatedCallbacks.AddUObject(this, &UDiamondAbilitySystemComponent::SetCharacterStateOnActivation);
	AbilityEndedCallbacks.AddUObject(this, &UDiamondAbilitySystemComponent::SetCharacterStateOnDeactivation);*/

	AbilityActivatedCallbacks.AddUObject(this, &UDiamondAbilitySystemComponent::UpdatePrimary);
	AbilityEndedCallbacks.AddUObject(this, &UDiamondAbilitySystemComponent::CheckForPrimaryOnEnd);
	UE_LOG(LogTemp, Warning, TEXT("Ability system comp begin play"));
}

void UDiamondAbilitySystemComponent::UpdatePrimary(UGameplayAbility* Ability)
{
	UDiamondAbility* DAbility = Cast<UDiamondAbility>(Ability);
	if (DAbility && DAbility->bIsPrimaryAbility && GetOwner())
	{
		if (PrimaryAbility)
		{
			PrimaryAbility->K2_CancelAbility();
			UE_LOG(LogTemp, Warning, TEXT("Prim Ability"));
		}
		PrimaryAbility = DAbility;
	}
}

void UDiamondAbilitySystemComponent::CheckForPrimaryOnEnd(UGameplayAbility* Ability)
{
	UDiamondAbility* DAbility = Cast<UDiamondAbility>(Ability);
	if (DAbility && DAbility->bIsPrimaryAbility && GetOwner())
	{
		if (PrimaryAbility == DAbility)
			PrimaryAbility = nullptr;
	}
}

void UDiamondAbilitySystemComponent::SetCharacterStateOnActivation(UGameplayAbility* Ability)
{
	/*UDiamondAbility* DAbility = Cast<UDiamondAbility>(Ability);
	if (DAbility && DAbility->bIsPrimaryAbility && CharacterOwner)
	{
		CharacterOwner->CharacterAnimState = ECharacterAnimState::Ability;
	}*/
}

void UDiamondAbilitySystemComponent::SetCharacterStateOnDeactivation(UGameplayAbility* Ability)
{
	/*UDiamondAbility* DAbility = Cast<UDiamondAbility>(Ability);
	if (DAbility && DAbility->bIsPrimaryAbility && CharacterOwner)
	{
		CharacterOwner->CharacterAnimState = ECharacterAnimState::Standard;
	}*/
}
