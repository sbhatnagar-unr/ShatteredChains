// Fill out your copyright notice in the Description page of Project Settings.


#include "AnchorPoint.h"

#include "PhysicsEngine/SphereElem.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(UtilityActor);


// Sets default values
AAnchorPoint::AAnchorPoint()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    anchor_radius = 200;
    mesh_component = nullptr;
    sphere_component = nullptr;
    
#if WITH_EDITOR
    mesh_component = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
    sphere_component = CreateDefaultSubobject<USphereComponent>("Anchor Radius Visual (editor)");
    sphere_component->SetupAttachment(mesh_component);
    sphere_component->InitSphereRadius(anchor_radius);
    sphere_component->SetRelativeLocation(FVector::ZeroVector);
#endif
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


#if WITH_EDITOR
void AAnchorPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName property_name = PropertyChangedEvent.GetPropertyName();
    // If name is none do nothing
    if (property_name == NAME_None) return;

    // If we changed the radius or the location
    if (property_name == GET_MEMBER_NAME_CHECKED(AAnchorPoint, anchor_radius))
    {
        if (GIsEditor)
        {
            sphere_component->SetSphereRadius(anchor_radius);
        }
    }
}
#endif


