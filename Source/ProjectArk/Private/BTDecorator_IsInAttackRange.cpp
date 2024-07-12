// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "ProjectArkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

// 공격 범위 내에 플레이어 존재하는지 확인 
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI의 Pawn 가져옴
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) 
	{
		PALOG(Error, TEXT("ControllingPawn is null"));
		return false;
	}

	// 블랙보드에서 TargetKey에 해당하는 값을 가져와 AProjectArkCharacter로 캐스팅
	auto Target = Cast<AProjectArkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetKey));
	if (nullptr == Target) 
	{
		PALOG(Error, TEXT("Target is null"));
		return false;
	}

	// Target과 ControllingPawn 사이의 거리가 200 이하인지 확인
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
		
	return bResult;
}
