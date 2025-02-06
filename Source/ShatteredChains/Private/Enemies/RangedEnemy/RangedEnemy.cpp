// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemy.h"
#include "ShatteredChains/Logging.h"
#include "NavigationSystem.h"

// Sets default values
ARangedEnemy::ARangedEnemy() : AEnemy()
{
    weapon_class = nullptr;
    weapon = nullptr;
    anchor_point = nullptr;

    location_to_go_to = FVector::ZeroVector;
    anchor_tolerance = 0;
    max_pitch_offset = 0;
    max_yaw_offset = 0;
}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Get the world
    UWorld* world = GetWorld();
    if (world == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get world for %s"), *actor_name);
        return;
    }
    
    // Check that we have an Anchor
    if (anchor_point == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No anchor point for %s"), *actor_name);
        return;
    }
    
    // Check that we have stuff to attach the weapon
    if (weapon_class == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No weapon class for %s"), *actor_name);
        return;
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Weapon class for %s is %s"), *actor_name, *weapon_class->GetName());
    
    USkeletalMeshComponent* skeletal_mesh_component = GetMesh();
    if (skeletal_mesh_component == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No skeletal Mesh for %s"), *actor_name);
        return;
    }
    
    // Get navigation system
    const UNavigationSystemV1* navigation_system = UNavigationSystemV1::GetCurrent(world);
    if (navigation_system == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get navigation system for %s"), *actor_name);
        return;
    }
    
    // Check the anchor tolerance
    if (anchor_tolerance == 0)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Anchor tolerance is unset for %s"), *actor_name);
        return;
    }
    UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Anchor tolerance for %s is %f"), *actor_name, anchor_tolerance);


    // Spawn the weapon for the Enemy
    FActorSpawnParameters spawn_parameters;
    spawn_parameters.Owner = this;
    // Spawn it with the same transform as the enemy, it will be moved anyway
    weapon = world->SpawnActor<AWeapon>(weapon_class, GetActorTransform(), spawn_parameters);
    if (weapon == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not spawn weapon for %s"), *actor_name);
        return;
    }
    // Set weapon name
    weapon->set_actor_name(actor_name + FString(TEXT("'s ")) + weapon->GetName());

    UE_LOG(Enemy, Verbose, LOG_TEXT("Weapon %s successfully spawned for %s"), *(weapon->get_actor_name()), *actor_name);
    
    // Attach it to the enemy
    const bool successfully_attached = weapon->AttachToComponent(skeletal_mesh_component, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("GunSocket")));
    if (!successfully_attached)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not attach %s to %s"), *(weapon->get_actor_name()), *actor_name);
        return;
    }
    UE_LOG(Enemy, Verbose, LOG_TEXT("Successfully attached %s to %s"), *actor_name, *weapon_class->GetName());

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
            UE_LOG(Enemy, Verbose, LOG_TEXT("Location to go to in anchor for %s is %s"), *actor_name, *location_to_go_to.ToString());
            break;
        }
    }
    if (!found_location)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("Could not find reachable point near anchor \"%s\" (radius=%f).  Defaulting to anchor location (%s)"), *(anchor_point->get_actor_name()), anchor_point->get_anchor_radius(), *anchor_point->GetActorLocation().ToString());
        location_to_go_to = anchor_point->GetActorLocation();
    }


    // Here we set all the bone collision modifier values
    stats_modifiers["head"]->set_additive_damage_modifier(head_shot_damage_adder);
    stats_modifiers["head"]->set_multiplicative_damage_modifier(head_shot_damage_multiplier);
    stats_modifiers["head"]->set_additive_speed_modifier(head_shot_speed_adder);
    stats_modifiers["head"]->set_multiplicative_speed_modifier(head_shot_speed_multiplier);

    stats_modifiers["pelvis"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["pelvis"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["pelvis"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["pelvis"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["spine_01"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["spine_01"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["spine_01"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["spine_01"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["spine_03"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["spine_03"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["spine_03"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["spine_03"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);
    
    stats_modifiers["upperarm_l"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["upperarm_l"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_l"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["upperarm_l"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);
    
    stats_modifiers["hand_l"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["hand_l"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["hand_l"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["hand_l"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);
    
    stats_modifiers["upperarm_r"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["upperarm_r"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_r"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["upperarm_r"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);
    
    stats_modifiers["hand_r"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["hand_r"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["hand_r"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["hand_r"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);
    
    stats_modifiers["thigh_l"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["thigh_l"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_l"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["thigh_l"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["calf_l"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["calf_l"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["calf_l"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["calf_l"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["thigh_r"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["thigh_r"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_r"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["thigh_r"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["calf_r"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["calf_r"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["calf_r"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["calf_r"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
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
    // Calculate a forward vector based off of offsets
    // Basically pick a random vector in the cone provided by the offsets

    // 50/50 chance for moving in either direction
    float pitch_change = (FMath::FRand() < 0.5f) ? -1 : 1;
    float yaw_change = (FMath::FRand() < 0.5f) ? -1 : 1;

    // Apply a random angle in between 0 and the offset
    pitch_change *= FMath::FRand() * max_pitch_offset;
    yaw_change   *= FMath::FRand() * max_yaw_offset;

    // Make the rotator
    const FRotator accuracy_change = FRotator(pitch_change, yaw_change, 0);

    // Get the forward vector and rotate it
    const FVector direction = accuracy_change.RotateVector(GetActorForwardVector());
    
    return direction;
}