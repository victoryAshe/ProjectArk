// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReturnToHomePos.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARK_API UBTTask_ReturnToHomePos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ReturnToHomePos();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
