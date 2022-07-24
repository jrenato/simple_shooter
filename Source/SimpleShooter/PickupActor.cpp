// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CaptureCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CaptureCollider"));
	RootComponent = CaptureCollider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DetectionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollider"));
	DetectionCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	CaptureCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBeginCapture);

	DetectionCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBeginDetection);
	DetectionCollider->OnComponentEndOverlap.AddDynamic(this, &APickupActor::OnOverlapEndDetection);
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FRotator NewRotation = Mesh->GetComponentRotation();
	// NewRotation.Yaw += RotateSpeed * DeltaTime;
	// Mesh->SetWorldRotation(NewRotation);

}

void APickupActor::OnOverlapBeginCapture(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor))
	{
		if (Player->IsPlayerControlled())
		{
			if (AmmoAmount > 0)
			{
				Player->AddAmmo(AmmoAmount);
			}
			else if (HealthAmount > 0)
			{
				Player->AddHealth(HealthAmount);
			}

			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
			Destroy();
		}
	}
}

void APickupActor::OnOverlapBeginDetection(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Mesh->SetRenderCustomDepth(true);
}

void APickupActor::OnOverlapEndDetection(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Mesh->SetRenderCustomDepth(false);
}
