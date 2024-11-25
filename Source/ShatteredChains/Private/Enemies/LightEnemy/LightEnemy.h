// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public AEnemy
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALightEnemy();

    float get_attack_damage() const;

    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


private:
    UPROPERTY(EditDefaultsOnly, Category="Attacking")
    float attack_damage;
};
