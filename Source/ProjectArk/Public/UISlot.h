// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UISlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
enum ESlotType;

/*
enum ESlotType
{
	SLOT_NONE UMETA(DisplayName = "None"),
	SLOT_ITEM UMETA(DisplayName = "Item"),
	SLOT_SKILL UMETA(DisplayName = "Skill"),
	SLOT_QUICK UMETA(DisplayName = "Quick"),
	SLOT_Q_ITEM UMETA(DisplayName = "Q_Item"),
	SLOT_Q_SKILL UMETA(DisplayName = "Q_Skill")
};
*/

/**
 * 
 */
UCLASS()
class PROJECTARK_API UUISlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Slotnum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Count;

//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	TEnumAsByte<ESlotType> Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Button;
};
