// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReturnToHomePos.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectArkCharacter.h"

UBTTask_ReturnToHomePos::UBTTask_ReturnToHomePos()
{
    NodeName = TEXT("ReturnToHomePos");
}

EBTNodeResult::Type UBTTask_ReturnToHomePos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        FVector HomePos = AIController->GetBlackboardComponent()->GetValueAsVector(AMonsterAIController::HomePosKey);
        AIController->MoveToLocation(HomePos);

        // HomePos에 도착하면 bShouldReturnHome을 false로 설정
        // AIController->GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::ShouldReturnHomeKey, false);
        
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}