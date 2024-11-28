// Fill out your copyright notice in the Description page of Project Settings.


#include "AnchorPoint.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(UtilityActor);


// Sets default values
AAnchorPoint::AAnchorPoint()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    anchor_radius = 0;
}

// Called when the game starts or when spawned
void AAnchorPoint::BeginPlay()
{
    Super::BeginPlay();
    SetActorHiddenInGame(true);

    UE_LOG(UtilityActor, Verbose, LOG_TEXT("Anchor location (%s) is %s"), *GetActorLabel(), *GetActorLocation().ToString());
    if (anchor_radius == 0)
    {
        UE_LOG(UtilityActor, Error, LOG_TEXT("Anchor radius (%s) was never set"), *GetActorLabel());
    }
    else
    {
        UE_LOG(UtilityActor, VeryVerbose, LOG_TEXT("Anchor point (%s) radius %f"), *GetActorLabel(), anchor_radius);
    }
}

float AAnchorPoint::get_anchor_radius() const
{
    return anchor_radius;    
}

