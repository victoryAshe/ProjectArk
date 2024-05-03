// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterFindPatrolPos.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_MonsterFindPatrolPos::UBTTask_MonsterFindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

// 몬스터 AI가 지정된 반경 내에서 정찰 위치를 찾고 블랙보드에 저장
EBTNodeResult::Type UBTTask_MonsterFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	// 네비게이션 시스템 가져오기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	// 정찰 시작 위치 가져오기
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonsterAIController::HomePosKey);
	FNavLocation NextPatrol;

	// 랜덤 정찰 시작
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}