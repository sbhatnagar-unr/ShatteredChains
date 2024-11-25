// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponFire.h"
#include "Camera/CameraComponent.h"
#include "ShatteredChains/Private/Weapons/Weapon.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"

void UAN_WeaponFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

    // The players camera
    UCameraComponent* camera;
    // The weapon
    AWeapon* weapon;
    // The Actor holding the weapon
    AActor* weapon_holder;
    // World is needed for the scan
    UWorld* world;
    try
    {
        MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
        weapon = Validity::check_value<AWeapon>(Cast<AWeapon>(MeshComp->GetOwner()), "Could not get weapon actor from MeshComp");
        //weapon_holder = Validity::check_value<AMyCharacter>(Cast<AMyCharacter>(weapon->GetAttachParentActor()), "Could not get weapon holder from weapon");
        //camera = Validity::check_value<UCameraComponent>(weapon_holder->Camera, "Coul not get camera component from character");
        world = Validity::check_value<UWorld>(MeshComp->GetWorld(), "Could not get world");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
        return;
    }

    /*TEST CODE*/
    /*TEST CODE*/
    /*TEST CODE*/
    /*TEST CODE*/
    AMyCharacter* player = Cast<AMyCharacter>(world->GetFirstPlayerController()->GetCharacter());
    if (player == nullptr)
    {
        UE_LOG(Weapons, Error, LOG_TEXT("CANT GET PLAYER"));
        return;
    }
    camera = player->Camera;
    weapon_holder = nullptr;
    /*DELETE AFTER TESTING*/
    /*DELETE AFTER TESTING*/
    /*DELETE AFTER TESTING*/
    /*DELETE AFTER TESTING*/


    // Trace results
    struct FHitResult trace_result;
    // Starting position
    FVector trace_start = camera->GetComponentLocation();
    // End position
    FVector trace_end = trace_start + (camera->GetForwardVector() * weapon->get_scan_distance());
    // Trace params
    struct FCollisionQueryParams trace_params;
    // Actually check if it hits the mesh
    trace_params.bTraceComplex = true;


    // Perform the trace
    UE_LOG(Weapons, Log, LOG_TEXT("Firing weapon"));
    UE_LOG(Weapons, VeryVerbose, LOG_TEXT("[Line Trace] %s -> %s"), *trace_start.ToString(), *trace_end.ToString());
    bool hit_something = world->LineTraceSingleByChannel(trace_result, trace_start, trace_end, ECollisionChannel::ECC_Visibility, trace_params);
    
    // Draw a debug line
    DrawDebugLine(world, trace_start, trace_end, FColor::Red, false, 0.07f, 0U, 0.3f);

    if (hit_something)
    {
        // Draw box around the spot we hit
        DrawDebugBox(world, trace_result.ImpactPoint, FVector(1, 1, 1), FColor::Blue, false, 3.f, 0U, 5.f);
        // Check if the actor was a charachter with health
        ACharacterWithHealth* hit_actor;
        try
        {
            hit_actor = Validity::check_value<ACharacterWithHealth>(Cast<ACharacterWithHealth>(trace_result.GetActor()), "");
        }
        catch (const Validity::NullPointerException&)
        {
            // Log the thing we hits name
    		UE_LOG(Weapons, Verbose, LOG_TEXT("Trace hit undamageable target (%s)"), *(trace_result.GetActor()->GetName()));
			return;
		}
	
		// If the hit actor has health
		float weapon_damage = weapon->get_weapon_damage();
		UE_LOG(Weapons, Log, LOG_TEXT("Trace hit target (%s), %f damage dealt"), *(trace_result.GetActor()->GetName()), weapon_damage);
		hit_actor->deal_damage(weapon_holder, weapon_damage);
	}
	else
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Trace hit no target"));
	}

}