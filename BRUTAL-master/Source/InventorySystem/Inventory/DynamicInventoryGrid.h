// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "DynamicInventoryGrid.generated.h"

class UGridPanel;
class UBorder;
class UBasicItemDataAsset;
class UInventoryItemDisplay;
class UGridSlot;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDynamicInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, Category="Settings",  meta = (ClampMin = "0", ClampMax = "50", UIMin = "0", UIMax = "50", ExpseOnSpawn = true))
	int ColumnCount = 10;

	UPROPERTY(EditAnywhere, Category="Settings",  meta = (ClampMin = "0", ClampMax = "50", UIMin = "0", UIMax = "50", ExpseOnSpawn = true))
	int RowCount = 10;

	UPROPERTY(EditAnywhere, Category="Settings",  meta = (ClampMin = "0", ClampMax = "50", UIMin = "0", UIMax = "50", ExpseOnSpawn = true))
	float TileSize = 25.f;

	UFUNCTION(BlueprintCallable)
	bool AddItem(UBasicItemDataAsset* ItemDataAsset);

	UFUNCTION(BlueprintCallable)
	bool AddItemAtIndex(UBasicItemDataAsset* ItemDataAsset,int Index);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UBasicItemDataAsset* ItemDataAsset);

	UFUNCTION(BlueprintCallable)
	void RemoveItematindex(UBasicItemDataAsset* ItemDataAsset,int32 index);

	UFUNCTION(BlueprintCallable)
	bool IsItemAvailableForSlot(const int Index, const FIntPoint& ItemSize) const;
		
	UFUNCTION(BlueprintCallable)
    int GetSlotIndexByCoordinate(const int Column, const int Row) const;
	
	UFUNCTION(BlueprintCallable)
	FIntPoint GetCoordinateByIndex(const int Index) const;

	void SortItems();

	void FillSlots(const FIntPoint& StartPoint, const FIntPoint& ItemSize);
	void ClearSlots(const FIntPoint& StartPoint, const FIntPoint& ItemSize);
	
	void SetInventoryComponent(UInventoryComponent* InvComp);
	
//protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UGridPanel* InventoryGridPanel = nullptr;

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UGridPanel* InventoryHeadPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* GridPanelBorder = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Class Settings")
	TSubclassOf<UInventoryItemDisplay> ItemDisplayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class Settings")
	TSubclassOf<UInventorySlot> InventorySlotClass;
	
//private:
	virtual void NativePreConstruct() override;
	
	void InitInventoryWidget();
	
	int GetFirstAvailableSlotIndex(const FIntPoint& ItemSize) const;

	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;
	


	TMap<int32,UInventoryItemDisplay*> ItemDisplayContainer;
	TArray<UInventorySlot*> Slots;
	TMap<UInventorySlot*, bool> SlotMap;
};
