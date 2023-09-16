// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondAbility.h"
#include "AbilitySystemComponent.h"
//#include "Diamond/Effects/AbilityCostEffect.h"
#include "Diamond/DiamondCharacter.h"
#include "Diamond/DiamondAbilitySystemComponent.h"
#include "Diamond/DiamondObjectAndStuff.h"
#include "Diamond/CharacterAttributeSet.h"
#include "Diamond/DiamondAbilitySystemGlobals.h"
#include "Diamond/DiamondBlueprintFunctionLibrary.h"

UDiamondAbility::UDiamondAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::Type::ServerOnly;
	// TODO: add tags that need block ability from being activated
}

//#define HEALTH_INDEX 0
//#define MANA_INDEX 1
//#define STAMINA_INDEX 2

// never called
UGameplayEffect* UDiamondAbility::GetCostGameplayEffect() const
{
	// Not To be used



	/*CostEffect->Modifiers[HEALTH_INDEX].ModifierMagnitude = FScalableFloat(HealthCost * -1.f);
	CostEffect->Modifiers[MANA_INDEX].ModifierMagnitude = FScalableFloat(ManaCost * -1.f);
	CostEffect->Modifiers[STAMINA_INDEX].ModifierMagnitude = FScalableFloat(StaminaCost * -1.f);*/
	
	//UE_LOG(LogTemp, Warning, TEXT("LOG:GET COST GAMEPLAY EFFECT WORKS. Hp: %f "), CostEffect->Modifiers[HEALTH_INDEX].ModifierMagnitude.ScalableFloatMagnitude.Value);

	return nullptr;
}

void UDiamondAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
}

void UDiamondAbility::AddCommitTag()
{
	static FGameplayTag CommitTag = FGameplayTag::RequestGameplayTag(FName("CommitedToAbility"));
	auto AbilitySystemComp = GetAbilitySystemComponentFromActorInfo_Ensured();
	AbilitySystemComp->AddLooseGameplayTag(CommitTag);
	bHasCommitTag = true;
}

void UDiamondAbility::RemoveCommitTag()
{
	static FGameplayTag CommitTag = FGameplayTag::RequestGameplayTag(FName("CommitedToAbility"));
	auto AbilitySystemComp = GetAbilitySystemComponentFromActorInfo_Ensured();
	AbilitySystemComp->RemoveLooseGameplayTag(CommitTag);
	bHasCommitTag = false;
}

bool UDiamondAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bRes = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	static FGameplayTag CommitTag = FGameplayTag::RequestGameplayTag(FName("CommitedToAbility"));

	if (bRes)
	{
		// Can Use Ability will check to be only Primary Ability
		// Ability may not commit and play anim montage BUT will be canceled for diffrent anim montage => Ending ability
		if (bIsPrimaryAbility)
		{
			UDiamondAbilitySystemComponent* AbilitySystem = Cast<UDiamondAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
			if (AbilitySystem->HasMatchingGameplayTag(CommitTag) || AbilitySystem->CharacterOwner->CharacterAnimState == ECharacterAnimState::Hit)
				return false;
		}
		return true;
	}
	else return false;
}

bool UDiamondAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	check(AbilitySystemComponent != nullptr);

	ADiamondCharacter* Character = Cast<ADiamondCharacter>(GetAvatarActorFromActorInfo());
	check(Character != nullptr);

	return Character->GetHealth() > HealthCost && Character->GetMana() >= ManaCost && Character->GetStamina() >= StaminaCost;
}

bool UDiamondAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	// Last chance to fail (maybe we no longer have resources to commit since we after we started this ability activation)
	if (!CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	CommitExecute(Handle, ActorInfo, ActivationInfo);

	// Fixme: Should we always call this or only if it is implemented? A noop may not hurt but could be bad for perf (storing a HasBlueprintCommit per instance isn't good either)
	Super::K2_CommitExecute();

	// Broadcast this commitment
	ActorInfo->AbilitySystemComponent->NotifyAbilityCommit(this);

	AddCommitTag();

	return true;
}

bool UDiamondAbility::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	if (UAbilitySystemGlobals::Get().ShouldIgnoreCosts())
	{
		return true;
	}

	// Last chance to fail (maybe we no longer have resources to commit since we after we started this ability activation)
	if (!CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	ApplyCostEffect(ActorInfo->AbilitySystemComponent.Get());
	return true;
}

void UDiamondAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// apply cooldown (TODO)

	ApplyCostEffect(ActorInfo->AbilitySystemComponent.Get());
}

void UDiamondAbility::ApplyCostEffect(UAbilitySystemComponent* Source)
{
	using FuncLib = UDiamondBlueprintFunctionLibrary;

	FuncLib::ReduceHealth(Source, HealthCost);
	FuncLib::ReduceMana(Source, ManaCost);
	FuncLib::ReduceStamina(Source, StaminaCost);
}

void UDiamondAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bHasCommitTag)
		RemoveCommitTag();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
