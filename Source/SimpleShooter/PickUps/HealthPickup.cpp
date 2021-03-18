// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

// Gives Health to Player
void AHealthPickup::GiveItemToPlayer()
{
   AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
   if (!PlayerCharacter)
   {
       UE_LOG(LogTemp, Warning, TEXT("Health Pickup could not grab Player Character"));
       return;
   }
   if (PlayerCharacter->GetHealthPercent() == 1) return;
   PlayerCharacter->AddHealth(HealthAmount);

   Super::GiveItemToPlayer();
}