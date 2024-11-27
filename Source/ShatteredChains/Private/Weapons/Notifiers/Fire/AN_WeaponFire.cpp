// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponFire.h"
#include "Weapons/Weapon.h"
#include "Interfaces/WeaponUser/WeaponUser.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"

void UAN_WeaponFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    // The animation instance
    UAnimInstance* anim_instance = MeshComp->GetAnimInstance();
    // The weapon
    AWeapon* weapon;
    // The actor (should be actor anyway) holding the weapon
    IWeaponUser* weapon_holder;
    // The holders name
    FString weapon_holder_name;
    // World is needed for the scan
    const UWorld* world;
    try
    {
        MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
        weapon = Validity::check_value<AWeapon>(Cast<AWeapon>(MeshComp->GetOwner()), "Could not get weapon actor from MeshComp");
        AActor* weapon_holder_actor = Validity::check_value<AActor>(Cast<AActor>(weapon->GetAttachParentActor()), "Could not get weapon holder from weapon (as AActor)");
        weapon_holder_name = weapon_holder_actor->GetActorLabel();
        weapon_holder = Validity::check_value<IWeaponUser>(Cast<IWeaponUser>(weapon_holder_actor), "Weapon holder (" + std::string(TCHAR_TO_UTF8(*weapon_holder_name)) + ") does not implement IWeaponUser");
        world = Validity::check_value<UWorld>(MeshComp->GetWorld(), "Could not get world");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
        anim_instance->Montage_Stop(0);
        return;
    }


    // Hitscan direction
    FVector scan_direction = weapon_holder->get_hitscan_direction();
    if (!scan_direction.Normalize()) // Normalize in place, returns true if successful
    {
        UE_LOG(Weapons, Error, LOG_TEXT("Could not normalize %s scan direction, aborting weapon fire"), *weapon_holder_name);
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


    // Perform the trace
    UE_LOG(Weapons, Log, LOG_TEXT("Firing weapon"));
    UE_LOG(Weapons, VeryVerbose, LOG_TEXT("[Line Trace] %s -> %s"), *trace_start.ToString(), *trace_end.ToString());
    const bool hit_something = world->LineTraceSingleByChannel(trace_result, trace_start, trace_end, ECollisionChannel::ECC_Visibility, trace_params);

    // Remove one ammunition
    weapon->decrement_mag_ammo_count();

    // Draw a debug line
    DrawDebugLine(world, trace_start, trace_end, FColor::Red, false, 0.07f, 0U, 0.3f);

    if (hit_something)
    {
        // Draw box around the spot we hit
        DrawDebugBox(world, trace_result.ImpactPoint, FVector(1, 1, 1), FColor::Blue, false, 3.f, 0U, 5.f);

        // Check if the actor was a character with health
        AActor* a = trace_result.GetActor();
        const FString hit_actor_label = a->GetActorLabel();
        const IHasHealth* hit_actor = Cast<IHasHealth>(a);

        if (hit_actor == nullptr)
        {
            // Log the thing we hits name
            UE_LOG(Weapons, Verbose, LOG_TEXT("Trace hit un-damageable target (%s)"), *hit_actor_label);
            return;
        }
    
        // If the hit actor has health
        const float weapon_damage = weapon->get_weapon_damage();
        hit_actor->get_health_component()->deal_damage(Cast<AActor>(weapon_holder), weapon_damage);
        UE_LOG(Weapons, Log, LOG_TEXT("Trace hit target (%s), %f damage dealt"), *hit_actor_label, weapon_damage);
    }
    else
    {
        UE_LOG(Weapons, Verbose, LOG_TEXT("Trace hit no target"));
    }
}