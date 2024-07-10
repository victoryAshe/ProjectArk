// Fill out your copyright notice in the Description page of Project Settings.


#include "PAItem.h"
#include "ProjectArkCharacter.h"
#include "PAGameInstance.h"

// Sets default values
APAItem::APAItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Item->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);


	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	auto PAGameInstance = Cast<UPAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PACHECK(nullptr != PAGameInstance);

	int determindKind = FMath::RandRange(0, 100);
	//TODO: Ȯ���� ���� eKind ���ϱ�
	if (determindKind <= 40) {
		eItemKind = EItemKind::IKE_NONE;
		amount = 0;
		return;
	}//None: �ƹ��͵� ���� ����
	else if (determindKind <= 80)
	{
		//�Ǹ�
		eItemKind = EItemKind::IKE_SHILLING;
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHILLINGS(TEXT("/Game/ProjectArkContents/Items/StaticMeshs/Shillings"));
		if (SM_SHILLINGS.Succeeded())
		{
			Item->SetStaticMesh(SM_SHILLINGS.Object);
		}
		amount = FMath::RandRange(10, 100);
	}
	else
	{
		if (determindKind <= 95)// ����
		{
			eItemKind = EItemKind::IKE_POTION;
		}
		else // ����ǰ
		{
			eItemKind = EItemKind::IKE_COLLECTABLE;
		}
		FString itemID = PAGameInstance->ChooseItemID(eItemKind);
		ItemData = PAGameInstance->GetPAItemData(eItemKind, itemID);
	}	// �ܴ̿� ���ο��Լ� �����ϰų�, ó���� ���޵� ���·� ó��

	
	

	/*
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}
	*/

	Item->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("Item"));
	Item->SetCollisionProfileName(TEXT("NoCollision"));

	Trigger->bIgnoreRadialImpulse = true;
	Trigger->bIgnoreRadialForce = true;
	Trigger->SetCanEverAffectNavigation(false);
	Trigger->SetSimulatePhysics(true);
	Trigger->BodyInstance.bLockXRotation = true;
	Trigger->BodyInstance.bLockYRotation = true;
	Trigger->BodyInstance.bLockZRotation = true;
	Trigger->AddImpulse(FVector(FMath::RandRange(5.f,10.f), FMath::RandRange(5.f, 10.f), Trigger->GetMass()), NAME_None, true);
}

// Called when the game starts or when spawned
void APAItem::BeginPlay()
{
	Super::BeginPlay();

	if (eItemKind !=EItemKind::IKE_SHILLING && eItemKind!=EItemKind::IKE_NONE)
	{
	}
}

void APAItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}


// Called every frame
void APAItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



