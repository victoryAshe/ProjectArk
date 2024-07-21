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

// 공격 작업 실행될 때 호출 
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;
	
	// 애니메이션 몽타주 구현 필요!!
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

	// 공격 완료 및 성공 알리기 
	if (!IsAttacking)
	{
		// 공격이 끝났을 때 태스크가 끝났다고 알리기
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		PALOG(Warning, TEXT("Attack Finish"));
	}
}
