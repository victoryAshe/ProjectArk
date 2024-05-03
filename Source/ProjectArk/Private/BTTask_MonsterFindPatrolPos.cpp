// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterFindPatrolPos.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_MonsterFindPatrolPos::UBTTask_MonsterFindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

// ���� AI�� ������ �ݰ� ������ ���� ��ġ�� ã�� �����忡 ����
EBTNodeResult::Type UBTTask_MonsterFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	// �׺���̼� �ý��� ��������
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	// ���� ���� ��ġ ��������
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonsterAIController::HomePosKey);
	FNavLocation NextPatrol;

	// ���� ���� ����
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}