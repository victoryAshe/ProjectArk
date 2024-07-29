// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "PACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<APACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	// Ÿ���� ��ġ�� ���� ĳ������ ��ġ�� �̿��� �� ��ġ ���� ���� ���͸� ���
	FVector LookVector = Target->GetActorLocation() - MonsterCharacter->GetActorLocation();
	// Z ��(����) ������ ��ȭ�� ���� : ���� ĳ���Ͱ� ���� ��鿡���� ȸ��
	LookVector.Z = 0.0f; 
	// ���� ����(LookVector)�� �̿��� ȸ�� ����� �����ϰ�, �� ����� ȸ�� ��(FRotator)���� ��ȯ
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// �ε巴�� ȸ�� FMath::RInterpTo �� ���� ���� �Լ�
	MonsterCharacter->SetActorRotation(FMath::RInterpTo(MonsterCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}