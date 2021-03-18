// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    if (HUD)
    {
        HUD->RemoveFromViewport();
    }
    if (bIsWinner)
    {
        if (!WinScreenClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("No Win Screen Set"));
            return;
        }
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        bShowMouseCursor = true;
        WinScreen->AddToViewport();
    }
    else
    {
        if (!LoseScreenClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("No Lose Screen Set"));
            return;
        }
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        LoseScreen->AddToViewport();
        GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    }
}

void AShooterPlayerController::ShowPauseUI(bool bIsShowingPauseUI) 
{
    if (!PauseScreenClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Pause Screen Class Defined!"));
        return;
    }
    if (bIsShowingPauseUI)
    {
        PauseScreen = CreateWidget(this, PauseScreenClass);
        PauseScreen->AddToViewport();
    }
    else
    {
        PauseScreen->RemoveFromViewport();
    }
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    if (!HUDClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("No HUD Set"));
        return;
    }
    HUD = CreateWidget(this, HUDClass);
    HUD->AddToViewport();
    
}
