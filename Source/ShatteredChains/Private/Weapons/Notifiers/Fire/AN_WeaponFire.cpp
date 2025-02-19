// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponFire.h"
#include "Weapons/Weapon.h"
#include "Interfaces/WeaponUser/WeaponUser.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Components/HealthComponent/HealthComponent.h"
#include "Interfaces/HasBoneCollider/HasBoneCollider.h"
#include "ShatteredChains/CustomTraceChannels.h"
#include "ShatteredChains/Logging.h"

void UAN_WeaponFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No mesh for animation notify"));
        return;
    }

    // The weapon
    AWeapon* weapon = Cast<AWeapon>(MeshComp->GetOwner());
    if (weapon == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get weapon actor from MeshComp"));
        return;
    }
    const FString weapon_name = weapon->get_actor_name();

    // The weapon holder as an actor
    AActor* weapon_holder_actor = Cast<AActor>(weapon->GetAttachParentActor());
    if (weapon_holder_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get weapon holder from weapon (as AActor)"));
        return;
    }

    // The holders name
    const INamedActor* const weapon_holder_named_actor = Cast<INamedActor>(weapon_holder_actor);
    if (weapon_holder_named_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Weapon (%s) holder does not inherit INamedActor"), *weapon_name);
        return;
    }
    const FString weapon_holder_name = weapon_holder_named_actor->get_actor_name();
    // The animation instance
    UAnimInstance* anim_instance = MeshComp->GetAnimInstance();
    if (anim_instance == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get anim instance for %s"), *weapon_name);
        return;
    }

    // The weapon holder as a weapon user
    IWeaponUser* weapon_holder = Cast<IWeaponUser>(weapon_holder_actor);
    if (weapon_holder == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Weapon holder (%s) does not implement IWeaponUser"), *weapon_holder_name);
    }
    
    // World is needed for the scan
    const UWorld* world = MeshComp->GetWorld();
    if (world == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get world"));
        return;
    }


    // Hitscan direction
    FVector scan_direction = weapon_holder->get_hitscan_direction();
    if (!scan_direction.Normalize()) // Normalize in place, returns true if successful
    {
        UE_LOG(Weapon, Error, LOG_TEXT("Could not normalize %s scan direction, aborting weapon fire"), *weapon_holder_name);
        anim_instance->Montage_Stop(0);
        return;
    }
    
    // Trace results
    struct FHitResult trace_result;
    // Starting position
    const FVector trace_start = weapon_holder->get_hitscan_start_location();
    // End position
    const FVector trace_end = trace_start + (scan_direction * weapon->get_scan_distance());
    // Trace params
    struct FCollisionQueryParams trace_params;
    // Actually check if it hits the mesh, rather than some collision boundary or something
    trace_params.bTraceComplex = true;
    // Ignore the weapon holder from the line-trace
    trace_params.AddIgnoredActor(weapon_holder_actor);
    
    // Perform the trace
    UE_LOG(Weapon, Log, LOG_TEXT("Firing weapon (%d mag ammo remaining)"), weapon->get_current_magazine_ammo_count());
    UE_LOG(Weapon, VeryVerbose, LOG_TEXT("[Line Trace] %s -> %s"), *trace_start.ToString(), *trace_end.ToString());
    const bool hit_something = world->LineTraceSingleByChannel(trace_result, trace_start, trace_end, ShootableChannel, trace_params);

    // Remove one ammunition
    weapon->decrement_mag_ammo_count();


    if (hit_something)
    {
        // Draw a debug line
        DrawDebugLine(world, trace_start, trace_result.ImpactPoint, FColor::Red, false, 0.07f, 0U, 0.3f);
        // Draw box around the spot we hit
        DrawDebugBox(world, trace_result.ImpactPoint, FVector(1, 1, 1), FColor::Blue, false, 3.f, 0U, 5.f);

        // Check if the actor was a character with health
        TObjectPtr<AActor> a = trace_result.GetActor();
        const INamedActor* const named_actor = Cast<INamedActor>(a);
        FString hit_actor_label = FString(TEXT("NOT A NAMED ACTOR"));

        
        if (named_actor != nullptr)
        {
            hit_actor_label = named_actor->get_actor_name();
        }
        const IHasHealth* hit_health_actor = Cast<IHasHealth>(a);

        if (hit_health_actor == nullptr)
        {
            // Log the thing we hits name
            UE_LOG(Weapon, Verbose, LOG_TEXT("Trace hit un-damageable target (%s)"), *hit_actor_label);
            return;
        }
        
        
        float weapon_damage = weapon->get_weapon_damage();
        
        // Bone collision
        // If the actor has a bone collider
        if (IHasBoneCollider* hit_bone_actor = Cast<IHasBoneCollider>(a))
        {
            const TObjectPtr<UStatsModifier>* modifier_ptr = hit_bone_actor->get_bone_collider_stats_modifiers()->Find(trace_result.BoneName);
            if (modifier_ptr == nullptr)
            {
                UE_LOG(BoneCollision, Error, LOG_TEXT("Actor '%s' does not have modifier for bone %s.  Its possible its capsule component is blocking the shot, set trace channel Shootable to Ignore on the capsule component."), *hit_actor_label, *(trace_result.BoneName.ToString()));
                return;
            }
            const TObjectPtr<UStatsModifier> modifier = *modifier_ptr;
            
            const float old_damage = weapon_damage;
            weapon_damage += modifier->get_additive_damage_modifier();
            weapon_damage *= modifier->get_multiplicative_damage_modifier();
            UE_LOG(BoneCollision, Log, LOG_TEXT("Stats modifiers for '%s' in group '%s': DAMAGE_ADD=%f\tDAMAGE_MUL=%f"), *hit_actor_label, *(trace_result.BoneName.ToString()), modifier->get_additive_damage_modifier(), modifier->get_multiplicative_damage_modifier());
            UE_LOG(BoneCollision, Log, LOG_TEXT("Damage for '%s' modified from %f -> %f"), *hit_actor_label, old_damage, weapon_damage);

            // Pass the bone info to the actor that was hit so they can adjust their stats
            hit_bone_actor->hit_bone(trace_result.BoneName);
        }
        // Otherwise
        else
        {
            UE_LOG(Weapon, Verbose, LOG_TEXT("Trace hit target (%s), no bone collider"), *hit_actor_label);
        }
        
        hit_health_actor->get_health_component()->deal_damage(Cast<AActor>(weapon_holder), weapon_damage);
        UE_LOG(Weapon, Log, LOG_TEXT("Trace hit target (%s), %f damage dealt"), *hit_actor_label, weapon_damage);
    }
    else
    {
        // Draw a debug line
        DrawDebugLine(world, trace_start, trace_end, FColor::Red, false, 0.07f, 0U, 0.3f);
        UE_LOG(Weapon, Verbose, LOG_TEXT("Trace hit no target"));
    }
}