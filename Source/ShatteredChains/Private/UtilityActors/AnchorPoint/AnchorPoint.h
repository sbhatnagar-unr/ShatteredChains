// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnchorPoint.generated.h"

UCLASS()
class SHATTEREDCHAINS_API AAnchorPoint : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAnchorPoint();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
