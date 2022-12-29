// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemDisplay.h"

#include "DragDropWidget.h"
#include "DynamicInventoryGrid.h"
#include "../Item/BasicItemDataAsset.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInventoryItemDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if(SlotBorder)
	{
		BorderInitialColor = SlotBorder->BrushColor;		
	}
}

void UInventoryItemDisplay::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(SlotBorder)
	{
		SlotBorder->SetBrushColor(FLinearColor{1, 1, 0, .5});
	}
}

void UInventoryItemDisplay::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if(SlotBorder)
	{
		SlotBorder->SetBrushColor(BorderInitialColor);
	}
}

FReply UInventoryItemDisplay::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this, FKey{"LeftMouseButton"});

	return Reply.NativeReply;
}

FReply UInventoryItemDisplay::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UInventoryItemDisplay::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                 UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("UInventoryItemDisplay::NativeOnDragDetected Owner %s"), *Owner->GetName());
	//Owner;
	UDragDropWidget* WidgetDrag = Cast<UDragDropWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropWidget::StaticClass()));

	if (WidgetDrag == nullptr)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	//since I got problem to use exist widget, I created clone Widget for visualize 
	UInventoryItemDisplay* DragVisual = CreateWidget<UInventoryItemDisplay>(PC, GetClass());
	DragVisual->Init(ItemData);
	
	WidgetDrag->WidgetToDrag = this;
	WidgetDrag->Payload = this;
	WidgetDrag->Data = ItemData;
	WidgetDrag->OriginalOwner = Owner;
	WidgetDrag->DefaultDragVisual = DragVisual;

	WidgetDrag->Pivot = EDragPivot::TopLeft;

	OutOperation = WidgetDrag;

	this->SetVisibility(ESlateVisibility::Hidden);

	if(Owner)
	{
		Owner->ClearSlots(Owner->GetCoordinateByIndex(SlotIndexOnInventory), ItemData->GetItemSize());
	}
}

void UInventoryItemDisplay::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	this->SetVisibility(ESlateVisibility::Visible);
	
	if(SlotBorder)
	{
		SlotBorder->SetBrushColor(BorderInitialColor);
	}

	if(Owner)
	{
		Owner->FillSlots(Owner->GetCoordinateByIndex(SlotIndexOnInventory), ItemData->GetItemSize());
	}
}

void UInventoryItemDisplay::Init(UBasicItemDataAsset* BasicItemData)
{
	ItemData = DuplicateObject<UBasicItemDataAsset>(BasicItemData,this);;

	if(ItemData)
	{
		ItemIcon->SetBrushFromTexture(ItemData->GetThumbnail());
		//todo replace tile size with global variable
		ItemIcon->Brush.SetImageSize(ItemData->GetItemSize() * 50);
		
		StackSizeTextBlock->SetText(FText::FromString(FString::Printf( TEXT( "x%d" ), ItemData->GetStackSize())));
		ItemCount = ItemData->GetStackSize();
	}
}

FIntPoint UInventoryItemDisplay::GetItemSize() const
{
	if(ItemData)
	{
		return ItemData->GetItemSize();
	}
	else
	{
		return FIntPoint::ZeroValue;
	}
}

void UInventoryItemDisplay::IncreaseCount(const int Value)
{
	ItemCount = ItemCount + Value;
	StackSizeTextBlock->SetText(FText::FromString(FString::Printf( TEXT( "x%d" ), ItemCount)));
}

inline void UInventoryItemDisplay::SetInventoryIndex(const int Index)
{
	UInventoryItemDisplay* t = nullptr;
	if (Owner != nullptr)
	{
		//this->Owner->ItemDisplayContainer.Find(SlotIndexOnInventory);
		//this->Owner->ItemDisplayContainer.RemoveAndCopyValue(SlotIndexOnInventory, t);
		UE_LOG(LogTemp, Warning, TEXT("(Owner  = %s"), *Owner->GetName());
		if (t)
		{
			//this->Owner->ItemDisplayContainer.Add(Index, t);
		}
		SlotIndexOnInventory = Index;
	}
	else
	{
		SlotIndexOnInventory = Index;
	}
}

inline void UInventoryItemDisplay::SetInventoryIndexAndChangeMap(const int Index)
{
	UInventoryItemDisplay* t = nullptr;
	if (Owner != nullptr)
	{
		this->Owner->ItemDisplayContainer.Find(SlotIndexOnInventory);
		this->Owner->ItemDisplayContainer.RemoveAndCopyValue(SlotIndexOnInventory, t);
		UE_LOG(LogTemp, Warning, TEXT("(Owner  = %s"), *Owner->GetName());
		if (t)
		{
			this->Owner->ItemDisplayContainer.Add(Index, t);
		}
		SlotIndexOnInventory = Index;
	}
	else
	{
		SlotIndexOnInventory = Index;
	}
}

void UInventoryItemDisplay::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryItemDisplay, ItemData);

}