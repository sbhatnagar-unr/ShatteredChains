// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponReload.h"
#include "ShatteredChains/Utility.h"
#include "ShatteredChains/Logging.h"
#include "Camera/CameraComponent.h"
#include "Engine/HitResult.h"
#include "CollisionQueryParams.h"
#include "../../../Characters/Player/MyCharacter.h"
#include "../../Guns/Weapon.h"


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
		UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
		return;
	}

	// Reload the weapon
	weapon->refill_magazine();
	UE_LOG(Weapons, Log, LOG_TEXT("Weapon reloaded"));

}

