// Fill out your copyright notice in the Description page of Project Settings.

#include "PAGameInstance.h"

UPAGameInstance::UPAGameInstance()
{
#pragma region Make DataTables
	//Get Cooking Data
	FString CookingDataPath = TEXT("/Game/ProjectArkContents/GameData/CookingItemDataTable");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PACOOKINGDATA(*CookingDataPath);
	PACHECK(DT_PACOOKINGDATA.Succeeded());
	PACookingItemTable = DT_PACOOKINGDATA.Object;
	PACHECK(PACookingItemTable->GetRowMap().Num() > 0);

	//Get Consumable Data
	FString ConsumableDataPath = TEXT("/Game/ProjectArkContents/GameData/ConsumableItemDataTable");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PACONSUMABLEDATA(*ConsumableDataPath);
	PACHECK(DT_PACONSUMABLEDATA.Succeeded());
	PAConsumableItemTable = DT_PACONSUMABLEDATA.Object;
	PACHECK(PAConsumableItemTable->GetRowMap().Num() > 0);

	//Get Potion Data
	FString PotionDataPath = TEXT("/Game/ProjectArkContents/GameData/PotionItemDataTable");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PAPOTIONDATA(*PotionDataPath);
	PACHECK(DT_PAPOTIONDATA.Succeeded());
	PAPotionItemTable = DT_PAPOTIONDATA.Object;
	PACHECK(PAPotionItemTable->GetRowMap().Num() > 0);

	//Get Collectable Data
	FString CollectableDataPath = TEXT("/Game/ProjectArkContents/GameData/CollectableItemDataTable");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COLLECTABLEDATA(*CollectableDataPath);
	PACHECK(DT_COLLECTABLEDATA.Succeeded());
	PACollectableItemTable = DT_COLLECTABLEDATA.Object;
	PACHECK(PACollectableItemTable->GetRowMap().Num() > 0);

	//TODO: Get Weapon Data
#pragma endregion

}

void UPAGameInstance::Init()
{
	Super::Init();
}

FString UPAGameInstance::ChooseItemID(EItemKind eKind)
{
	TArray<FName> ItemIDs;
	switch (eKind)
	{
	case EItemKind::IKE_COOKING:
		ItemIDs = PACookingItemTable->GetRowNames();
		break;
	case EItemKind::IKE_CONSUMABLE:
		ItemIDs = PAConsumableItemTable->GetRowNames();
		break;
	case EItemKind::IKE_POTION:
		ItemIDs = PAPotionItemTable->GetRowNames();
		break;
	case EItemKind::IKE_COLLECTABLE:
		ItemIDs = PACollectableItemTable->GetRowNames();
		break;
	default:
		PALOG(Warning, TEXT("정의되지 않은 EItemKind입니다: %s"), eKind);
		return "";
		break;
	}

	int randomIdx = FMath::RandRange(0, ItemIDs.Num() - 1);
	return ItemIDs[randomIdx].ToString();
}


FPAItemData* UPAGameInstance::GetPAItemData(EItemKind eKind, FString ItemID)
{
	if(ItemID.IsEmpty())
	{
		PALOG(Warning, TEXT("유효하지 않은 ItemID입니다."), NULL);
		return nullptr;
	}

	switch (eKind)
	{
	case EItemKind::IKE_COOKING:
		return PACookingItemTable->FindRow<FPACookingData>(*ItemID, TEXT(""));
		break;
	case EItemKind::IKE_CONSUMABLE :
		return PAConsumableItemTable->FindRow<FPAConsumableData>(*ItemID, TEXT(""));
		break;
	case EItemKind::IKE_POTION:
		return PAPotionItemTable->FindRow<FPAPotionData>(*ItemID, TEXT(""));
		break;
	case EItemKind::IKE_COLLECTABLE:
		return PACollectableItemTable->FindRow<FPACollectableData>(*ItemID, TEXT(""));
		break;
	default:
		PALOG(Warning, TEXT("정의되지 않은 EItemKind입니다: %s"), eKind);
		return nullptr;
		break;
	}
}