// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIAMOND_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Should be unique
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName ItemName;

	// Can't have more same items then this ()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int MaxCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName Description;
};
