// Fill out your copyright notice in the Description page of Project Settings.


#include "UISlot.h"

void UUISlot::NativeConstruct()
{
	Super::NativeConstruct();
	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
}