// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h" 
#include "ShooterCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CaptureCollider"));
	RootComponent = BoxCollider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollider"));
	SphereCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &APickupActor::OnOverlapEnd);
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FRotator NewRotation = Mesh->GetComponentRotation();
	// NewRotation.Yaw += RotateSpeed * DeltaTime;
	// Mesh->SetWorldRotation(NewRotation);

}

bool APickupActor::IsCaptureComponent(UPrimitiveComponent* OverlappedComponent) const
{
	return OverlappedComponent->GetName() == BoxCollider->GetName();
}

void APickupActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Enable highlight on the pickup
	Mesh->SetRenderCustomDepth(true);
}

void APickupActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Disable highlight on the pickup
	Mesh->SetRenderCustomDepth(false);
}
