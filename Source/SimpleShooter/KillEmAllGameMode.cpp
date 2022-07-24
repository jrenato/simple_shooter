// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "PickupActor.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    // int NumPickups = 0;
    // for (APickupActor* Pickup : TActorRange<APickupActor>(GetWorld()))
    // {
    //     if (Pickup->GetPoints() > 0)
    //     {
    //         NumPickups += Pickup->GetPoints();
    //     }
    // }

    // UE_LOG(LogTemp, Warning, TEXT("%d pickups left"), NumPickups);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        EndGame(false);
    }

    for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!AIController->IsDead())
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
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
