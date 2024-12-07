// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponReload.h"
#include "ShatteredChains/Logging.h"
#include "Engine/HitResult.h"
#include "CollisionQueryParams.h"
#include "Weapons/Weapon.h"

void UAN_WeaponReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp == nullptr)
    {
        UE_LOG(Weapon, Error, LOG_TEXT("No mesh for animation notify"));
        return;
    }
    
    AWeapon* weapon = Cast<AWeapon>(MeshComp->GetOwner());
    if (weapon == nullptr)
    {
        UE_LOG(Weapon, Error, LOG_TEXT("Could not get weapon actor from MeshComp"));
        return;
    }

    const FString weapon_name = (weapon->Tags.Num() > 0) ? weapon->Tags[0].ToString() : FString(TEXT("UNTAGGED"));
    

    // Reload the weapon
    weapon->refill_magazine();
    UE_LOG(Weapon, Log, LOG_TEXT("Weapon (%s) reloaded"), *weapon_name);

}

