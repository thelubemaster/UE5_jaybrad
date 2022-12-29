// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicItemDataAsset.h"
#include "GameFramework/Actor.h"
#include "BaseItemActor.generated.h"

class UBasicItemDataAsset;
class UStaticMesh;
class UBoxComponent;


UCLASS()
class INVENTORYSYSTEM_API ABaseItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItemActor();
	void InitItemActor(UBasicItemDataAsset* DropItemData);
	void InitItemActor(UBasicItemDataAsset* DropItemData, const FName& Name);
	UFUNCTION(NetMulticast, Reliable)
	void LoadDALClients(const FName& Name);
	
	UPROPERTY(EditAnywhere, Replicated)
	UBasicItemDataAsset* ItemData = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, Replicated)
	UStaticMeshComponent* MeshComponent = nullptr;

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* BoxCollision = nullptr;

	//UPROPERTY()
	//USceneComponent* SceneComponent = nullptr;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
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
