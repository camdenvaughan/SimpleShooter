// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"


// Gives Ammo to player
void AAmmoPickup::GiveItemToPlayer()
{
    AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ammo Pickup could not grab Player Character"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Adding Ammo To Player"));
    PlayerCharacter->AddAmmo(AmmoAmount);
    
    Super::GiveItemToPlayer();
}
