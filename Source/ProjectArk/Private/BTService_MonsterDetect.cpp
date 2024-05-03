// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MonsterDetect.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "ProjectArkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// ���Ͱ� �÷��̾� �����ϴ� ���� 
UBTService_MonsterDetect::UBTService_MonsterDetect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f; // 1�ʸ��� ���� ����
}

void UBTService_MonsterDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� AI�����ϴ� Pawn ��������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.0f; // ���� ���� ������

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	// �ֺ� �浹ü ���� 
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, // �浹ü ���� ��� ���� �迭
		Center, 
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius), // ��ü ����� �浹ü
		CollisionQueryParam
	);

	// �浹ü �߰��ϸ�
	if (bResult) 
	{
		// PALOG(Log, TEXT("bResult is true."));
		for (auto const& OverlapResult : OverlapResults)
		{
			// �浹ü�� ���� �������� : �ش� ���Ͱ� �÷��̾� ĳ���Ͷ�� ProjectArkCharacter ������ �Ҵ�, �ƴ϶�� nullptr
			AProjectArkCharacter* ProjectArkCharacter = Cast<AProjectArkCharacter>(OverlapResult.GetActor());

			// ProjectArkCharacter�� �� && �÷��̾ �����ϴ� ��Ʈ�ѷ����� Ȯ��
			if (ProjectArkCharacter && ProjectArkCharacter->GetController()->IsPlayerController())
			{
				// AMonsterAIController�� �����忡 �÷��̾ Ÿ������ ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, ProjectArkCharacter);
				// �����Ǹ� �ʷϻ����� ǥ��
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, ProjectArkCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ProjectArkCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	// ���� �� �Ǹ� ���������� ǥ��
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}