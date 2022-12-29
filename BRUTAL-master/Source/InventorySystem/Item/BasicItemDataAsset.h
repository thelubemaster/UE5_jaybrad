// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BasicItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UBasicItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UBasicItemDataAsset();

	UFUNCTION(BlueprintCallable)
    int GetId() const  
	{
		return Id;
	}

	UFUNCTION(BlueprintCallable)
	const FText& GetItemName() const
	{
		return ItemName;
	};

	UFUNCTION(BlueprintCallable)
	const FString& GetDescription() const
	{
		return Description;
	};

	UFUNCTION(BlueprintCallable)
    UTexture2D* GetThumbnail() const
	{
		return ItemIcon;
	};

	UFUNCTION(BlueprintCallable)
    UStaticMesh* GetMeshComponent() const
	{
		return ItemMesh;	
	};

	UFUNCTION(BlueprintCallable)
	FIntPoint GetItemSize() const
	{
		return ItemSize;	
	};

	UFUNCTION(BlueprintCallable)
	bool IsStackable() const
	{
		return CanStackable;
	};

	UFUNCTION(BlueprintCallable)
	int GetStackSize() const
	{
		return StackSize; 
	}

	UFUNCTION(BlueprintCallable)
	int AddStack(const int Size)
	{
		StackSize += Size;
		return StackSize;
	}


	UPROPERTY(EditAnywhere)
	double MeshSizeMultiplier = 1;

	UPROPERTY(EditAnywhere)
	FString Type = "General";

	UPROPERTY(VisibleAnywhere)
	int Id = GetUniqueID();
	
	UPROPERTY(EditAnywhere)
	FText ItemName;

	UPROPERTY(EditAnywhere)
	FString Description;

	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "6", UIMin = "1", UIMax = "6"))
	FIntPoint ItemSize = {1,1};

	UPROPERTY(EditAnywhere)
	bool CanStackable = false;

	UPROPERTY(EditAnywhere)
	int StackSize = 1;

};
