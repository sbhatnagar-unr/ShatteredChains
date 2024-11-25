// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "RangedEnemy.generated.h"

UCLASS()
class SHATTEREDCHAINS_API ARangedEnemy : public AEnemy
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ARangedEnemy();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};
