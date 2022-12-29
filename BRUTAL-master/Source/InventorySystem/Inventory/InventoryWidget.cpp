// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"


#include "DynamicInventoryGrid.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;
	
	SortButton->OnClicked.AddDynamic(this, &UInventoryWidget::UInventoryWidget::OnClickSortButton);
}

void UInventoryWidget::OnClickSortButton()
{
	InventoryGridPanel->SortItems();
}

void UInventoryWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UInventoryComponent, ItemContainer);
	//DOREPLIFETIME(UInventoryWidget, InventoryGridPanel);
	//DOREPLIFETIME(UInventoryWidget, InventoryHeadPanel);

}