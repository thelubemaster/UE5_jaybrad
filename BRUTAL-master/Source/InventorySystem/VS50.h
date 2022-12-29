// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "VS50.generated.h"

class UNiagaraSystem;
class USphereComponent;
UCLASS()
class INVENTORYSYSTEM_API AVS50 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVS50();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* TriggerSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* GroundSmoke;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* GroundSparks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* GroundUpSparks;
	UFUNCTION(NetMulticast, reliable)
	void ExplosionFX();
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
