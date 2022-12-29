// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UDynamicInventoryGrid;
class UButton;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,meta = (BindWidget),replicated)
	UDynamicInventoryGrid* InventoryGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget) , replicated)
	UDynamicInventoryGrid* InventoryHeadPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* RightArmGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* LeftArmGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* InventoryChestPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* Pocket0GridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* Pocket1GridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* Pocket2GridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* Pocket3GridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* LegsGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* RightFootGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* LeftFootGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* RightHandGridPanel = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget), replicated)
	UDynamicInventoryGrid* LeftHandGridPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* SortButton = nullptr;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	UFUNCTION()
	void OnClickSortButton();	

protected:
	
	virtual void NativeConstruct() override;

};
