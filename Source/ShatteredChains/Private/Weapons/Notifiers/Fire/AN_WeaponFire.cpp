// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponFire.h"
#include "ShatteredChains/Logging.h"


void UAN_WeaponFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	// The players camera
	//UCameraComponent* camera;
	//AWeapon* weapon;
	//
	//try
	//{
	//	MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
	//	weapon = Validity::check_value<AWeapon>(Cast<AWeapon>(MeshComp->GetOwner()), "Could not get weapon actor from MeshComp");
	//	AMyCharacter* weapon_holder = Validity::check_value<AMyCharacter>(Cast<AMyCharacter>(weapon->GetAttachParentActor()), "Could not get weapon holder from weapon");
	//	camera = Validity::check_value<UCameraComponent>(weapon_holder->Camera, "Could not get camera component from character");
	//}
	//catch (const Validity::NullPointerException& e)
	//{
	//	UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
	//	return;
	//}
	//
	//// World is needed for the scan
	//UWorld* world = GetWorld();
	//
	//// Starting position
	//FVector trace_start = camera->GetComponentLocation();
	//// End position
	//FVector trace_end = camera->GetForwardVector() * weapon->get_scan_distance();
	//
	//// Trace results
	//struct FHitResult trace_result;
	//
	//struct FCollisionQueryParams query_params;
	//// Actually check if it hits the mesh
	//query_params.bTraceComplex = true;
	////world->LineTraceSingleByChannel(trace_result, trace_start, trace_end, ECollisionChannel::ECC_Visibility, );
	//
	//DrawDebugLine();

	UE_LOG(Weapons, Log, LOG_TEXT("Firing weapon"));
}