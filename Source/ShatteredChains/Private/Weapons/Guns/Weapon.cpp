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
	current_mag_ammo_count = 0;
	max_ammo_stock_pile_count = 0;
	current_ammo_stock_pile_count = 0;
	weapon_skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	RootComponent = weapon_skeletal_mesh_component;
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
		Validity::check_value<UAnimMontage>(reload_animation_montage, "No reload animation montage for weapon");
		UE_LOG(Weapons, Verbose, LOG_TEXT("Weapon has animation montages"));
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
	}

	// Set ammo
	refill_magazine();
}


void AWeapon::fire() const
{
	// Weapons Animation instance
	UAnimInstance* anim_instance;
	try
	{
		Validity::check_value<UAnimMontage>(fire_animation_montage, "No fire animation montage for weapon");
		anim_instance = Validity::check_value<UAnimInstance>(this->weapon_skeletal_mesh_component->GetAnimInstance(), "No anim instance found");
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
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
	// Weapons Animation instance
	UAnimInstance* anim_instance;
	try
	{
		Validity::check_value<UAnimMontage>(reload_animation_montage, "No reload animation montage for weapon");
		anim_instance = Validity::check_value<UAnimInstance>(this->weapon_skeletal_mesh_component->GetAnimInstance(), "No anim instance found");
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Weapons, Error, LOG_TEXT("%hs"), e.what());
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


unsigned int AWeapon::get_current_mag_ammo_count() const
{
	return current_mag_ammo_count;
}


unsigned int AWeapon::get_max_ammo_stock_pile_count() const
{
	return max_ammo_stock_pile_count;
}


unsigned int AWeapon::get_current_ammo_stock_pile_count() const
{
	return current_ammo_stock_pile_count;
}


void AWeapon::refill_magazine()
{
	unsigned int remaining_mag_space = magazine_size - current_mag_ammo_count;

	// If we have enough ammo to top it off
	if (current_ammo_stock_pile_count >= remaining_mag_space)
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Topping off weapon magazine"));
		current_ammo_stock_pile_count -= remaining_mag_space;
		current_mag_ammo_count = magazine_size;
	}
	// Otherwise
	else
	{
		UE_LOG(Weapons, Verbose, LOG_TEXT("Putting last ammo in weapon magazine"));
		current_mag_ammo_count += current_ammo_stock_pile_count;
		current_ammo_stock_pile_count = 0;
	}
}
