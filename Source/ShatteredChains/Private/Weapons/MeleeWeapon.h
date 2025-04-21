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

protected:
    APawn* OwnerPawn;
};
