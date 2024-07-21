// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PAItemSetting.generated.h"

/**
 * 
 */
UCLASS(config = ProjectArk)
class PROJECTARKSETTING_API UPAItemSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UPAItemSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> PotionAssets;
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> CollectableAssets;
};
