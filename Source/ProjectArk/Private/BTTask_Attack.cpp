// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "ProjectArkCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

// ���� �۾� ����� �� ȣ�� 
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;
	
	// �ִϸ��̼� ��Ÿ�� ���� �ʿ�!!
	MonsterCharacter->Attack(); 
	IsAttacking = true;
	MonsterCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
		// PALOG(Warning, TEXT("AttackEnd.AddLambda : IsAttacking = false"));
		});
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� �Ϸ� �� ���� �˸��� 
	if (!IsAttacking)
	{
		// ������ ������ �� �½�ũ�� �����ٰ� �˸���
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		PALOG(Warning, TEXT("Attack Finish"));
	}
}
