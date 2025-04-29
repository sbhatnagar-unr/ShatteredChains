// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class SHATTEREDCHAINS_API AMeleeWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMeleeWeapon();

    virtual void BeginPlay() override;

    void Punch();

    UPROPERTY()
    bool bIsPunching = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MeleeRange = 150.0f;
     
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MeleeRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float Damage = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float DamageDelay = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* PunchMontage1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* PunchMontage2;

    UPROPERTY(EditAnywhere, Category = "Combat")
    USoundBase* PunchSound;

    UFUNCTION()
    void ApplyDamage();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    class USphereComponent* PickupSphere;

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool bIsPlayerInRange = false;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* SwordSlashMontage1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* SwordSlashMontage2;

    UPROPERTY(EditAnywhere, Category = "Combat")
    USoundBase* SwordSwingSound;

    UPROPERTY(EditAnywhere, Category = "Combat")
    USoundBase* SwordDrawSound;

    void SetOwnerPawn(APawn* NewOwnerPawn) { OwnerPawn = NewOwnerPawn; }
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsSword = false;

protected:
    APawn* OwnerPawn;
};

