// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawner.h"
#include "PickupActor.h"

// Sets default values for this component's properties
UPickupSpawner::UPickupSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPickupSpawner::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPickupSpawner::SpawnPickup()
{
	if (PickupActorClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FTransform SpawnTransform = GetComponentTransform();
			APickupActor* const Pickup = World->SpawnActor<APickupActor>(PickupActorClass->GetDefaultObject()->GetClass(), SpawnTransform);
		}
	}
}

// Called every frame
void UPickupSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
