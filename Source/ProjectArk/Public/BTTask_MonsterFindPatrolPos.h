// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MonsterFindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARK_API UBTTask_MonsterFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MonsterFindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
