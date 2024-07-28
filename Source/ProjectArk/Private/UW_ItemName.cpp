// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ItemName.h"

void UUW_ItemName::NativeConstruct()
{
	Super::NativeConstruct();
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
}

void UUW_ItemName::SetItemName(FString ItemName)
{
	Text->SetText(FText::FromString(ItemName));
}