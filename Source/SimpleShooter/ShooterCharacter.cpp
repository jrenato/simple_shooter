// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PickupSpawner.h"
#include "PickupActor.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupSpawner = CreateDefaultSubobject<UPickupSpawner>(TEXT("PickupSpawner"));
	PickupSpawner->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CollisionCylinder = Cast<UCapsuleComponent>(GetCapsuleComponent());
	if (this->IsPlayerControlled()){
		CollisionCylinder->OnComponentBeginOverlap.AddDynamic(this, &AShooterCharacter::OnOverlapBegin);
	}

	Health = MaxHealth;
	Points = 0;

	if (GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("LookRightRate", this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APickupActor::StaticClass()))
	{
		APickupActor* Pickup = Cast<APickupActor>(OtherActor);

		if (!Pickup->IsCaptureComponent(OtherComp))
		{
			return;
		}

		this->AddHealth(Pickup->GetHealth());
		this->AddAmmo(Pickup->GetAmmo());
		this->AddPoints(Pickup->GetPoints());

		UGameplayStatics::PlaySoundAtLocation(this, Pickup->GetPickupSound(), GetActorLocation());
		Pickup->Destroy();
	}
}

void AShooterCharacter::Shoot()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health = FMath::Max(0.0f, Health - DamageToApply);

	if (IsDead())
	{
		if (GetCapsuleComponent()->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
		{
			PickupSpawner->SpawnPickup();
		}

		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.0f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

int AShooterCharacter::GetCurrentAmmo() const
{
	if (Gun)
	{
		return Gun->GetCurrentAmmo();
	}
	return 0;
}

int AShooterCharacter::GetCurrentPoints() const
{
	return Points;
}

void AShooterCharacter::AddAmmo(int Amount)
{
	if (Gun)
	{
		Gun->AddAmmo(Amount);
	}
}

void AShooterCharacter::AddHealth(float Amount)
{
	Health = FMath::Min(Health + Amount, MaxHealth);
}

void AShooterCharacter::AddPoints(int Amount)
{
	Points += Amount;	
}
