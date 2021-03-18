// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"
#include "Components/BoxComponent.h"
#include "SimpleShooter/Controllers/ShooterPlayerController.h"

#define OUT

// Sets default values
APickUpBase::APickUpBase()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	PickupShape = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Shape"));
	PickupShape->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	PickupShape->SetGenerateOverlapEvents(true);
	PickupShape->OnComponentBeginOverlap.AddDynamic(this, &APickUpBase::OnPlayerEnterPickupBox);
	PickupShape->SetupAttachment(Mesh);
}

// Called when an Actor overlapps PickupShape
void APickUpBase::OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(OtherActor->GetInstigatorController());
	if (!PlayerController)
	{
		return;
	}
	GiveItemToPlayer();
}

// Function to be overridden by children
void APickUpBase::GiveItemToPlayer() 
{
	Destroy();
}
