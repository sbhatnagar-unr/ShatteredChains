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

    burst_count = 4;
    shoot_pause = 1;
    
    current_burst = 0;
    ready_to_shoot = true;
}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
    Super::BeginPlay();

    // If we spawn as dead we don't need to do any of the other things
    if (health_component->get_health() == 0) on_death(nullptr, false);
    
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
    stats_modifiers["Neck"]->set_damage_multiplier(head_shot_damage_multiplier);
    stats_modifiers["Neck"]->set_speed_multiplier(head_shot_speed_multiplier);
    stats_modifiers["Neck"]->set_accuracy_multiplier(head_shot_accuracy_multiplier);

    stats_modifiers["Hips"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Hips"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Hips"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["Spine"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["Spine2"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine2"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine2"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["LeftLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["LeftShoulder"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["LeftShoulder"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["LeftShoulder"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);
    
    stats_modifiers["LeftHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);
    
    stats_modifiers["RightArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["RightArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["RightArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);
    
    stats_modifiers["RightHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);
    
    sound_map["Neck"] = head_shot_sounds;
    sound_map["Hips"] = torso_shot_sounds;
    sound_map["Spine"] = torso_shot_sounds;
    sound_map["Spine2"] = torso_shot_sounds;
    sound_map["LeftLeg"] = leg_shot_sounds;
    sound_map["RightLeg"] = leg_shot_sounds;
    sound_map["LeftShoulder"] = arm_shot_sounds;
    sound_map["LeftHand"] = hand_shot_sounds;
    sound_map["RightArm"] = arm_shot_sounds;
    sound_map["RightHand"] = hand_shot_sounds;
}


void ARangedEnemy::on_death(const AActor* killed_by, const bool play_death_sound)
{
    if (weapon)
    {
        // Destroy the weapon
        weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        weapon->Destroy();
        UE_LOG(Enemy, Log, LOG_TEXT("Removing weapon from ranged enemy '%s'"), *actor_name);

        // Spawn one on the ground that the player can pick up
        const FVector enemy_transform = GetActorLocation();
        const FVector point_below = enemy_transform - FVector(0, 0, 1000);

        FHitResult hit_result;
        FCollisionQueryParams collision_query_params;
        collision_query_params.AddIgnoredActor(this);

        const UWorld* world = GetWorld();
        if (world->LineTraceSingleByChannel(hit_result, enemy_transform, point_below, ECC_Visibility, collision_query_params))
        {
            const FVector location = hit_result.Location + FVector(0, 0, 2);
            FRotator rotation = GetActorRotation();
            rotation.Pitch += 90;
            
            FActorSpawnParameters spawn_parameters;
            weapon = GetWorld()->SpawnActor<AWeapon>(weapon_class, location, rotation, spawn_parameters);
            UE_LOG(Enemy, Log, LOG_TEXT("Spawned weapon on ground upon death of enemy '%s'"), *actor_name);
        } else
        {
            UE_LOG(Enemy, Warning, LOG_TEXT("Unable to find ground to spawn weapon upon death for ranged enemy '%s'"), *actor_name);
        }
        

    }
    
    AEnemy::on_death(killed_by, play_death_sound);
}



void ARangedEnemy::increment_current_burst()
{
    ++current_burst;
    UE_LOG(Enemy, Log, LOG_TEXT("Ranged enemy '%s' incremented burst count to %d, max %d"), *actor_name, current_burst, burst_count);

    if (current_burst >= burst_count)
    {
        ready_to_shoot = false;

        UE_LOG(Enemy, Log, LOG_TEXT("Ranged enemy '%s' reached burst count of %d (max=%d), starting reset time (%f seconds)"), *actor_name, current_burst, burst_count, shoot_pause);
        FTimerHandle timer_handle;
        GetWorld()->GetTimerManager().SetTimer(
            timer_handle,
            FTimerDelegate::CreateLambda([this]()
            {
                this->ready_to_shoot = true;
                this->current_burst = 0;
                UE_LOG(Enemy, Log, LOG_TEXT("Ranged Enemy '%s' shoot timer has been reset (%f seconds)"), *actor_name, shoot_pause);
            }),
            shoot_pause,
            false
        );
    }
}


bool ARangedEnemy::is_ready_to_shoot() const
{
    return ready_to_shoot;
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

void ARangedEnemy::hit_bone(const AActor* hit_by, const FName bone_name, const float weapon_damage)
{
    Super::hit_bone(hit_by, bone_name, weapon_damage);

    const UStatsModifier* const stats_modifier = stats_modifiers[bone_name];

    const float old_pitch_offset = max_pitch_offset;
    const float old_yaw_offset = max_yaw_offset;
    
    max_pitch_offset *= stats_modifier->get_accuracy_multiplier();
    max_yaw_offset *= stats_modifier->get_accuracy_multiplier();

    UE_LOG(Enemy, Log, LOG_TEXT("Enemy '%s' accuracy adjusted (max_pitch_offset: %f -> %f, max_yaw_offset: %f -> %f)"), *actor_name, old_pitch_offset, max_pitch_offset, old_yaw_offset, max_yaw_offset);
}