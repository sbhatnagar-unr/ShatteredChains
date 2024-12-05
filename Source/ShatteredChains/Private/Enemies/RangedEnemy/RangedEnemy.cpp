// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "NavigationSystem.h"

// Sets default values
ARangedEnemy::ARangedEnemy() : AEnemy()
{
    weapon_class = nullptr;
    weapon = nullptr;
    anchor_point = nullptr;

    location_to_go_to = FVector::ZeroVector;
    anchor_tolerance = 0;
    accuracy = 0;
}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
    Super::BeginPlay();

    UWorld* world;
    USkeletalMeshComponent* skeletal_mesh_component;
    UNavigationSystemV1* navigation_system;

    try
    {
        // Get the world
        world = Validity::check_value<UWorld>(GetWorld(), "Could not get world");
        
        // Get the target
        target = Validity::check_value<AActor>(Cast<AActor>(world->GetFirstPlayerController()->GetPawn()), "Could not locate target (player)");
        
        // Check that we have an Anchor
        Validity::check_value<AAnchorPoint>(anchor_point, "No anchor point");
        
        // Check that we have stuff to attach the weapon
        Validity::check_value<UClass>(weapon_class, "No weapon class");
        UE_LOG(Enemy, Verbose, LOG_TEXT("Weapon class for %s is %s"), *GetActorLabel(), *weapon_class->GetName());
        
        skeletal_mesh_component = Validity::check_value<USkeletalMeshComponent>(GetMesh(), "No skeletal Mesh");
        
        // Get navigation system
        navigation_system = Validity::check_value<UNavigationSystemV1>(UNavigationSystemV1::GetCurrent(world), "Could not get navigation system");
        
        // Check the anchor tolerance
        Validity::check_value<float>(anchor_tolerance, 0, "Anchor tolerance is unset");
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Anchor tolerance for %s is %f"), *GetActorLabel(), anchor_tolerance);
        
        // Check the accuracy
        Validity::check_value<float>(accuracy, 0, "Accuracy is unset");
    }
    catch (const Validity::NullPointerException &e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs for %s"), e.what(), *GetActorLabel());
        return;
    }

    // Validate the value of accuracy
    if (accuracy <= 0 || accuracy > 100)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("Accuracy for %s is out of range (0-100].  accuracy=%f will be replaced by 1 if <=0 or 100 if >100"), *GetActorLabel(), accuracy);
        FMath::Clamp(accuracy, 1, 100);
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Accuracy for %s is %f%%"), *GetActorLabel(), accuracy);
    accuracy /= 100;

    
    // Spawn the weapon for the Enemy
    FActorSpawnParameters spawn_parameters;
    spawn_parameters.Owner = this;
    // Spawn it with the same transform as the enemy, it will be moved anyway
    weapon = world->SpawnActor<AWeapon>(weapon_class, GetActorTransform(), spawn_parameters);
    if (weapon == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not spawn weapon for %s"), *GetActorLabel());
        return;
    }
    // Set weapon name
    weapon->SetActorLabel(GetActorLabel() + FString(TEXT("'s weapon")));
    UE_LOG(Enemy, Verbose, LOG_TEXT("Weapon %s successfully spawned for %s"), *weapon->GetActorLabel(), *GetActorLabel());
    
    // Attach it to the enemy
    const bool successfully_attached = weapon->AttachToComponent(skeletal_mesh_component, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("GunSocket")));
    if (!successfully_attached)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not attach %s to %s"), *weapon->GetActorLabel(), *GetActorLabel());
        return;
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Successfully attached %s to %s"), *GetActorLabel(), *weapon_class->GetName());

    // https://forums.unrealengine.com/t/getrandomreachablepointinradius/380662
    // So this function is apparently bugged for who knows what reason.  To combat this, we will just run it a bunch of times
    // Until a valid result is gotten before we decide if it's an error
    
    // Get random point in anchor radius, this will be the point it runs to and shoots from
    FNavLocation nav_location;
    // Try 50 (arbitrary) times to get a location
    bool found_location = false;
    for (unsigned int i = 0; i < 50; i++)
    {
        // If we find one break out
        if (navigation_system->GetRandomReachablePointInRadius(anchor_point->GetActorLocation(), anchor_point->get_anchor_radius(), nav_location))
        {
            location_to_go_to = nav_location.Location;
            found_location = true;
            UE_LOG(Enemy, Log, LOG_TEXT("Location to go to in anchor for %s is %s"), *GetActorLabel(), *location_to_go_to.ToString());
            break;
        }
    }
    if (!found_location)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("Could not find reachable point near anchor \"%s\" (radius=%f).  Defaulting to anchor location (%s)"), *anchor_point->GetActorLabel(), anchor_point->get_anchor_radius(), *anchor_point->GetActorLocation().ToString());
        location_to_go_to = anchor_point->GetActorLocation();
    }
}


AWeapon* ARangedEnemy::get_weapon() const
{
    return weapon;
}


AAnchorPoint* ARangedEnemy::get_anchor_point() const
{
    return anchor_point;
}


FVector ARangedEnemy::get_location_to_go_to() const
{
    return location_to_go_to;    
}


float ARangedEnemy::get_anchor_tolerance() const
{
    return anchor_tolerance;
}


FVector ARangedEnemy::get_hitscan_start_location() const
{
    return GetActorLocation();
}


FVector ARangedEnemy::get_hitscan_direction() const
{
    return GetActorForwardVector();
}