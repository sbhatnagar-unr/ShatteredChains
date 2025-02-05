// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Player/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(Weapon);

// Sets default values
AWeapon::AWeapon()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    magazine_size = 0;
    current_magazine_ammo_count = 0;
    max_ammo_stock_pile_count = 0;
    current_ammo_stock_pile_count = 0;

    has_fire_animation_montage = false;
    has_reload_animation_montage = false;
    
    // Root component
    root_scene_component = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = root_scene_component;
   
    // Skeletal Mesh
    weapon_skeletal_mesh_component = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    weapon_skeletal_mesh_component->SetupAttachment(RootComponent);

    // Interaction Sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetSphereRadius(100.0f); // Adjustable interaction range
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    InteractionSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    actor_name = default_actor_name;

    // Bind the overlap event
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

    // Check for mesh
    if (weapon_skeletal_mesh_component->GetSkeletalMeshAsset() == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No skeletal mesh for weapon %s"), *actor_name);
    }
    UE_LOG(Weapon, Verbose, LOG_TEXT("Weapon has skeletal mesh"));


    // Check for animations
    if (fire_animation_montage == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No fire animation montage for weapon %s"), *actor_name);
    }
    else
    {
        has_fire_animation_montage = true;
    }
    UE_LOG(Weapon, Verbose, LOG_TEXT("Fire animation montage: %s"), *fire_animation_montage.GetFullName());

    
    if (reload_animation_montage == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No reload animation montage for weapon %s"), *actor_name);
    }
    else
    {
        has_reload_animation_montage = true;
    }
    UE_LOG(Weapon, Verbose, LOG_TEXT("Reload animation montage: %s"), *reload_animation_montage.GetFullName());

        
    anim_instance = weapon_skeletal_mesh_component->GetAnimInstance();
    if (anim_instance == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No anim instance found for weapon %s"), *actor_name);
    }

    // Set ammo
    current_ammo_stock_pile_count = max_ammo_stock_pile_count;
    refill_magazine();
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character)
    {
        UE_LOG(LogTemp, Log, TEXT("Player overlapped with: %s"), *GetName());

        if (GetOwner() != nullptr)
        {
            UE_LOG(Weapon, Verbose, LOG_TEXT("Could not pick up weapon because another Actor is holding it"));
            return;
        }
        
        // Use proxy function
        Character->PickUpWeapon(this);

        // Optional: Make the weapon disappear from the world
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
}



void AWeapon::fire() const
{
    /*
    Starts firing animation montage
    Actual weapon firing is done through notifiers set up in the montage
    This is so that firing happens at the correct part of the animation
    For most guns this is probably at the start so you can put a notifier at the beginning
    But in the case of say a burst weapons, we would want 3 notifiers in our animation, and that allows this function to be used for all firing type, since the firing is handled by notifiers.
    Also, some weapons don't fire right at the start, some may have a small charge up animation.
    Basically, handling the firing through notifiers in the montage makes this function flexible enough to be used with ANY weapon, regardless of how it functions.
    */

    // Don't fire if we are out of ammo
    if (current_magazine_ammo_count <= 0)
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("No ammo, skipping fire"));
        return;
    }

    // Don't fire if we don't have a montage
    // We can't anyway since firing happens in notifiers
    if (!has_fire_animation_montage)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No fire weapon animation montage, aborting fire"));
        return;
    }

    // Don't fire if we don't have a animation instance
    // We can't anyway since firing happens in notifiers
    if (anim_instance == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No animation instance, aborting fire"));
        return;
    }
    
    // Check if either of the animations are currently playing
    if (anim_instance->Montage_IsPlaying(fire_animation_montage))
    {
        UE_LOG(Weapon, VeryVerbose, LOG_TEXT("Can't fire now, already firing"));
        return;
    }

    if (anim_instance->Montage_IsPlaying(reload_animation_montage))
    {
        UE_LOG(Weapon, VeryVerbose, LOG_TEXT("Can't fire now, already reloading"));
        return;
    }
    
    UE_LOG(Weapon, Verbose, LOG_TEXT("Playing weapon fire animation montage"));
    
    const float duration = anim_instance->Montage_Play(fire_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

    // If duration == 0.f that means an error
    if (duration == 0.f)
    {
        UE_LOG(Weapon, Error, LOG_TEXT("Could not play weapon fire animation montage"));
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
        UE_LOG(Weapon, Verbose, LOG_TEXT("Weapon magazine is full, skipping reload (current_magazine_ammo_count=%d, magazine_size=%d)"), current_magazine_ammo_count, magazine_size);
        return;
    }

    if (current_ammo_stock_pile_count <= 0)
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("No ammo to reload with (current_ammo_stock_pile_count=%d)"), current_ammo_stock_pile_count);
        return;
    }

    // Don't reload if we don't have a montage
    // We can't anyway since firing happens in notifiers
    if (!has_reload_animation_montage)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No reload weapon animation montage, aborting fire"));
        return;
    }

    // Don't reload if we don't have a animation instance
    // We can't anyway since firing happens in notifiers
    if (anim_instance == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No animation instance, aborting reload"));
        return;
    }

    // Check if either of the animations are currently playing
    if (anim_instance->Montage_IsPlaying(fire_animation_montage))
    {
        UE_LOG(Weapon, VeryVerbose, LOG_TEXT("Can't reload now, already firing"));
        return;
    }

    if (anim_instance->Montage_IsPlaying(reload_animation_montage))
    {
        UE_LOG(Weapon, VeryVerbose, LOG_TEXT("Can't reload now, already reloading"));
        return;
    }
    

    UE_LOG(Weapon, Verbose, LOG_TEXT("Playing weapon reload animation montage"));
    const float duration = anim_instance->Montage_Play(reload_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

    // If duration == 0.f that means an error
    if (duration == 0.f)
    {
        UE_LOG(Weapon, Error, LOG_TEXT("Could not play weapon reload animation montage"));
    }
}


void AWeapon::decrement_mag_ammo_count()
{
    if (current_magazine_ammo_count > 0)
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("Removed ammo from magazine on %s (%d->%d)"), *actor_name, current_magazine_ammo_count, current_magazine_ammo_count-1);
        current_magazine_ammo_count--;
    }
    else
    {
        UE_LOG(Weapon, Warning, LOG_TEXT("Weapon mag ammo decrement triggered but mag ammo count was 0"));
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
    UE_LOG(Weapon, Log, LOG_TEXT("Refilling weapon magazine"));

    const unsigned int remaining_mag_space = magazine_size - current_magazine_ammo_count;

    // If we have enough ammo to top it off
    if (current_ammo_stock_pile_count >= remaining_mag_space)
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("Topping off weapon magazine (%d ammo)"), remaining_mag_space);
        current_ammo_stock_pile_count -= remaining_mag_space;
        current_magazine_ammo_count = magazine_size;
    }
    // If we have some ammo, but not enough to top it of
    else if (current_ammo_stock_pile_count > 0)
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("Putting last ammo in weapon magazine (%d ammo)"), current_ammo_stock_pile_count);
        current_magazine_ammo_count += current_ammo_stock_pile_count;
        current_ammo_stock_pile_count = 0;
    }
    // If we have none :(
    else
    {
        UE_LOG(Weapon, Verbose, LOG_TEXT("No ammo to refill"));
    }
}


FString AWeapon::get_default_actor_name() const
{
    return default_actor_name;
}

