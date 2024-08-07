// Fill out your copyright notice in the Description page of Project Settings.


#include "PAItem.h"
#include "PACharacter.h"
#include "PAPlayerController.h"
#include "PAGameInstance.h"
#include "PAItemSetting.h"
#include "Components/WidgetComponent.h"
#include "UW_ItemName.h"

// Sets default values
APAItem::APAItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	ItemNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ITEMNAMEWIDGET"));

	RootComponent = Trigger;
	Item->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);
	ItemNameWidget->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	int determindKind = FMath::RandRange(0, 100);
	//TODO: 확률에 따라 eKind 정하기
	if (determindKind <= 40) {
		eItemKind = EItemKind::IKE_NONE;
		amount = 0;
		return;
	}//None: 아무것도 스폰 안함
	else if (determindKind <= 80)
	{
		//실링
		eItemKind = EItemKind::IKE_SHILLING;
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHILLINGS(TEXT("/Game/ProjectArkContents/Items/StaticMeshs/Shillings"));
		if (SM_SHILLINGS.Succeeded())
		{
			Item->SetStaticMesh(SM_SHILLINGS.Object);
		}
		amount = FMath::RandRange(10, 100);
	}
	else if (determindKind <= 95)// 물약
	{
		eItemKind = EItemKind::IKE_POTION;
	}
	else // 수집품
	{
		eItemKind = EItemKind::IKE_COLLECTABLE;
	}// 이외 ItemKind는 상인에게서 구입하거나, 처음에 지급된 상태로 처리(스폰 X)


	/*
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}
	*/

	ItemNameWidget->SetRelativeLocation(FVector::Zero());
	ItemNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_ITEMNAME(TEXT("/Game/ProjectArkContents/UI/WB_ItemName.WB_ItemName_C"));
	if (UI_ITEMNAME.Succeeded())
	{
		ItemNameWidget->SetWidgetClass(UI_ITEMNAME.Class);
		ItemNameWidget->SetDrawSize(FVector2D(120.0f, 50.0f));
	}

	Item->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemTrigger"));
	Item->SetCollisionProfileName(TEXT("ItemMesh"));

	Item->bIgnoreRadialImpulse = true;
	Item->bIgnoreRadialForce = true;
	Item->SetCanEverAffectNavigation(false);
	Item->SetSimulatePhysics(true);
	Item->BodyInstance.bLockXRotation = true;
	Item->BodyInstance.bLockYRotation = true;
	Item->BodyInstance.bLockZRotation = true;
}

// Called when the game starts or when spawned
void APAItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (eItemKind !=EItemKind::IKE_SHILLING && eItemKind!=EItemKind::IKE_NONE)
	{
		if (eItemKind == EItemKind::IKE_POTION || eItemKind == EItemKind::IKE_COLLECTABLE)
		{
			auto PAGameInstance = Cast<UPAGameInstance>(GetGameInstance());
			PACHECK(nullptr != PAGameInstance);

			TPair<FString, int32> ChoosedID = PAGameInstance->ChooseItemID(eItemKind);
			
			auto DefaultSetting = GetDefault<UPAItemSetting>();
			if (eItemKind == EItemKind::IKE_POTION)
			{
				ItemAssetToLoad = DefaultSetting->PotionAssets[ChoosedID.Value];
			}
			else // collectable
			{
				ItemAssetToLoad = DefaultSetting->CollectableAssets[ChoosedID.Value];
			}

			
			AssetStreamingHandle = PAGameInstance->Streamablemanager.RequestAsyncLoad(ItemAssetToLoad, 
				FStreamableDelegate::CreateUObject(this, &APAItem::OnAssetLoadCompleted));
			
			ItemData = PAGameInstance->GetPAItemData(eItemKind, ChoosedID.Key);
		}
	}

	if (eItemKind != EItemKind::IKE_NONE)
	{
		Item->AddImpulse(FVector(FMath::RandRange(5.f, 10.f), FMath::RandRange(5.f, 10.f), Item->GetMass()), NAME_None, true);

		auto NameWidget = Cast<UUW_ItemName>(ItemNameWidget->GetUserWidgetObject());
		if (nullptr != NameWidget)
		{
			if (eItemKind == EItemKind::IKE_SHILLING)
			{
				std::string ShillingName = "실링";
				NameWidget->SetItemName(UTF8_TO_TCHAR(ShillingName.c_str()));
			}
			else
			{
				NameWidget->SetItemName(ItemData->itemName);
			}
		}

		Trigger->OnComponentBeginOverlap.AddDynamic(this, &APAItem::OnPlayerOverlap);
	}
}

void APAItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APAItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PALOG_S(Warning); // temporary, 확인용

	auto PACharacter = Cast<APACharacter>(OtherActor);
	PACHECK(nullptr != PACharacter);

	if (PACharacter->IsPlayerControlled() == false)
	{
		return;
	}


	auto PAPlayerController = Cast<APAPlayerController>(PACharacter->GetController());
	PACHECK(nullptr != PAPlayerController);

	if (eItemKind != EItemKind::IKE_NONE)
	{
		ItemNameWidget->RemoveFromRoot();
		ItemNameWidget->DestroyComponent();
		Item->SetHiddenInGame(true, true);
		SetActorEnableCollision(false);
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		Item->SetCollisionProfileName(TEXT("NoCollision"));
		PAPlayerController->AddItem(this);
	}
	
}


// Called every frame
void APAItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APAItem::OnAssetLoadCompleted()
{
	UStaticMesh* AssetLoaded = Cast<UStaticMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	if (nullptr != AssetLoaded)
	{
		Item->SetStaticMesh(AssetLoaded);
	}
	else { PALOG(Warning, TEXT("StaticMesh is not loaded!: %s"), *ItemAssetToLoad.ToString()); }
}



