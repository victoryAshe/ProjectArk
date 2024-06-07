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


FPAItemData* UPAGameInstance::GetPAItemData(EItemKind eKind, int32 ItemID)
{
	
	switch (eKind)
	{
	case EItemKind::IKE_COOKING:
		return PACookingItemTable->FindRow<FPACookingData>(*FString::FromInt(ItemID), TEXT(""));
		break;
	case EItemKind::IKE_CONSUMABLE :
		return PAConsumableItemTable->FindRow<FPAConsumableData>(*FString::FromInt(ItemID), TEXT(""));
		break;
	case EItemKind::IKE_POTION:
		return PAPotionItemTable->FindRow<FPAPotionData>(*FString::FromInt(ItemID), TEXT(""));
		break;
	case EItemKind::IKE_COLLECTABLE:
		return PACollectableItemTable->FindRow<FPACollectableData>(*FString::FromInt(ItemID), TEXT(""));
		break;
	default:
		PALOG(Warning, TEXT("정의되지 않은 EItemKind입니다: %s"), eKind);
		return nullptr;
		break;
	}
}