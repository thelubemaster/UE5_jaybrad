// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItemActor.h"

#include "BasicItemDataAsset.h"
#include "Components/BoxComponent.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
ABaseItemActor::ABaseItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = SceneComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//MeshComponent->SetupAttachment(SceneComponent);
	MeshComponent->SetupAttachment(RootComponent);
	//MeshComponent->SetRelativeLocation({0,0,0});
	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//BoxCollision->SetupAttachment(MeshComponent);
	//BoxCollision->SetRelativeLocation({0,0,0});
	//BoxCollision->bHiddenInGame = false;
	
}

void ABaseItemActor::InitItemActor(UBasicItemDataAsset* DropItemData)
{
	if(ItemData)
	{
		if(ItemData->GetMeshComponent())
		{
			UStaticMesh& SM = *ItemData->GetMeshComponent();
			
			MeshComponent->SetStaticMesh(&SM);
			MeshComponent->SetWorldScale3D(FVector(1.0, 1.0, 1.0)*DropItemData->MeshSizeMultiplier);
			//MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			// MeshComponent->SetRelativeLocation(MeshComponent->GetRelativeLocation());
			//BoxCollision->SetBoxExtent(SM.GetBounds().BoxExtent);
			//MeshComponent->SetupAttachment(RootComponent);
		
			//BoxCollision->SetRelativeLocation(FVector{0,0,SM.GetBounds().BoxExtent.Z});
			//MeshComponent->SetRelativeLocation({BoxCollision->GetRelativeLocation().X, BoxCollision->GetRelativeLocation().Y,
            //                                 BoxCollision->GetRelativeLocation().Z -SM.GetBounds().BoxExtent.Z });
		}
	}
}

void ABaseItemActor::InitItemActor(UBasicItemDataAsset* DropItemData, const FName& Name)
{
	if (ItemData)
	{
		if (ItemData->GetMeshComponent())
		{
			UStaticMesh& SM = *ItemData->GetMeshComponent();

			MeshComponent->SetStaticMesh(&SM);
			MeshComponent->SetWorldScale3D(FVector(1.0, 1.0, 1.0) * DropItemData->MeshSizeMultiplier);
			//MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			// MeshComponent->SetRelativeLocation(MeshComponent->GetRelativeLocation());
			//BoxCollision->SetBoxExtent(SM.GetBounds().BoxExtent);
			//MeshComponent->SetupAttachment(RootComponent);

			//BoxCollision->SetRelativeLocation(FVector{0,0,SM.GetBounds().BoxExtent.Z});
			//MeshComponent->SetRelativeLocation({BoxCollision->GetRelativeLocation().X, BoxCollision->GetRelativeLocation().Y,
			//                                 BoxCollision->GetRelativeLocation().Z -SM.GetBounds().BoxExtent.Z });
		}
	}
	else
	{
		ItemData = LoadDAFromPath(Name);
		if (ItemData->GetMeshComponent())
		{
			UStaticMesh& SM = *ItemData->GetMeshComponent();
			UE_LOG(LogTemp, Error, TEXT("ItemData = LoadDAFromPath(Name);   mData) %d HasAthority %d"), ItemData->IsValidLowLevel(), HasAuthority());
			MeshComponent->SetStaticMesh(&SM);
			MeshComponent->SetWorldScale3D(FVector(1.0, 1.0, 1.0) * ItemData->MeshSizeMultiplier);
			//MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			// MeshComponent->SetRelativeLocation(MeshComponent->GetRelativeLocation());
			//BoxCollision->SetBoxExtent(SM.GetBounds().BoxExtent);
			//MeshComponent->SetupAttachment(RootComponent);

			//BoxCollision->SetRelativeLocation(FVector{0,0,SM.GetBounds().BoxExtent.Z});
			//MeshComponent->SetRelativeLocation({BoxCollision->GetRelativeLocation().X, BoxCollision->GetRelativeLocation().Y,
			//BoxCollision->GetRelativeLocation().Z -SM.GetBounds().BoxExtent.Z });
		}
	}
	UE_LOG(LogTemp, Error, TEXT("if (ItemData) %d HasAthority %d"),ItemData->IsValidLowLevel(),HasAuthority());
	LoadDALClients(Name);
}

void ABaseItemActor::LoadDALClients_Implementation(const FName& Name)
{
	if (!ItemData)
	{
		ItemData = LoadDAFromPath(Name);
		if (ItemData->GetMeshComponent())
		{
			UStaticMesh& SM = *ItemData->GetMeshComponent();
			UE_LOG(LogTemp, Error, TEXT("ItemData = LoadDAFromPath(Name);   mData) %d HasAthority %d"), ItemData->IsValidLowLevel(), HasAuthority());
			MeshComponent->SetStaticMesh(&SM);
			MeshComponent->SetWorldScale3D(FVector(1.0, 1.0, 1.0) * ItemData->MeshSizeMultiplier);
			//MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			// MeshComponent->SetRelativeLocation(MeshComponent->GetRelativeLocation());
			//BoxCollision->SetBoxExtent(SM.GetBounds().BoxExtent);
			//MeshComponent->SetupAttachment(RootComponent);

			//BoxCollision->SetRelativeLocation(FVector{0,0,SM.GetBounds().BoxExtent.Z});
			//MeshComponent->SetRelativeLocation({BoxCollision->GetRelativeLocation().X, BoxCollision->GetRelativeLocation().Y,
			//BoxCollision->GetRelativeLocation().Z -SM.GetBounds().BoxExtent.Z });
		}
	}
}

// Called when the game starts or when spawned
void ABaseItemActor::BeginPlay()
{
	Super::BeginPlay();
	// InitItemActor();
	MeshComponent->SetSimulatePhysics(1);
}

void ABaseItemActor::OnConstruction(const FTransform& Transform)
{
	if(ItemData)
	InitItemActor(ItemData);
}

void ABaseItemActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseItemActor, ItemData);
	DOREPLIFETIME(ABaseItemActor, MeshComponent);

}