// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"


// Sets default values
ARangedEnemy::ARangedEnemy() : AEnemy()
{
    weapon_class = nullptr;
    weapon = nullptr;
    anchor_point = nullptr;
}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
    Super::BeginPlay();

    UWorld* world;
    USkeletalMeshComponent* skeletal_mesh_component;
    
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
    }
    catch (const Validity::NullPointerException &e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs for %s"), e.what(), *GetActorLabel());
        return;
    }
    
    // Spawn the weapon for the enemy
    FActorSpawnParameters spawn_parameters;
    spawn_parameters.Owner = this;
    // Spawn it with the same transform as the enemy, it will be moved anyway
    weapon = world->SpawnActor<AWeapon>(weapon_class, GetActorTransform(), spawn_parameters);
    
    if (weapon == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not spawn weapon for %s"), *GetActorLabel());
        return;
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Weapon %s successfully spawned for %s"), *weapon->GetActorLabel(), *GetActorLabel());
    
    // Attach it to the enemy
    const bool successfully_attached = weapon->AttachToComponent(skeletal_mesh_component, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("GunSocket")));
    if (!successfully_attached)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not attach %s to %s"), *weapon->GetActorLabel(), *GetActorLabel());
        return;
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Successfully attached %s to %s"), *GetActorLabel(), *weapon_class->GetName());
    
}


AWeapon* ARangedEnemy::get_weapon() const
{
    return weapon;
}
