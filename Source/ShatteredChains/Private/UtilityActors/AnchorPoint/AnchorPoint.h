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

    float get_anchor_radius() const;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditInstanceOnly, Category="Anchor", meta=(ToolTip="Acceptable radius for things to be considered anchored"))
    float anchor_radius;
};
