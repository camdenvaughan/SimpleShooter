// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "SimpleShooter/Controllers/ShooterAIController.h"



void AKillEmAllGameMode::PawnKilled(APawn* KilledPawn) 
{
    Super::PawnKilled(KilledPawn);

    APlayerController* PlayerController = Cast<APlayerController>(KilledPawn->GetController());
    if (PlayerController)
    {
        EndGame(false);
        return;
    }
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->IsDead())
        {
            return;
        }
    }
            EndGame(true);

}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerWinner);
    }
}
