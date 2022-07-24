// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class SIMPLESHOOTER_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetAmmo() const { return AmmoAmount; }
	int GetHealth() const { return HealthAmount; }

	USoundBase* GetPickupSound() const { return PickupSound; }

	bool IsCaptureComponent(UPrimitiveComponent* OverlappedComponent) const;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollider;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Pickup")
	bool RotatePickup = false;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	float RotateSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	int AmmoAmount = 0;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	int HealthAmount = 0;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	int PointAmount = 0;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	USoundBase* PickupSound;
};
