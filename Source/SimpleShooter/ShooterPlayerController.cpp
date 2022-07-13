// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    HUD = CreateWidget<UUserWidget>(this, HUDClass);
    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();

    UUserWidget* Screen;
    if (bIsWinner)
    {
        Screen = CreateWidget<UUserWidget>(this, WinScreenClass);
    }
    else
    {
        Screen = CreateWidget<UUserWidget>(this, LoseScreenClass);
    }

    if (Screen != nullptr)
    {
        Screen->AddToViewport();
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &AShooterPlayerController::RestartLevel, RestartDelay);
}
