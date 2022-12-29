// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "../Item/BasicItemDataAsset.h"

#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "DynamicInventoryGrid.h"
#include "InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UInventoryComponent::PickUpItem(UBasicItemDataAsset* ItemData)
{
	if (ItemData == nullptr)
	return false;
	if(InventoryWidget)
	{
		if (InventoryWidget->InventoryHeadPanel->AddItem(ItemData))
		{
			PrintItemContainter();
			return true;
		}
		if(InventoryWidget->InventoryGridPanel->AddItem(ItemData))
		{
			PrintItemContainter();
			return true;
		}
	}
	PrintItemContainter();
	//HasAthority()
	//UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::PrintItemContainter()  %s"), HasAthority());
	return false;
}

void UInventoryComponent::RemoveItem(UBasicItemDataAsset* ItemData)
{
	ItemContainer.Remove(ItemData);

	if(InventoryWidget)
	{
		InventoryWidget->InventoryGridPanel->RemoveItem(ItemData);
	}
	PrintItemContainter();
}

void UInventoryComponent::RemoveItemfromowneratindex(UDynamicInventoryGrid* Owner, UBasicItemDataAsset* ItemData, int32 Index)
{
	ItemContainer.Remove(ItemData);

	if (Owner)
	{
		Owner->RemoveItem(ItemData);
	}
	PrintItemContainter();
}

const UBasicItemDataAsset* UInventoryComponent::ReturnThisItem(UBasicItemDataAsset* ItemData)
{
	for (auto Item : ItemContainer)
	{
		//Item->GetName()
		if (Item == ItemData)
		{
			return Item;
		}
		UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::ReturnThisItem *Item->GetName()  =  %s    *ItemData->GetName() = %s "), *Item->GetName(), *ItemData->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::ReturnThisItem return nullptr;"));
	return nullptr;
}

void UInventoryComponent::PrintItemContainterBlueprintCallable()
{
	PrintItemContainter_Implementation();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, ItemContainer);
}

void UInventoryComponent::PrintItemContainter_Implementation()
{

	UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::PrintItemContainter()*this->GetOwner()->GetName() %s, "), *this->GetOwner()->GetName())
	//UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::PrintItemContainter()*this->GetOwner()->GetName() %s, "), *this->GetOwner()->GetName(), Cast<AModdedALSCharacter>(this->GetOwner())->InventoryComponent->;
	for (auto Item : ItemContainer)
	{
		//Item->GetName()
		UE_LOG(LogTemp, Warning, TEXT(" UInventoryComponent::PrintItemContainter()  %s, *this->GetOwner()->GetName()  %s"), *Item->GetName(),*this->GetOwner()->GetName());
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
