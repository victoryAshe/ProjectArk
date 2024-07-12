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

// ���� ���� ���� �÷��̾� �����ϴ��� Ȯ�� 
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI�� Pawn ������
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) 
	{
		PALOG(Error, TEXT("ControllingPawn is null"));
		return false;
	}

	// �����忡�� TargetKey�� �ش��ϴ� ���� ������ AProjectArkCharacter�� ĳ����
	auto Target = Cast<AProjectArkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetKey));
	if (nullptr == Target) 
	{
		PALOG(Error, TEXT("Target is null"));
		return false;
	}

	// Target�� ControllingPawn ������ �Ÿ��� 200 �������� Ȯ��
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
		
	return bResult;
}
