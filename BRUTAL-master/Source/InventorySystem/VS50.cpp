// Fill out your copyright notice in the Description page of Project Settings.

#include "VS50.h"
#include "ModdedALSCharacter.h"
#include "Components/SphereComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
AVS50::AVS50()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	//TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	//RootComponent = TriggerSphere;
	SetRootComponent(Mesh);
	//Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh->SetupAttachment(TriggerSphere);
	//GroundSmoke = CreateDefaultSubobject<UNiagaraSystem>(TEXT("GroundSmoke"));
	TriggerSphere->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	//TriggerSphere->SetupAttachment(Mesh);
	//Mesh->SetupAttachment(RootComponent);
	//Mesh->AttachTo(RootComponent);
	//Mesh->AttachToComponent(RootComponent);
	//TriggerSphere->SetupAttachment(RootComponent);
	// declare overlap events
	//Mesh->SetupAttachment(TriggerSphere);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AVS50::OnOverlapBegin);
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVS50::ExplosionFX_Implementation()
{
	if (GroundSmoke && GroundSparks && GroundUpSparks)
	{
		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GroundSmoke, GetActorLocation(), GetActorRotation());
		UNiagaraComponent* effect1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GroundSparks, (GetActorLocation() + FVector(0, 0, -60)), GetActorRotation());
		UNiagaraComponent* effect2 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GroundUpSparks, GetActorLocation(), GetActorRotation());
		//Destroy();
	}
}

void AVS50::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Error, TEXT("OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,"));
	
	if (auto player = Cast<AModdedALSCharacter>(OtherActor))
	{
		player->RagdollStart();
	}
	if (HasAuthority())
	{
		
		//Mesh->AddRadialImpulse(GetActorLocation(), 999000.f, 999000.f, ERadialImpulseFalloff::RIF_Constant);
		Mesh->AddForce(FVector(0, 0, 1) * 9999);
		ExplosionFX();
		Destroy();

	}
}

// Called when the game starts or when spawned
void AVS50::BeginPlay()
{
	Super::BeginPlay();
	//SetRootComponent(Mesh);
	//RootComponent = TriggerSphere;
	//TriggerSphere->SetupAttachment(Mesh);
	//Mesh->SetRelativeLocation(FVector(0, 0, 0));
	//TriggerSphere->SetRelativeLocation(FVector(0, 0, 0));
	//Mesh->AttachToComponent(TriggerSphere, FAttachmentTransformRules::KeepRelativeTransform);
	//TriggerSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh->SetupAttachment(RootComponent);
	//TriggerSphere->SetRelativeLocation(FVector(0, 0, 0));
	//TriggerSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
}

// Called every frame
void AVS50::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

