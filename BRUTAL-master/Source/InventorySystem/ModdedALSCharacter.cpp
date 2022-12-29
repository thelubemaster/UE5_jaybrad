// Fill out your copyright notice in the Description page of Project Settings.


#include "ModdedALSCharacter.h"
#include "DrawDebugHelpers.h"
#include "../InventorySystem/Inventory/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Inventory/DynamicInventoryGrid.h"
#include "Inventory/InventoryItemDisplay.h"
#include "Item/BaseItemActor.h"
#include "Inventory/ItemNameDisplayer.h"
#include "Item/BasicItemDataAsset.h"
#include "WheeledVehiclePawn.h"
//#include "../../../../../Engine/Plugins/Experimental/ChaosVehiclesPlugin/Source/ChaosVehicles/Public/WheeledVehiclePawn.h"
//#include "../../../../../Plugins/ChaosVehiclesPlugin/Source/ChaosVehicles/Public/WheeledVehiclePawn.h"

//class AWheeledVehiclePawn;
AModdedALSCharacter::AModdedALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	isinterracting = false;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

}

void AModdedALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			ItemName = CreateWidget<UItemNameDisplayer>(PC, ItemNameWidgetClass);
			//ItemName->ind
			ItemName->AddToViewport();
			ItemName->SetVisibility(ESlateVisibility::Hidden);

			if (InventoryComponent)
			{
				//InventoryComponent->InventoryWidget = CreateWidget<UInventoryWidget>(PC, InventoryComponent->InventoryWidgetClass);
				//InventoryComponent->InventoryWidget->AddToViewport();
				//InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
				//InventoryComponent->InventoryWidget->InventoryGridPanel->SetInventoryComponent(InventoryComponent);
			}
		}
		//UGameplayStatics::GetPlayerController(GetWorld(),
	}
	if (InventoryComponent)
	{
		APlayerController* PC1 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		InventoryComponent->InventoryWidget = CreateWidget<UInventoryWidget>(PC1, InventoryComponent->InventoryWidgetClass);
		
		if (InventoryComponent->InventoryWidget)
		{
			InventoryComponent->InventoryWidget->AddToViewport();
			InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			InventoryComponent->InventoryWidget->InventoryGridPanel->SetInventoryComponent(InventoryComponent);
		}
	}
}

void AModdedALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTrace();
}

void AModdedALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("worldSpectatorPossessKey", IE_Pressed, this, &AModdedALSCharacter::SwitchToWorldSpectator);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AModdedALSCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AModdedALSCharacter::PressPickUp);
	PlayerInputComponent->BindAction("PickUp", IE_Released, this, &AModdedALSCharacter::ReleasePickUp);
	PlayerInputComponent->BindAction("Loot", IE_Pressed, this, &AModdedALSCharacter::PressingOpenInventory);
	PlayerInputComponent->BindAction("Loot", IE_Released, this, &AModdedALSCharacter::ReleaseOpenInventory);
	PlayerInputComponent->BindAction("Esc", IE_Pressed, this, &AModdedALSCharacter::RemoveOtherPlayersInventoryWidget);
}

void AModdedALSCharacter::SwitchToWorldSpectator()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot = this->GetViewRotation();
	FVector myLoc = this->GetPawnViewLocation();
	FMinimalViewInfo caminfo;
	this->CalcCamera(GetWorld()->GetDeltaSeconds(), caminfo);
	//caminfo.ro
	this->Controller->GetControlRotation();
	this->Controller->GetTargetLocation();
	this->GetInstigatorController()->GetActorEyesViewPoint(myLoc, myRot);
	this->GetActorEyesViewPoint(caminfo.Location, caminfo.Rotation);
	this->GetPawnViewLocation();
	//this->GetEye
	APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FTransform mytemphopethisworks = this->GetThirdPersonPivotTarget();
	UCameraComponent* FollowCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	
	auto myspecpawn = GetWorld()->SpawnActor<APawn>(TheSpectatorPawn, PlayerCamera->GetCameraLocation(), caminfo.Rotation, SpawnInfo);
	
	// save a copy of our controller
	AController* SavedController = GetController();
	// unpossess first ( helps with multiplayer )
	SavedController->UnPossess();
	
	// disable movement mode
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	SavedController->Possess(Cast<APawn>(myspecpawn));

	
	ItemName->SetTextName(FText::FromString(""));
}

void AModdedALSCharacter::ToggleInventory()
{
	if (InventoryComponent->InventoryWidget)
	{
		if (bIsInventoryActive)
		{
			InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			bIsInventoryActive = false;
		}
		else
		{
			InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			bIsInventoryActive = true;
		}
		EnableUIMode(bIsInventoryActive);

	}
}

void AModdedALSCharacter::DropItem(UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid)
{
	UE_LOG(LogTemp, Warning, TEXT("DropItem(UBasicItemDataAsset* DropItemData)   %d"), DropItemData->IsValidLowLevel());
	
	FString PathName = DropItemData->GetPathName();
	GetObjPath(DropItemData);
	UE_LOG(LogTemp, Warning, TEXT("FString PathName = DropItemData->GetPathName() = %s"), *PathName);
	InventoryComponent->ReturnThisItem(DropItemData);
	UE_LOG(LogTemp, Warning, TEXT("FString PathName = DropItemData->GetPathName() = %s"), *PathName);
	UE_LOG(LogTemp, Warning, TEXT("DropItemData->GetName()  %s HasAuthority() = %d"), *GetObjPath(DropItemData).ToString(), HasAuthority());
	DropItemData->IsSupportedForNetworking();
	
	UE_LOG(LogTemp, Warning, TEXT("DropItemData->IsSupportedForNetworking(); = %d"), DropItemData->IsSupportedForNetworking());
	DropItemOnServer(GetObjPath(DropItemData),DropItemData,Grid);
	
}

void AModdedALSCharacter::DropItemOnServer_Implementation(const FName& Name, UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid)
{
	DropItemData = LoadDAFromPath(Name);
	//DropItemOnServerData_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("NAME    ===   %s"), *Name.ToString());
	UE_LOG(LogTemp, Warning, TEXT("DropItemOnServer(UBasicItemDataAsset* DropItemData) DropItemData->IsValidLowLevel() = %d  HasAuthority() == %d"), DropItemData->IsValidLowLevel(), HasAuthority());
	DropItemNetMultiCastData(Name,DropItemData,Grid);
	this->GetName();
	this->InventoryComponent->PrintItemContainter_Implementation();
}
void AModdedALSCharacter::DropItemOnServerData_Implementation()//not used remove
{
	this->InventoryComponent->PrintItemContainter_Implementation();
	TArray<const UBasicItemDataAsset*> ItemContainer;
	UE_LOG(LogTemp, Warning, TEXT("this->InventoryComponent->ItemContainer.Num();= %d HasAthority() = %d "), this->InventoryComponent->ItemContainer.Num(), HasAuthority());
	UE_LOG(LogTemp, Warning, TEXT("this->InventoryComponent->PrintItemContainter_Implementation(); this->GetName();= %s, HasAuthority() = %d"), *this->GetName(), HasAuthority());
}
void AModdedALSCharacter::DropItemNetMultiCastData_Implementation(const FName& Name, UBasicItemDataAsset* DropItemData, UDynamicInventoryGrid* Grid)
{
	DropItemData = LoadDAFromPath(Name);
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridNULLLLLLLLL %d"), HasAuthority() );
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Grid %d"), HasAuthority());
	}
	if (!DropItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItemDataNULLLLLLLLL %d"), HasAuthority());
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItemData %d"), HasAuthority());
	}
	FVector Location = (GetActorForwardVector() * 200.f) + GetActorLocation();
	Location.Z = 150;

	const FRotator Rotate = FRotator(0, 0, 0);
	const FActorSpawnParameters SpawnInfo;
	//DropItemData = <UBasicItemDataAsset>LoadObject(Name);
	UE_LOG(LogTemp, Warning, TEXT("NNNNAMMMMMMEEEE    ===   %s"), *Name.ToString());

	const auto DroppedItem = GetWorld()->SpawnActor<ABaseItemActor>(ABaseItemActor::StaticClass(), Location, Rotate, SpawnInfo);
	DroppedItem->ItemData = const_cast<UBasicItemDataAsset*>(DropItemData);
	DroppedItem->InitItemActor(const_cast<UBasicItemDataAsset*>(DropItemData), Name);
	DroppedItem->SetReplicateMovement(true);
	DroppedItem->SetReplicates(true);
	//DroppedItem->ItemData->
	DroppedItem->MeshComponent->SetIsReplicated(true);

	InventoryComponent->RemoveItem(const_cast<UBasicItemDataAsset*>(DropItemData));
	UE_LOG(LogTemp, Warning, TEXT("DropItemNetMultiCastData(UBasicItemDataAsset* DropItemData)DropItemData->IsValidLowLevel() = %d  HasAuthority() == %d"), DropItemData->IsValidLowLevel(), HasAuthority());

}

void AModdedALSCharacter::bIsPressLooting(int input)
{

}
void AModdedALSCharacter::PressingOpenInventory()
{
	bIsPressLoot = true;
}
void AModdedALSCharacter::ReleaseOpenInventory()
{
	bIsPressLoot = false;
}

void AModdedALSCharacter::EnableUIMode(const bool IsEnable)
{
	if (APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GWorld))
	{
		if (IsEnable)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, InventoryComponent->InventoryWidget);
			PlayerController->bShowMouseCursor = true;
		}
		else
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerController->bShowMouseCursor = false;
		}
	}
}

void AModdedALSCharacter::LineTrace()
{
	if (IsLocallyControlled() && !bIsInventoryActive)
	{
		FVector Loc;
		FRotator Rot;

		GetController()->GetPlayerViewPoint(Loc, Rot);

		const FVector Start = Loc;
		const FVector End = Start + (Rot.Vector() * TraceDistance);

		const FCollisionQueryParams TraceParams;
		
		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
		
		if (bHit && Hit.GetActor())
		{
			//Hit.GetActor()->GetName()
			//UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->GetName());
			if (ABaseItemActor* ItemActor = Cast<ABaseItemActor>(Hit.GetActor()))
			{
				// DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 2.f,FColor::Yellow, false, 2.f);
				if (ItemActor == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("if (ItemActor == nullptr)"),)
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("HHHHHHHHHHHHHHHHHHHHHHHHHHHH"), )
				}
				if (ItemActor->ItemData )
				{
					//ItemName->SetTextName(ItemActor->ItemData->GetItemName());

					if (bIsPressPickUp && ItemActor)
					{
						//if (InventoryComponent->PickUpItem(ItemActor->ItemData))
						//{
						//UE_LOG(LogTemp, Warning, TEXT("DeleteOnServerOnPressPickUp_Implementation(ABaseItemActor* ItemActor) *ItemActor->GetName() = %s, HasAuthority() = %d"), );
						DeleteOnServerOnPressPickUp(ItemActor);

						isinterracting = true;
						//}
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Item Actor Item Data is Null"), )
				}
			}
			else
			{
				//ItemName->SetTextName(FText::FromString(""));
			}
			if (AModdedALSCharacter* player = Cast<AModdedALSCharacter>(Hit.GetActor()))
			{
				//ItemName->SetTextName(FText::FromString(player->GetName()));

			}
		}
		const bool bHit2 = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PhysicsBody, TraceParams);
		if (bHit2 && Hit.GetActor())
		{
			//UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->GetName());
			//Hit.GetActor()->GetName()
			if ((Cast<AWheeledVehiclePawn>(Hit.GetActor())) && bIsPressPickUp)
			{
				//UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->GetName());
				PossessCar(Cast<AWheeledVehiclePawn>(Hit.GetActor()));
			}
			if (bIsPressPickUp)
			{
				if (Hit.GetActor() )
				{
					UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->GetClass()->GetName());
					//UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->IsA(AWheeledVehiclePawn));
					
				}
			}
			if(!Cast<AModdedALSCharacter>(Hit.GetActor()))
			ItemName->SetTextName(FText::FromString(*Hit.GetActor()->GetName()));
			if (bIsPressLoot)
			{
				if (Cast<AModdedALSCharacter>(Hit.GetActor()))
				{
					OpenInventoryOfThisCharacter(Cast<AModdedALSCharacter>(Hit.GetActor()));
					UE_LOG(LogTemp, Error, TEXT("Hit.GetActor()->GetName() %s"), *Hit.GetActor()->GetName());
					AModdedALSCharacter* otherplayer = Cast<AModdedALSCharacter>(Hit.GetActor());
					otherplayer->InventoryComponent->InventoryWidget;
					if (otherplayer->InventoryComponent->InventoryWidget == nullptr)
					{
						UE_LOG(LogTemp, Error, TEXT("otherplayer->InventoryComponent->InventoryWidget == nullptr"), );
					}
					else
					//this->InventoryComponent->InventoryWidget = otherplayer->InventoryComponent->InventoryWidget;
					if (!bIsInventoryActive)
					{
						OtherPlayersInventoryWidget = otherplayer->InventoryComponent->InventoryWidget;
						otherplayer->InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
						if(ItemName)
						ItemName->SetVisibility(ESlateVisibility::Hidden);
						//otherplayer->InventoryComponent->InventoryWidget->index
						//otherplayer->InventoryComponent->InventoryWidget->SetFocus();
						bIsInventoryActive = true;
						EnableUIMode(bIsInventoryActive);
					}
				}
			}
		}
	}

}
void AModdedALSCharacter::InteractwithVehicle()
{


}
void AModdedALSCharacter::RemoveOtherPlayersInventoryWidget()
{
	if (OtherPlayersInventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("AModdedALSCharacter::RemoveOtherPlayersInventoryWidget()"), );
		OtherPlayersInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bIsInventoryActive = false;
		EnableUIMode(bIsInventoryActive);
	}
	if (ItemName)
	ItemName->SetVisibility(ESlateVisibility::Visible);
	if(InventoryComponent)
	if (InventoryComponent->InventoryWidget)
	{
		if (InventoryComponent->InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
			InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}

}
void AModdedALSCharacter::OpenInventoryOfThisCharacter_Implementation(AModdedALSCharacter* player)
{

}
void AModdedALSCharacter::PressPickUp()
{
	bIsPressPickUp = true;
}

void AModdedALSCharacter::MoveItemOnServer_Implementation(UBasicItemDataAsset* DropItemData,UDynamicInventoryGrid* GridPanel, int pIndex, int sIndex)
{


	UE_LOG(LogTemp, Error, TEXT("AModdedALSCharacter::MoveItemOnServer_Implementation()"), );
	if (DropItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("if (DropItemData)"), );
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("else ! (DropItemData)"), );
	}
	if (GridPanel)
	{
		UE_LOG(LogTemp, Error, TEXT("if (GridPanel) %s"), *GridPanel->GetName());
		UE_LOG(LogTemp, Error, TEXT("if (GridPanel) = %s"), *GridPanel->GetRootWidget()->GetName());
		//UE_LOG(LogTemp, Error, TEXT("if (GridPanel) = %s"), *GridPanel->Wi);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GRIDDPANNEL ======NUUUULLLLPTRR"), );
	}
	if (GridPanel)
	{
		UInventoryItemDisplay* zItem = GridPanel->ItemDisplayContainer.FindRef(pIndex);
		if (zItem)
		{
			//zItem->SetInventoryIndex(sIndex);
			UE_LOG(LogTemp, Error, TEXT("*zItem->GetName()AAAAAAAAAA %s"), *zItem->GetName());
			//zItem->
			//FIntPoint coord = GridPanel->GetCoordinateByIndex(pIndex);
			//FIntPoint ncoord = GridPanel->GetCoordinateByIndex(sIndex);
			//GridPanel->ClearSlots(coord, zItem->GetItemSize());
			//GridPanel->FillSlots(ncoord, zItem->GetItemSize());
			//GridPanel->RemoveItem(zItem->ItemData);
			//zItem->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("zItem =  nullptr"), );
		}
	}
}

void AModdedALSCharacter::DeleteOnServerOnPressPickUp_Implementation(ABaseItemActor* ItemActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("DeleteOnServerOnPressPickUp_Implementation(ABaseItemActor* ItemActor) *ItemActor->GetName() = %s, HasAuthority() = %d"), );
	if (ItemActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeleteOnServerOnPressPickUp_Implementation(ABaseItemActor* ItemActor) *ItemActor->GetName() = %s, HasAuthority() = %d"), *ItemActor->GetName(), HasAuthority());
		AddOnAllClientsOnPressPickUp(ItemActor);
		//ItemActor->Destroy();
	}
	else
	{

		//UE_LOG(LogTemp, Error, TEXT("DeleteOnServerOnPressPickUp_Implementation(ABaseItemActor* ItemActor) *ItemActor->GetName() = %s, HasAuthority() = %d"),);
	}
	//if(ItemActor)
	//ItemActor->Destroy();
}

void AModdedALSCharacter::AddOnAllClientsOnPressPickUp_Implementation(ABaseItemActor* ItemActor)
{
	InventoryComponent->PickUpItem(ItemActor->ItemData);
	UE_LOG(LogTemp, Warning, TEXT("AddOnAllClientsOnPressPickUp_Implementation(ABaseItemActor* ItemActor) *ItemActor->GetName() = %s, HasAuthority() = %d"), *ItemActor->GetName(), HasAuthority());

	ItemActor->Destroy();
	isinterracting = false;
}

void AModdedALSCharacter::PossessCar(AWheeledVehiclePawn* car)
{
	// save a copy of our controller
	AController* SavedController = GetController();
	// unpossess first ( helps with multiplayer )
	SavedController->UnPossess();

	// disable movement mode
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SavedController;
	SavedController->Possess(Cast<AWheeledVehiclePawn>(car));
	UE_LOG(LogTemp, Warning, TEXT("Possessing car"));
}

void AModdedALSCharacter::ReleasePickUp()
{
	bIsPressPickUp = false;
}
