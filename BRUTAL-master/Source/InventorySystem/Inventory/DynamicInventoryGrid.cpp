// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicInventoryGrid.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "InventoryItemDisplay.h"
#include "Components/Image.h"
#include "InventorySlot.h"
#include "InventorySystem/Item/BasicItemDataAsset.h"

bool UDynamicInventoryGrid::AddItem(UBasicItemDataAsset* ItemDataAsset)
{
	const int Index = GetFirstAvailableSlotIndex(ItemDataAsset->GetItemSize());

	if(ItemDataAsset->IsStackable())
	{
		for(const auto ItemRef : ItemDisplayContainer)
		{
			if(ItemRef.Value->ItemData->GetId() == ItemDataAsset->GetId())
			{
				ItemRef.Value->ItemData->AddStack(ItemDataAsset->GetStackSize());
				ItemRef.Value->IncreaseCount();
				return true;
			}
		}
	}
	
	if(Index != -1)
	{
		UInventoryItemDisplay* InventoryItemDisplay = CreateWidget<UInventoryItemDisplay>(this, ItemDisplayClass);
		InventoryItemDisplay->Init(ItemDataAsset);
		InventoryItemDisplay->SetInventoryIndex(Index);
		InventoryItemDisplay->Owner = this;
		
		const FIntPoint ItemStartPoint = GetCoordinateByIndex(Index);
	
		InventoryGridPanel->AddChildToGrid(InventoryItemDisplay, ItemStartPoint.Y, ItemStartPoint.X);
		
		ItemDisplayContainer.Add(Index,InventoryItemDisplay);

		FillSlots(ItemStartPoint, ItemDataAsset->GetItemSize());
		
		return true;
	}

	return false;
}

bool UDynamicInventoryGrid::AddItemAtIndex(UBasicItemDataAsset* ItemDataAsset, int Index)
{
	//const int Index = GetFirstAvailableSlotIndex(ItemDataAsset->GetItemSize());

	if (ItemDataAsset->IsStackable())
	{
		for (const auto ItemRef : ItemDisplayContainer)
		{
			if (ItemRef.Value->ItemData->GetId() == ItemDataAsset->GetId())
			{
				ItemRef.Value->ItemData->AddStack(ItemDataAsset->GetStackSize());
				ItemRef.Value->IncreaseCount();
				return true;
			}
		}
	}

	if (Index != -1)
	{
		UInventoryItemDisplay* InventoryItemDisplay = CreateWidget<UInventoryItemDisplay>(this, ItemDisplayClass);
		InventoryItemDisplay->Init(ItemDataAsset);
		InventoryItemDisplay->SetInventoryIndex(Index);
		InventoryItemDisplay->Owner = this;

		const FIntPoint ItemStartPoint = GetCoordinateByIndex(Index);

		InventoryGridPanel->AddChildToGrid(InventoryItemDisplay, ItemStartPoint.Y, ItemStartPoint.X);

		ItemDisplayContainer.Add(Index,InventoryItemDisplay);

		FillSlots(ItemStartPoint, ItemDataAsset->GetItemSize());

		return true;
	}

	return false;
}
void UDynamicInventoryGrid::RemoveItem(UBasicItemDataAsset* ItemDataAsset)
{
	UInventoryItemDisplay* DeleteItem = nullptr;
	int32 DeleteKey = -1;
	
	for	(const auto ItemDisplay: ItemDisplayContainer)
	{
		if(ItemDataAsset == ItemDisplay.Value->ItemData)
		{
			//DeleteItem = ItemDisplay;
			DeleteKey = ItemDisplay.Key;
		}
	}

	if(DeleteItem)
	{
	
		ItemDisplayContainer.Remove(DeleteKey);
	}
}

void UDynamicInventoryGrid::RemoveItematindex(UBasicItemDataAsset* ItemDataAsset, int32 index)
{
	UInventoryItemDisplay* Itemtoremove = *ItemDisplayContainer.Find(index);
	if (Itemtoremove)
	{
		ItemDisplayContainer.Remove(index);
		UE_LOG(LogTemp, Error, TEXT("Successufully removedItem"));
	}
}

void UDynamicInventoryGrid::NativePreConstruct()
{
	Super::NativeConstruct();
	InitInventoryWidget();
	
}

void UDynamicInventoryGrid::InitInventoryWidget() 
{
	InventoryGridPanel->RowFill.Empty();
	InventoryGridPanel->ColumnFill.Empty();
	
	if(UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(GridPanelBorder->Slot))
	{

		CanvasPanelSlot->SetSize({TileSize * ColumnCount, TileSize * RowCount});
	}

	int count = 0;
	for (int Row = 0; Row < RowCount; Row++)
	{
		if(InventoryGridPanel)
		{
			InventoryGridPanel->RowFill.Add(Row);
			InventoryGridPanel->SetRowFill(Row, 1.f);
		}
		
		for (int Column = 0; Column < ColumnCount; Column++)
		{
			if(Row == 0)
			{
				if(InventoryGridPanel)
				{
					InventoryGridPanel->ColumnFill.Add(Column);
					InventoryGridPanel->SetColumnFill(Column, 1.f);
				}
			}

			UInventorySlot* InventorySlot = WidgetTree->ConstructWidget<UInventorySlot>(InventorySlotClass);
			InventorySlot->SetCoordinate(Row, Column);
			InventorySlot->SetPadding(FMargin{0, 0, 0, 0});
			InventorySlot->RenderTransformPivot = FVector2D{0,0};
			InventorySlot->Owner = this;
			InventorySlot->SetIndex(count);
			count++;
							
			UPanelSlot* PanelSlot = InventoryGridPanel->AddChild(InventorySlot);

			if(UGridSlot* GridSlot = Cast<UGridSlot>(PanelSlot))
			{
				GridSlot->SetColumn(Column);
				UE_LOG(LogTemp, Warning, TEXT("GridSlot->SetColumn(Column) %d"), Column);
				GridSlot->SetRow(Row);
				UE_LOG(LogTemp, Warning, TEXT("GridSlot->SetRow(Row); %d"), Row);
				GridSlot->SetColumnSpan(1);
				GridSlot->SetRowSpan(1);
			}
			
			Slots.Add(InventorySlot);
			SlotMap.Add(InventorySlot, false);
		}
	}
}

int UDynamicInventoryGrid::GetFirstAvailableSlotIndex(const FIntPoint& ItemSize) const
{
	int i = 0;
	
	for (const auto& SingleSlot : SlotMap)
	{
		if(IsItemAvailableForSlot(i, ItemSize))
		{
			return i;
		}
		
		i++;
	}
	
	return -1;
}

FIntPoint UDynamicInventoryGrid::GetCoordinateByIndex(const int Index) const
{
	if(Index >= 0 && Index < Slots.Num())
	{
		if(UInventorySlot* InvSlot = Cast<UInventorySlot>(Slots[Index]))
		{
			if(UGridSlot* GridSlot = Cast<UGridSlot>(InvSlot->Slot))
			{
				return FIntPoint{GridSlot->Column, GridSlot->Row};
			}	
		}
	}

	return FIntPoint{-1,-1};
}

void UDynamicInventoryGrid::SortItems()
{
	ClearSlots({0,0}, {RowCount, ColumnCount});
	
	//ItemDisplayContainer.Sort([=](const auto& Left, const auto& Right)
	//{
	//	return Left.GetTotalSlotForItem() > Right.GetTotalSlotForItem();
	//});
	
	for (const auto& Item : ItemDisplayContainer)
	{
		//const int Index = GetFirstAvailableSlotIndex(Item->ItemData->GetItemSize());
		//const FIntPoint ItemStartPoint = GetCoordinateByIndex(Index);

		//Item->SetInventoryIndex(Index);
		
		//InventoryGridPanel->AddChildToGrid(Item, ItemStartPoint.Y, ItemStartPoint.X);

		//FillSlots(ItemStartPoint, Item->GetItemSize());
	}
	
}

void UDynamicInventoryGrid::FillSlots(const FIntPoint& StartPoint, const FIntPoint& ItemSize)
{
	UE_LOG(LogTemp, Warning, TEXT(" StartPoint.X = %d , StartPoint.Y = %d"), StartPoint.X, StartPoint.Y);
	int Index = (StartPoint.Y * ColumnCount) + StartPoint.X;
	UE_LOG(LogTemp, Warning, TEXT("Jays Next Index %d"), Index);
	//UE_LOG(LogTemp, Warning, TEXT("Jays Next Index function %d"), Index + i + (j * (RowCount - 1)));
	for (int i = 0; i < ItemSize.X ; i++)
	{
		for (int j = 0; j < ItemSize.Y; j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Jays Next Index function %d"), Index + i + (j * (ColumnCount)));
			//UE_LOG(LogTemp, Warning, TEXT("GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount) %d"), GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount));
			UE_LOG(LogTemp, Warning, TEXT("(StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount) =  (%d,%d)"), (StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount);

			UE_LOG(LogTemp, Warning, TEXT("GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount) == %i"), GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount));
			//auto& SingleSlot = Slots[GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount)];

			auto& SingleSlot = Slots[Index + i + (j * (ColumnCount))];
			SlotMap.Add(SingleSlot, true);
			// SingleSlot->EnableFillorReFill(true);
		}
	}
}

void UDynamicInventoryGrid::ClearSlots(const FIntPoint& StartPoint, const FIntPoint& ItemSize)
{
	UE_LOG(LogTemp, Error, TEXT(" StartPoint.X = %d , StartPoint.Y = %d"), StartPoint.X, StartPoint.Y);
	//UE_LOG(LogTemp, Warning, TEXT("index %d"), Index);
	int Index = (StartPoint.Y * ColumnCount) + StartPoint.X;
	for (int i = 0; i < ItemSize.X ; i++)
	{
		for (int j = 0; j < ItemSize.Y; j++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount) %d"), GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount));
			//UE_LOG(LogTemp, Warning, TEXT("(StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount) =  (%d,%d)"), ((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount));
			//auto& SingleSlot = Slots[GetSlotIndexByCoordinate((StartPoint.X + i) % ColumnCount, (StartPoint.Y + j) % RowCount)];
			auto& SingleSlot = Slots[Index + i + (j * (ColumnCount))];
			SlotMap.Add(SingleSlot, false);
			// SingleSlot->EnableFillorReFill(false);			
		}
	}
}

void UDynamicInventoryGrid::SetInventoryComponent(UInventoryComponent* InvComp)
{
	InventoryComponent = InvComp;
}

int UDynamicInventoryGrid::GetSlotIndexByCoordinate(const int Column, const int Row) const
{
	UE_LOG(LogTemp, Warning, TEXT("UDynamicInventoryGrid::GetSlotIndexByCoordinate  const int Column  %d"), Column);
	UE_LOG(LogTemp, Warning, TEXT("UDynamicInventoryGrid::GetSlotIndexByCoordinate  const int Row %d"), Row);


	if(Column > ColumnCount || Row > RowCount )
	{
		return -1;
	}
	UE_LOG(LogTemp, Warning, TEXT("UDynamicInventoryGrid::GetSlotIndexByCoordinate %d"), (Column + (Row * RowCount)));
	return (Column + (Row * RowCount));
}

bool UDynamicInventoryGrid::IsItemAvailableForSlot(const int Index, const FIntPoint& ItemSize) const
{
	const auto Coordinate = GetCoordinateByIndex(Index);
	//UE_LOG(LogTemp, Warning, TEXT("index %d"), Index);
	//UE_LOG(LogTemp, Warning, TEXT("SlotMap.num %d"), SlotMap.Num());
	//UE_LOG(LogTemp, Warning, TEXT("Slots.num %d"), Slots.Num());

	//if (SlotMap.Num() < Index)
	//{
	//	return false;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("(Coordinate.X %d"), Coordinate.X);
	//UE_LOG(LogTemp, Warning, TEXT("(Coordinate.Y %d"), Coordinate.Y);
	const int ItemColumnSize = ItemSize.Y;
	const int ItemRowSize = ItemSize.X;

	//check neighbours
	for (int i = 0; i < ItemRowSize ; i++)
	{
		for (int j = 0; j < ItemColumnSize; j++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ColumnCount %d"), ColumnCount);
			//UE_LOG(LogTemp, Warning, TEXT("(Coordinate.X + i) ColumnCount %d"), (Coordinate.X + i) % ColumnCount);
			//UE_LOG(LogTemp, Warning, TEXT("(Coordinate.X %d"),  Coordinate.X  );
			//UE_LOG(LogTemp, Warning, TEXT("i %d"), i);
			//UE_LOG(LogTemp, Warning, TEXT("RowCount %d"), RowCount);
			//UE_LOG(LogTemp, Warning, TEXT("Coordinate.Y + j) % RowCount %d"), (Coordinate.Y + j) % RowCount);
			//UE_LOG(LogTemp, Warning, TEXT("GetSlotIndexByCoordinate %d"), GetSlotIndexByCoordinate((Coordinate.X + i) % ColumnCount, (Coordinate.Y + j) % RowCount));
			UE_LOG(LogTemp, Warning, TEXT("Jays Next Index function %d"), Index+i+(j*(RowCount-1)));
			//if(GetSlotIndexByCoordinate((Coordinate.X + i) % ColumnCount, (Coordinate.Y + j) % RowCount) < Slots.Num())
			//if (GetSlotIndexByCoordinate((Coordinate.X + i) % ColumnCount, (Coordinate.Y + j) % RowCount))
			//{
			//}
			//UE_LOG(LogTemp, Warning, TEXT("Coordinate.Y + i  =  %d"), Coordinate.Y + i);
			//UE_LOG(LogTemp, Warning, TEXT("Coordinate.X + j = %d"), Coordinate.X + j);
			if (Coordinate.Y + j >= RowCount || Coordinate.X + i >= ColumnCount)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Coordinate.Y + i >= RowCount || Coordinate.X + j >= ColumnCountGGGGGGGGGGGG %d ColumnCount" ), ColumnCount);
				return false;
			}
			//UE_LOG(LogTemp, Warning, TEXT("Coordinate.Y %d"), Coordinate.Y);
			//UE_LOG(LogTemp, Warning, TEXT("Coordinate.X %d"), Coordinate.X);
			//(j + StartPoint.Y) + ((i + StartPoint.X) * (RowCount - 1))
			//if (Index + j + (i * (RowCount - 1)) < Slots.Num())
			//	if (SlotMap[Slots[Index + j + (i * (RowCount - 1))]])

			if (Index + i + (j * (ColumnCount)) < Slots.Num())
			if(SlotMap[Slots[Index + i + (j * (ColumnCount))]])
			{
				//UE_LOG(LogTemp, Warning, TEXT("INVALID SLOOTS SlotMap[Slots[GetSlotIndexByCoord %d "), Index + j + (i * RowCount - 1));
				return false;
			}
			//if(Index + j + (i * (RowCount - 1)) < Slots.Num())
			//if(SlotMap[Slots[Index + j + (i * (RowCount - 1))]])
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("VALLLLIDDD SLOOTS"));
			//	return false;
			//}
		}
	}
	//if ((Coordinate.X + ItemSize.X > RowCount) || (Coordinate.Y + ItemSize.Y > ColumnCount))
	//(j + StartPoint.Y) + ((i + StartPoint.X) * (RowCount - 1))
	//if((j + StartPoint.Y) + ((i + StartPoint.X) * (RowCount - 1)))

	if((Coordinate.X + ItemSize.X  > RowCount) || (Coordinate.Y + ItemSize.Y  > ColumnCount))
	{
		//UE_LOG(LogTemp, Warning, TEXT("INVALID SLOOTS (Coordinate.X + ItemSize.X  > RowCount) || (Coordinate.Y + ItemSize.Y  > ColumnCount) %d"), Index);
		//return false;
	}

	return true;
}
