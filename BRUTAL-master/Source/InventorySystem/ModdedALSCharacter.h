// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Character/ALSCharacter.h"
#include "Item/BasicItemDataAsset.h"
#include "ModdedALSCharacter.generated.h"

class UInventoryWidget;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class ABaseItemActor;
class UItemNameDisplayer;
class UBasicItemDataAsset;


/**
 * 
 */
UCLASS(Blueprintable)
class INVENTORYSYSTEM_API AModdedALSCharacter : public AALSCharacter
{
	GENERATED_BODY()
	
public:
	//Sets Defualts
	AModdedALSCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UItemNameDisplayer> ItemNameWidgetClass = nullptr;

	UPROPERTY()
	UItemNameDisplayer* ItemName = nullptr;

	bool isinterracting;


	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	float CameraDistance = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spectatorpawn")
	class UClass* TheSpectatorPawn = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SwitchToWorldSpectator();

	void ToggleInventory();

	void DropItem(UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid);
	UFUNCTION(Server, reliable)
	void DropItemOnServer(const FName& Name,UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid);
	UFUNCTION(Server, reliable)
	void DropItemNetMultiCastData(const FName& Name,UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid);

	// Create Event
	UFUNCTION(BlueprintNativeEvent, Category = "OpenOtherPlayersInventory")
	void OpenInventoryOfThisCharacter(AModdedALSCharacter* player);
	void PressingOpenInventory();
	void ReleaseOpenInventory();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryWidget* OtherPlayersInventoryWidget = nullptr;
	UFUNCTION(Server, reliable)
	void MoveItemOnServer(UBasicItemDataAsset* DropItemData,UDynamicInventoryGrid* GridPanel, int pIndex, int sIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveOtherPlayersInventoryWidget();

	UFUNCTION(Server, reliable)
	void DropItemOnServerData();

	FHitResult Hit;

	bool bIsInventoryActive = false;
	bool bIsPressPickUp = false;
	bool bIsPressLoot = false;

	void bIsPressLooting(int input);
	void EnableUIMode(const bool IsEnable);
	void LineTrace();
	void InteractwithVehicle();
	void PressPickUp();
	UFUNCTION(Server, reliable)
	void DeleteOnServerOnPressPickUp(ABaseItemActor* ItemActor);
	UFUNCTION(NetMulticast, reliable)
	void AddOnAllClientsOnPressPickUp(ABaseItemActor* ItemActor);
	void PossessCar(AWheeledVehiclePawn* car);
	void ReleasePickUp();
	static FORCEINLINE FName GetObjPath(const UObject* Obj)
	{
		if (!Obj)
		{
			return NAME_None;
		}
		//~

		FSoftObjectPath ThePath = FSoftObjectPath(Obj);

		if (!ThePath.IsValid()) return NAME_None;

		//The Class FString Name For This Object
		FString Str = Obj->GetClass()->GetDescription();

		Str += "'";
		Str += ThePath.ToString();
		Str += "'";

		return FName(*Str);
	}
	//TEMPLATE Load Obj From Path
	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}
	static FORCEINLINE UBasicItemDataAsset* LoadDAFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return LoadObjFromPath<UBasicItemDataAsset>(Path);
	}
};
