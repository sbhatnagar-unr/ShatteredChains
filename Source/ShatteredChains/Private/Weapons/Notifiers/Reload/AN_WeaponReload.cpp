// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponReload.h"
#include "ShatteredChains/Utility.h"
#include "ShatteredChains/Logging.h"
#include "Engine/HitResult.h"
#include "CollisionQueryParams.h"
#include "Weapons/Weapon.h"

void UAN_WeaponReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AWeapon* weapon;

    try
    {
        MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
        weapon = Validity::check_value<AWeapon>(Cast<AWeapon>(MeshComp->GetOwner()), "Could not get weapon actor from MeshComp");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Weapon, Error, LOG_TEXT("%hs"), e.what());
        return;
    }

    // Reload the weapon
    weapon->refill_magazine();
    UE_LOG(Weapon, Log, LOG_TEXT("Weapon reloaded"));

}
