// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MyCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MedKit.generated.h"

UCLASS()
class SHATTEREDCHAINS_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKit();

	// Function to use the medkit
	void Use(class AMyCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    // Properties
    UPROPERTY(EditAnywhere, Category = "Medkit Properties")
    float HealingAmount = 100.0f; // Fully restores health

    UPROPERTY(EditAnywhere, Category = "Medkit Properties")
    float NormalWalkSpeed = 400.0f; // Restores movement speed

    // Sphere collision for detecting overlap with player
    UPROPERTY(VisibleAnywhere, Category = "Collision")
    USphereComponent* SphereComponent;

    // Mesh for physical appearance and physics simulation
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* MeshComponent;

    // Collision handling for pickup
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
