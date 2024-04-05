// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARK_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();
	virtual void OnPossess(APawn* InPawn) override;
	// virtual void OnUnPossess() override;

private:
	// void OnRepeatTimer();

	// FTimerHandle RepeatTimerHandle;
	// float RepeatInterval;

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
