// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"

DEFINE_LOG_CATEGORY(Weapons);

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	magazine_size = 0;
	current_magazine_ammo_count = 0;
	max_ammo_stock_pile_count = 0;
	current_ammo_stock_pile_count = 0;
	
	root_scene_component = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	weapon_skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	
	RootComponent = root_scene_component;
	weapon_skeletal_mesh_component->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	try
	{
		// Check for mesh
		Validity::check_value<USkeletalMesh>(weapon_skeletal_mesh_component->GetSkeletalMeshAsset(), "No skeletal mesh for weapon");
		UE_LOG(Weapons, Verbose, LOG_TEXT("Weapon has skeletal mesh"));

		// Check for animations
		Validity::check_value<UAnimMontage>(fire_animation_montage, "No fire animation montage for weapon");
		UE_LOG(Weapons, Verbose, LOG_TEXT("Fire animation montage: %s"), *fire_animation_montage.GetFullName());
		
		Validity::check_value<UAnimMontage>(reload_animation_montage, "No reload animation montage for weapon");
		UE_LOG(Weapons, Verbose, LOG_TEXT("Reload animation montage: %s"), *reload_animation_montage.GetFullName());

		anim_instance = Validity::check_value<UAnimInstance>(weapon_skeletal_mesh_component->GetAnimInstance(), "No anim instance found");

	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
	}

	// Set ammo
	current_ammo_stock_pile_count = max_ammo_stock_pile_count;
	refill_magazine();
}


void AWeapon::fire() const
{
	/*
	Starts firing animation montage
	Actual weapon firing is done through notifiers set up in the montage
	This is so that firing happens at the correct part of the animation
	For most guns this is probably at the start so you can put a notifier at the begining
	But in the case of say a burst weapons, we would want 3 notifiers in our animation, and that allows this fucntion to be used for all firing type, since the firing is handled by notifiers.
	Also, some weapons dont fire right at the start, some may have a small charge up animation.
	Basically, handling the firing through notifiers in the montage makes this function flexible enough to be used with ANY weapon, regardless of how it functions.
	*/
	// Dont fire if we are still doing another animation

	// Dont fire if we are out of ammo
	if (current_magazine_ammo_count <= 0)
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("No ammo, skipping fire"));
		return;
	}


	UE_LOG(Weapons, Verbose, LOG_TEXT("Fire weapon initiated"));

	try
	{
		Validity::check_value<UAnimMontage>(fire_animation_montage, "No fire animation montage for weapon");
		Validity::check_value<UAnimInstance>(anim_instance, "No anim instance found");
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs (aborting fire)"), e.what());
		return;
	}

	// Check if either of the animations are currently playing
	// If one of them is nullptr, this is fine as this if statement will still work as intended, it will just be slightly less effienct
	// This is because it will check all montages to see if any of them are active if nullptr is passed in.  This is fine becuase in the final game
	// there SHOULD NEVER be nullptr.
	if (anim_instance->Montage_IsActive(fire_animation_montage) || anim_instance->Montage_IsActive(reload_animation_montage))
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Can't fire now, another process is taking place"));
		return;
	}
	UE_LOG(Weapons, Verbose, LOG_TEXT("Playing weapon fire animation montage"));
	float duration = anim_instance->Montage_Play(fire_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

	// If duration == 0.f that means an error
	if (duration == 0.f)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("Could not play weapon fire animation montage"));
		return;
	}
}


void AWeapon::reload() const
{
	/*
	Plays reload animation montage.
	The actual reloading is done through a notifier.
	This is to ensure the reloading happens at the right part of the animation sequence (ex: when the new mag is inserted into the gun)
	*/
	// Dont reload if ammo is full
	if (current_magazine_ammo_count >= magazine_size)
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Weapon magazine is full, skipping reload"));
		return;
	}

	UE_LOG(Weapons, Verbose, LOG_TEXT("Reload weapon initiated"));
	try
	{
		Validity::check_value<UAnimMontage>(reload_animation_montage, "No reload animation montage for weapon");
		Validity::check_value<UAnimInstance>(anim_instance, "No anim instance found");
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs (aborting reload)"), e.what());
		return;
	}

	// Check if either of the animations are currently playing
	// If one of them is nullptr, this is fine as this if statement will still work as intended, it will just be slightly less effienct
	// This is because it will check all montages to see if any of them are active if nullptr is passed in.  This is fine becuase in the final game
	// there SHOULD NEVER be nullptr.
	if (anim_instance->Montage_IsActive(fire_animation_montage) || anim_instance->Montage_IsActive(reload_animation_montage))
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Can't reload now, another process is taking place"));
		return;
	}

	UE_LOG(Weapons, Verbose, LOG_TEXT("Playing weapon reload animation montage"));
	float duration = anim_instance->Montage_Play(reload_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

	// If duration == 0.f that means an error
	if (duration == 0.f)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("Could not play weapon reload animation montage"));
		return;
	}
}


unsigned int AWeapon::get_magazine_size() const
{
	return magazine_size;
}


unsigned int AWeapon::get_current_magazine_ammo_count() const
{
	return current_magazine_ammo_count;
}


unsigned int AWeapon::get_max_ammo_stock_pile_count() const
{
	return max_ammo_stock_pile_count;
}


unsigned int AWeapon::get_current_ammo_stock_pile_count() const
{
	return current_ammo_stock_pile_count;
}


unsigned int AWeapon::get_scan_distance() const
{
	return scan_distance;
}


float AWeapon::get_weapon_damage() const
{
	return weapon_damage;
}


void AWeapon::refill_magazine()
{
	UE_LOG(Weapons, Log, LOG_TEXT("Refilling weapon magazine"));

	unsigned int remaining_mag_space = magazine_size - current_magazine_ammo_count;

	// If we have enough ammo to top it off
	if (current_ammo_stock_pile_count >= remaining_mag_space)
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Topping off weapon magazine"));
		current_ammo_stock_pile_count -= remaining_mag_space;
		current_magazine_ammo_count = magazine_size;
	}
	// If we have some ammo, but not enough to top it of
	else if (current_ammo_stock_pile_count > 0)
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Putting last ammo in weapon magazine"));
		current_magazine_ammo_count += current_ammo_stock_pile_count;
		current_ammo_stock_pile_count = 0;
	}
	// If we have none :(
	else
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("No ammo to refill"));
	}
}
