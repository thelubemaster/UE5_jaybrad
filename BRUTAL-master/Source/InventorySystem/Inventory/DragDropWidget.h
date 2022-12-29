// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDropWidget.generated.h"
class UDynamicInventoryGrid;
class UBasicItemDataAsset;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDragDropWidget : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UDragDropWidget();
	
	UDynamicInventoryGrid* OriginalOwner;
	UBasicItemDataAsset* Data;
	UPROPERTY()	
	UUserWidget* WidgetToDrag = nullptr;
	
	FVector2D MouseOffset = FVector2D::ZeroVector;
};
