// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DiamondAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UDiamondAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UDiamondAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cost)
	float HealthCost = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cost)
	float ManaCost = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cost)
	float StaminaCost = 0.f;

	class UGameplayEffect* GetCostGameplayEffect() const override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// Only One Ability like this can be activated at a time
	// For Secondary Ability Add GameplayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bIsPrimaryAbility = true;

	UFUNCTION(BlueprintCallable, Category = Commit)
	void AddCommitTag();

	UFUNCTION(BlueprintCallable, Category = Commit)
	void RemoveCommitTag();

	UPROPERTY(BlueprintReadOnly)
	bool bHasCommitTag = false;

	// don't use this
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	// Use property values (no effect)
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	// calls AddCommitTag also
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	// Uses new Apply funcs
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	// Apply Cost with Reduce functions
	void ApplyCostEffect(class UAbilitySystemComponent* Source);

	// also removes commit tag if one is present
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
