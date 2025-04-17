// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MedKit.generated.h"

class AMyCharacter;

UCLASS()
class SHATTEREDCHAINS_API AMedKit : public AActor
{
    GENERATED_BODY()

public:
    AMedKit();

    void Use(AMyCharacter* Character);
    bool IsPlayerInRange() const { return bIsPlayerInRange; }
    FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Medkit Properties")
    float HealingAmount = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Medkit Properties")
    float NormalWalkSpeed = 400.0f;

    UPROPERTY(VisibleAnywhere, Category = "Collision")
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* MeshComponent;

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool bIsPlayerInRange = false;
};