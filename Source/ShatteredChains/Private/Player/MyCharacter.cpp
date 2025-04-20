#include "MyCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"              
#include "Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputMappingContext.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ShatteredChains/Logging.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShatteredChains/CustomTraceChannels.h"
#include "MedKit.h"
#include "EngineUtils.h"



// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Initialize movement states and stamina
	bIsCrouched = false;
	bIsSprinting = false;
	bIsSliding = false;
	bCanRoll = true;
	CurrentStamina = Stamina; // Set initial stamina
    CurrentWeapon = nullptr;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

    camera_timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Camera Timeline"));
    
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

    actor_name = "Player";


    leg_shot_damage_multiplier = 1;
    leg_shot_speed_multiplier = 1;
    leg_shot_accuracy_multiplier = 1;
    foot_shot_damage_multiplier = 1;
    foot_shot_speed_multiplier = 1;
    foot_shot_accuracy_multiplier = 1;
    arm_shot_damage_multiplier = 1;
    arm_shot_speed_multiplier = 1;
    arm_shot_accuracy_multiplier = 1;
    hand_shot_damage_multiplier = 1;
    hand_shot_speed_multiplier = 1;
    hand_shot_accuracy_multiplier = 1;
    torso_shot_damage_multiplier = 1;
    torso_shot_speed_multiplier = 1;
    torso_shot_accuracy_multiplier = 1;
    head_shot_damage_multiplier = 1;
    head_shot_speed_multiplier = 1;
    head_shot_accuracy_multiplier = 1;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

    DefaultFOV = Camera->FieldOfView;
    TargetFOV = DefaultFOV;

    const TObjectPtr<UPhysicsAsset> physics_asset = GetMesh()->GetPhysicsAsset();
    add_stats_modifiers(physics_asset, &stats_modifiers);

    stats_modifiers["Hips"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Hips"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Hips"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["LeftUpLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftUpLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftUpLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["LeftLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["LeftFoot"]->set_damage_multiplier(foot_shot_damage_multiplier);
    stats_modifiers["LeftFoot"]->set_speed_multiplier(foot_shot_speed_multiplier);
    stats_modifiers["LeftFoot"]->set_accuracy_multiplier(foot_shot_accuracy_multiplier);

    stats_modifiers["RightUpLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightUpLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightUpLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["RightFoot"]->set_damage_multiplier(foot_shot_damage_multiplier);
    stats_modifiers["RightFoot"]->set_speed_multiplier(foot_shot_speed_multiplier);
    stats_modifiers["RightFoot"]->set_accuracy_multiplier(foot_shot_accuracy_multiplier);

    stats_modifiers["Spine1"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine1"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine1"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);
    
    stats_modifiers["Spine2"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine2"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine2"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["LeftArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["LeftArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["LeftArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);
    
    stats_modifiers["LeftHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["Head"]->set_damage_multiplier(head_shot_damage_multiplier);
    stats_modifiers["Head"]->set_speed_multiplier(head_shot_speed_multiplier);
    stats_modifiers["Head"]->set_accuracy_multiplier(head_shot_accuracy_multiplier);

    stats_modifiers["RightArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["RightArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["RightArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["RightHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    if (death_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for death in '%s'"), *actor_name);
    }
    if (leg_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for leg shot in '%s'"), *actor_name);
    }
    if (arm_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for arm shot in '%s'"), *actor_name);
    }
    if (hand_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for hand shot in '%s'"), *actor_name);
    }
    if (torso_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for torso shot in '%s'"), *actor_name);
    }
    if (head_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for head shot in '%s'"), *actor_name);
    }

    if (foot_shot_sounds.Num() == 0)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No sound effects for head shot in '%s'"), *actor_name);
    }
    
    // Add bones to sound map 
    for (int32 i = 0; i < physics_asset->SkeletalBodySetups.Num(); i++)
    {
        sound_map.Add(physics_asset->SkeletalBodySetups[i]->BoneName);
    }
    sound_map.Add("dead");
    
    sound_map["dead"] = death_sounds;
    sound_map["Hips"] = torso_shot_sounds;
    sound_map["LeftUpLeg"] = leg_shot_sounds;
    sound_map["LeftLeg"] = leg_shot_sounds;
    sound_map["LeftFoot"] = foot_shot_sounds;
    sound_map["RightUpLeg"] = leg_shot_sounds;
    sound_map["RightLeg"] = leg_shot_sounds;
    sound_map["RightFoot"] = foot_shot_sounds;
    sound_map["Spine1"] = torso_shot_sounds;
    sound_map["Spine2"] = torso_shot_sounds;
    sound_map["LeftArm"] = arm_shot_sounds;
    sound_map["LeftHand"] = hand_shot_sounds;
    sound_map["Head"] = head_shot_sounds;
    sound_map["RightArm"] = arm_shot_sounds;
    sound_map["RightHand"] = hand_shot_sounds;
}

// Player Input bindings
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called!"));

    // Add input mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {


        // Get Local Player Subsystem
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (Subsystem)
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMapping, 0);
            UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Mapping Applied!"));
        }
    }

    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Fire Weapon
        Input->BindAction(FireAction, ETriggerEvent::Ongoing, this, &AMyCharacter::FireWeapon);
        Input->BindAction(FireAction, ETriggerEvent::Completed, this, &AMyCharacter::EndFireWeapon);

        //Reload Weapon
        Input->BindAction(ReloadAction, ETriggerEvent::Started, this, &AMyCharacter::ReloadWeapon);

        // Interact
        Input->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyCharacter::Interact);

        // Wasd 
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

        // Look 
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

        // Jump
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);

        // Bind crouch toggle
        Input->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMyCharacter::ToggleCrouch);

        // Bind slide action
        Input->BindAction(SlideAction, ETriggerEvent::Started, this, &AMyCharacter::StartSlide);

        // Bind jump action (trigger slide jump if sliding)
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::SlideJump);

        // Bind prone hold
        Input->BindAction(ProneAction, ETriggerEvent::Started, this, &AMyCharacter::ToggleProne);

        // Bind sprint start
        Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyCharacter::StartSprint);

        // Bind sprint stop 
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);

        // Bind roll action "alt"
        Input->BindAction(RollAction, ETriggerEvent::Started, this, &AMyCharacter::StartRoll);

        // bind toggle inventory ui "i"
        Input->BindAction(IA_ToggleInventory, ETriggerEvent::Triggered, this, &AMyCharacter::ToggleInventory);
        UE_LOG(LogTemp, Warning, TEXT("ToggleInventory binding successful!"));

        // bind log invetory "o"
        Input->BindAction(IA_LogInventory, ETriggerEvent::Triggered, this, &AMyCharacter::LogInventory);

        // weapon slots
        Input->BindAction(IA_WeaponSlot1, ETriggerEvent::Triggered, this, &AMyCharacter::HandleWeaponSlotInput, 1);

        Input->BindAction(IA_WeaponSlot2, ETriggerEvent::Triggered, this, &AMyCharacter::HandleWeaponSlotInput, 2);

        Input->BindAction(IA_WeaponSlot3, ETriggerEvent::Triggered, this, &AMyCharacter::HandleWeaponSlotInput, 3);

        Input->BindAction(IA_DropWeapon, ETriggerEvent::Triggered, this, &AMyCharacter::DropWeapon);

        // drop weapon
        Input->BindAction(IA_DropWeapon, ETriggerEvent::Triggered, this, &AMyCharacter::DropWeapon);

        // use healthkit/take out
        Input->BindAction(IA_UseHealthKit, ETriggerEvent::Started, this, &AMyCharacter::ToggleMedKit);

        // scope in
        Input->BindAction(ScopeAction, ETriggerEvent::Started, this, &AMyCharacter::StartZoom);

        // scope out
        Input->BindAction(ScopeAction, ETriggerEvent::Completed, this, &AMyCharacter::StopZoom);

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent NOT found!"));
    }

    // Bind axis mappings for movement
    /*
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
    */ 

    /*
    // Bind action mappings for jumping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);
    */

    // Bind action mappings for sprinting and crouching
    // PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
    // PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
    // PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &AMyCharacter::ToggleCrouch);

    // Bind action mappings for slide and roll
    // PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMyCharacter::StartSlide);
    // PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMyCharacter::StartRoll);

    // Prone
    // PlayerInputComponent->BindAction("ToggleProne", IE_Pressed, this, &AMyCharacter::ToggleProne);

    // Slide Jump
    //PlayerInputComponent->BindAction("SlideJump", IE_Pressed, this, &AMyCharacter::SlideJump);

}


UInventoryComponent* AMyCharacter::GetInventoryComponent() const
{
    return InventoryComponent;
}

void AMyCharacter::FireWeapon()
{
    if (bIsHoldingMedKit)
    {
        if (InventoryComponent && InventoryComponent->HasItem("MedKit", 1))
        {
            InventoryComponent->RemoveItem("MedKit", 1);
            HealthComponent->set_health(HealthComponent->get_max_health());

            if (medkit_heal_sound)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), medkit_heal_sound);
            }

            GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
            ResetMovementDebuffs();
            bIsHoldingMedKit = false;

            UE_LOG(LogTemp, Log, TEXT("Used MedKit and restored health."));
            return;
        }

        UE_LOG(LogTemp, Warning, TEXT("Tried to use MedKit but none left."));
        return;
    }

    if (CurrentWeapon)
    {
        // This condition stops it from spamming the out of ammo sound on full-auto weapons, which plays when the gun is fired without ammo
        // Here we will just fire once, then set has_fired_weapon to true, so all guns, regardless of full-auto or not, operate as if they were semi-auto ONLY WHEN
        // there is no ammo.
        // These if statements can probably be combined, since they run the same body, but the expression needed to make it work as intended
        // Would not be as readable
        if (CurrentWeapon->get_current_magazine_ammo_count() == 0)
        {
            // This can not be added to the above if-statement with an &&
            // If we did that, after we dry fire once, the following else if block would still run on full-auto weapons
            // leading to the weapon empty sound being spammed.
            // This way, code execution does not reach the else if below
            if (!has_fired_weapon)
            {
                CurrentWeapon->fire(); // Calls the fire function in Weapon.cpp
                UE_LOG(Player, Log, LOG_TEXT("Fired weapon with no ammo: %s"), *CurrentWeapon->GetName());
                has_fired_weapon = true;
            }
        }
        else if (!has_fired_weapon || CurrentWeapon->is_full_auto())
        {
            CurrentWeapon->fire(); // Calls the fire function in Weapon.cpp
            UE_LOG(Player, Log, LOG_TEXT("Fired weapon: %s"), *CurrentWeapon->GetName());
            has_fired_weapon = true;
        }
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("No weapon equipped to fire."));
    }
}


void AMyCharacter::EndFireWeapon()
{
    has_fired_weapon = false;
}

// scope in
void AMyCharacter::StartZoom()
{
    bIsZooming = true;

    if (CurrentWeapon)
    {
        TargetFOV = CurrentWeapon->GetZoomFOV();

        if (scope_in_sound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), scope_in_sound);
        }
    }
    else
    {
        // No weapon equipped, default to full FOV
        TargetFOV = DefaultFOV;
    }
}


//scope out
void AMyCharacter::StopZoom()
{
    bIsZooming = false;
    TargetFOV = DefaultFOV;
}

//Toggle medkit
void AMyCharacter::ToggleMedKit(const FInputActionValue& Value)
{
    if (!InventoryComponent) return;

    if (bIsZooming)
    {
        StopZoom();
    }

    // Unequip current weapon if holding one
    if (CurrentWeapon)
    {
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon = nullptr;
        CurrentEquippedWeaponSlot = -1;
    }

    // Toggle medkit state
    if (bIsHoldingMedKit)
    {
        bIsHoldingMedKit = false;
        if (EquippedMedKit)
        {
            EquippedMedKit->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            EquippedMedKit->SetActorHiddenInGame(true);
            EquippedMedKit->SetActorEnableCollision(false);
        }
        UE_LOG(Player, Log, TEXT("MedKit unequipped."));
        return;
    }

    if (InventoryComponent->HasItem("MedKit", 1))
    {
        bIsHoldingMedKit = true;

        if (!EquippedMedKit)
        {
            // Spawn medkit actor once and reuse it
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            EquippedMedKit = GetWorld()->SpawnActor<AMedKit>(AMedKit::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        }

        if (EquippedMedKit)
        {
            EquippedMedKit->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("MedKitSocket")); // or use WeaponSocket
            EquippedMedKit->SetActorHiddenInGame(false);
            EquippedMedKit->SetActorEnableCollision(false);
        }
        if (item_pickup_sound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
        }


        UE_LOG(Player, Log, TEXT("MedKit equipped."));
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("No MedKit in inventory."));
    }
}


void AMyCharacter::UseEquippedMedkit()
{
    UE_LOG(LogTemp, Log, TEXT("Attempting to use medkit"));

    if (!InventoryComponent->HasItem("MedKit", 1))
    {
        UE_LOG(LogTemp, Warning, TEXT("No MedKit in inventory."));
        return;
    }

    UHealthComponent* HealthComp = get_health_component();
    if (HealthComp)
    {
        HealthComp->set_health(HealthComp->get_max_health());

        if (medkit_heal_sound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), medkit_heal_sound);
        }
    }

    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    ResetMovementDebuffs();

    InventoryComponent->RemoveItem("MedKit", 1);
    CurrentEquippedWeaponSlot = -1;
    CurrentWeapon = nullptr;

    UE_LOG(LogTemp, Log, TEXT("Used MedKit: healed and removed movement debuffs."));
}




void AMyCharacter::ReloadWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->reload(); // Calls the reload function in Weapon.cpp
        UE_LOG(Player, Log, TEXT("Reloaded weapon: %s"), *CurrentWeapon->GetName());
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("No weapon equipped to reload."));
    }
}

void AMyCharacter::PickUpWeapon(AWeapon* PickedUpWeapon)
{
    if (PickedUpWeapon && InventoryComponent)
    {
        FName WeaponID = FName(*PickedUpWeapon->GetName());

        // Check if we have space for the weapon before picking it up
        if (InventoryComponent->AddWeapon(WeaponID))
        {
            UE_LOG(LogTemp, Log, TEXT("Picked up weapon: %s"), *WeaponID.ToString());

            // Item pickup sound
            if (item_pickup_sound)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
            }

            // Equip weapon if it's the first one picked up
            if (!CurrentWeapon)
            {
                EquipWeapon(PickedUpWeapon);
                UE_LOG(LogTemp, Log, TEXT("Equipped weapon: %s"), *WeaponID.ToString());
            }

            // Hide weapon from the world
            PickedUpWeapon->SetActorHiddenInGame(true);
            PickedUpWeapon->SetActorEnableCollision(false);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Weapon slots full! Cannot pick up %s"), *WeaponID.ToString());
        }
    }
}

// handle weapon equipping slot 1-3
void AMyCharacter::HandleWeaponSlotInput(int32 Slot)
{
    if (!InventoryComponent) return;

    // Cancel zoom if active
    if (bIsZooming)
    {
        StopZoom();
    }

    // Toggle medkit if Slot == 4
    if (Slot == 4)
    {
        if (bIsHoldingMedKit)
        {
            bIsHoldingMedKit = false;

            if (EquippedMedKit)
            {
                EquippedMedKit->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                EquippedMedKit->SetActorHiddenInGame(true);
                EquippedMedKit->SetActorEnableCollision(false);
            }

            UE_LOG(Player, Log, TEXT("MedKit unequipped due to weapon switch."));
        }
        else if (InventoryComponent->HasItem("MedKit", 1))
        {
            bIsHoldingMedKit = true;
            CurrentWeapon = nullptr;
            CurrentEquippedWeaponSlot = -1;
            UE_LOG(Player, Log, TEXT("[Slot 4][MedKit][EQUIPPED]"));
        }
        if (item_pickup_sound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
        }
        else
        {
            UE_LOG(Player, Warning, TEXT("No MedKit in inventory to equip."));
        }
        return;
    }

    // Unequip medkit if switching to a weapon
    if (bIsHoldingMedKit)
    {
        bIsHoldingMedKit = false;
        UE_LOG(Player, Log, TEXT("MedKit unequipped due to weapon switch."));
    }

    const int32 SlotIndex = Slot - 1;
    const TArray<FName>& WeaponSlots = InventoryComponent->GetWeaponSlots();

    if (SlotIndex >= WeaponSlots.Num() || WeaponSlots[SlotIndex].IsNone())
    {
        UE_LOG(Player, Warning, TEXT("[Slot %d][Empty][CANNOT EQUIP]"), Slot);
        return;
    }

    if (CurrentEquippedWeaponSlot == Slot)
    {
        if (CurrentWeapon)
        {
            CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            CurrentWeapon->SetActorHiddenInGame(true);
            CurrentWeapon->SetActorEnableCollision(false);
            UE_LOG(Player, Log, TEXT("Unequipped weapon: %s"), *CurrentWeapon->GetName());
        }

        CurrentWeapon = nullptr;
        CurrentEquippedWeaponSlot = -1;
        UE_LOG(Player, Log, TEXT("[Slot %d][%s][UNEQUIPPED]"), Slot, *WeaponSlots[SlotIndex].ToString());
        return;
    }


    AWeapon* FoundWeapon = nullptr;
    for (TActorIterator<AWeapon> It(GetWorld()); It; ++It)
    {
        if (It->GetName() == WeaponSlots[SlotIndex].ToString())
        {
            FoundWeapon = *It;
            break;
        }
    }

    if (FoundWeapon)
    {
        //  First, detach and hide the currently equipped weapon
        if (CurrentWeapon)
        {
            CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            CurrentWeapon->SetActorHiddenInGame(true);
            CurrentWeapon->SetActorEnableCollision(false);
            UE_LOG(Player, Log, TEXT("Swapping out weapon: %s"), *CurrentWeapon->GetName());
        }

        //  Now equip the new one
        CurrentWeapon = FoundWeapon;
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->SetActorHiddenInGame(false);

        CurrentEquippedWeaponSlot = Slot;
        UE_LOG(Player, Log, TEXT("[Slot %d][%s][EQUIPPED]"), Slot, *FoundWeapon->GetName());
    }
    if (item_pickup_sound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
    }


}


//drop weapon
void AMyCharacter::DropWeapon()
{
    if (!CurrentWeapon || CurrentEquippedWeaponSlot == -1)
    {
        UE_LOG(Player, Warning, TEXT("No weapon equipped to drop."));
        return;
    }

    UE_LOG(Player, Log, TEXT("[Slot %d][%s][DROPPED]"), CurrentEquippedWeaponSlot, *CurrentWeapon->GetName());

    // Place the weapon a little in front of the player
    FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 200.0f + FVector(0, 0, -40.0f);
    FRotator DropRotation = FRotator::ZeroRotator;

    CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentWeapon->SetActorLocation(DropLocation);
    CurrentWeapon->SetActorRotation(DropRotation);
    CurrentWeapon->SetActorEnableCollision(true);
    CurrentWeapon->SetActorHiddenInGame(false);
    CurrentWeapon->SetOwner(nullptr); // Ensure it's no longer "held"

    // Remove it from inventory
    InventoryComponent->RemoveWeapon(CurrentEquippedWeaponSlot - 1);
    CurrentWeapon = nullptr;
    CurrentEquippedWeaponSlot = -1;
}






// Interact Function
void AMyCharacter::Interact()
{
    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + (Camera->GetForwardVector() * 1500.0f);
    FHitResult HitResult;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = HitResult.GetActor();
        if (!HitActor) return;

        UE_LOG(Player, Log, TEXT("Line Trace Hit Actor: %s"), *HitActor->GetName());

        // Handle MedKit pickup
        if (AMedKit* MedKit = Cast<AMedKit>(HitActor))
        {
            if (InventoryComponent->AddItem("MedKit", EItemType::HealthKit, 1))
            {
                UE_LOG(Player, Log, TEXT("Picked up MedKit via Interact"));
                MedKit->Destroy();

                // Item pickup sound
                if (item_pickup_sound)
                {
                    UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
                }
            }
            else
            {
                UE_LOG(Player, Warning, TEXT("Could not pick up MedKit: Inventory full"));
            }
            return;
        }

        // Handle Weapon pickup
        if (AWeapon* HitWeapon = Cast<AWeapon>(HitActor))
        {
            UE_LOG(Player, Log, TEXT("Weapon Detected: %s"), *HitWeapon->GetName());

            EquipWeapon(HitWeapon);
            HitWeapon->SetActorHiddenInGame(true);
            HitWeapon->SetActorEnableCollision(false);
            return;
        }
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("No interactable object detected."));
    }
}







// equip weapon
void AMyCharacter::EquipWeapon(AWeapon* weapon)
{
    if (!weapon)
    {
        return;
    }

    if (CurrentWeapon)
    {
        UE_LOG(Player, Log, TEXT("Current Weapon: %s will be replaced with %s"), *CurrentWeapon->GetName(), *weapon->GetName());
        CurrentWeapon->Destroy(); // Destroy the old weapon if needed
    }
    else
    {
        UE_LOG(Player, Log, TEXT("Equipping first weapon: %s"), *weapon->GetName());
    }

    // Equip new weapon
    CurrentWeapon = weapon;
    CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
    CurrentWeapon->SetActorEnableCollision(false);
}




// WASD Movement Function
void AMyCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementInput = Value.Get<FVector2D>();

    if (Controller && MovementInput.SizeSquared() > 0.0f)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        ControlRotation.Pitch = 0.0f; // Ignore pitch for movement direction

        FVector ForwardDirection = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

        // Apply movement
        AddMovementInput(ForwardDirection, MovementInput.Y);
        AddMovementInput(RightDirection, MovementInput.X);

        // Play crouch walk animation if moving while crouched
        if (bIsCrouched)
        {
            if (CrouchWalkMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(CrouchWalkMontage))
            {
                PlayAnimMontage(CrouchWalkMontage);
            }
        }
        else if (bIsSprinting)
        {
            // Ensure sprinting animations play correctly
            if (SprintAnimMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(SprintAnimMontage))
            {
                PlayAnimMontage(SprintAnimMontage);
            }
        }
    }
    else
    {
        // If crouched and not moving, play crouch idle animation
        if (bIsCrouched && CrouchIdleMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(CrouchIdleMontage))
        {
            PlayAnimMontage(CrouchIdleMontage);
        }
    }
}




void AMyCharacter::Look(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();
    if (!IsValid(Controller)) return;

    float EffectiveSensitivity = MouseSensitivity;

    if (bIsZooming && CurrentWeapon)
    {
        EffectiveSensitivity *= CurrentWeapon->ZoomMouseSensitivity;
    }

    AddControllerYawInput(InputVector.X * EffectiveSensitivity);
    AddControllerPitchInput(InputVector.Y * EffectiveSensitivity);
}


// Jump Function
void AMyCharacter::Jump()
{
    if (bIsSliding)
    {
        SlideJump(); // Perform slide jump
    }
    else
    {
        ACharacter::Jump(); // Perform Unreal's default jump logic

        if (JumpAnimMontage)
        {
            float Duration = PlayAnimMontage(JumpAnimMontage, 1.0f);
            UE_LOG(Player, Log, TEXT("Jump animation montage triggered, duration: %f"), Duration);
        }
        else
        {
            UE_LOG(Player, Warning, TEXT("JumpAnimMontage is missing!"));
        }
    }
}




// Forward and Backwards functions
void AMyCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

// Right and Left Functions
void AMyCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

// Jump Functions with double Jump Logic for future reference
void AMyCharacter::StartJump()
{
    if (JumpCount < MaxJumpCount)
    {
        bPressedJump = true;
        JumpCount++;

        ACharacter::Jump(); // Perform Unreal's jump logic

        if (JumpAnimMontage)
        {
            PlayAnimMontage(JumpAnimMontage);
            UE_LOG(Player, Log, TEXT("StartJump triggered: Jump animation playing"));
        }
    }
}



// Stop Jump Function
void AMyCharacter::StopJump()
{
    bPressedJump = false;

    // Stop jump animation (if necessary, adjust based on behavior)
    if (JumpAnimMontage)
    {
        StopAnimMontage(JumpAnimMontage);
    }

    UE_LOG(Player, Log, TEXT("StopJump triggered"));
}

// Function to register landing
void AMyCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    JumpCount = 0; // Reset jump count on landing

    UE_LOG(Player, Log, TEXT("Landed: JumpCount reset, ensuring no animation override"));
}


// Start Sprint Function with Stamina Management
void AMyCharacter::StartSprint()
{
    if (CurrentStamina > 0.0f && !bIsSprinting) // checks stamina
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // increase speed
        UE_LOG(Player, Log, TEXT("Sprint started: CurrentStamina = %f"), CurrentStamina);
    }
}


// Stop Sprint Function
void AMyCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; // restore normal speed
    UE_LOG(Player, Log, TEXT("Sprint stopped"));
}

// Stamina Caculation Function
void AMyCharacter::UpdateStamina(float DeltaTime)
{
    if (bIsSprinting && CurrentStamina > 0.0f)
    {
        CurrentStamina -= StaminaDrainRate * DeltaTime;
        UE_LOG(Player, Log, TEXT("UpdateStamina: Sprinting - CurrentStamina = %f"), CurrentStamina);

        if (CurrentStamina <= 0.0f)
        {
            StopSprint();
            UE_LOG(Player, Warning, TEXT("UpdateStamina: Stamina depleted, stopping sprint"));
        }
    }
    else if (CurrentStamina < Stamina)
    {
        CurrentStamina += StaminaRecoveryRate * DeltaTime;
        UE_LOG(Player, Log, TEXT("UpdateStamina: Recovering - CurrentStamina = %f"), CurrentStamina);
    }
}

// Crouch Toggle Function
void AMyCharacter::ToggleCrouch()
{
    UCapsuleComponent* Capsule = GetCapsuleComponent();

    UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());

    if (bIsCrouched)
    {
        // Play stand-up animation before transitioning to standing state
        if (StandUpMontage)
        {
            PlayAnimMontage(StandUpMontage);
        }

        // Restore original height and speed when uncrouching
        Capsule->SetCapsuleHalfHeight(88.0f); // Default height
        UnCrouch();
        bIsCrouched = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        UE_LOG(Player, Log, TEXT("UnCrouch: Height restored to %f"), Capsule->GetUnscaledCapsuleHalfHeight());
    }
    else
    {
        // Play crouch enter animation
        if (CrouchEnterMontage)
        {
            PlayAnimMontage(CrouchEnterMontage);
        }

        // Reduce height and speed when crouching
        Capsule->SetCapsuleHalfHeight(44.0f); // Half of the default height
        Crouch();
        bIsCrouched = true;
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

        UE_LOG(Player, Log, TEXT("Crouch: Height reduced to %f"), Capsule->GetUnscaledCapsuleHalfHeight());
    }
}




// Toggle Prone Function
void AMyCharacter::ToggleProne()
{
    if (bIsProne)
    {
        // Exit prone
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsProne = false;

        // Restore normal collision and movement speed
        GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f); // Default height
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        UE_LOG(Player, Log, TEXT("ToggleProne: Exited prone mode"));

        // Play prone exit animation
        if (ProneAnimMontage)
        {
            PlayAnimMontage(ProneAnimMontage, 1.0f);
        }
    }
    else
    {
        // Enter prone
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking); // Use NavWalking for crawling-like behavior
        bIsProne = true;

        // Adjust collision and movement speed
        GetCapsuleComponent()->SetCapsuleHalfHeight(0.0f); // Reduced height for prone
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed / 2; // Half crouch speed for prone

        UE_LOG(Player, Log, TEXT("ToggleProne: Entered prone mode"));
        // Play prone enter animation
        if (ProneAnimMontage)
        {
            PlayAnimMontage(ProneAnimMontage, 1.0f);
        }
    }
}

// Sliding Function
void AMyCharacter::StartSlide()
{
    // Ensure sliding only triggers while sprinting and not already sliding
    if (bIsSprinting && !bIsSliding)
    {
        bIsSliding = true;
        bCanSlideJump = true; // Allow slide jump

        // Reduce capsule height for sliding
        GetCapsuleComponent()->SetCapsuleHalfHeight(22.0f); 
        GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;

        // Calculate the slide direction
        FVector SlideDirection = GetLastMovementInputVector();
        if (SlideDirection.IsZero())
        {
            // Default to forward direction if no movement input
            SlideDirection = GetActorForwardVector();
        }
        SlideDirection.Normalize(); // Ensure the direction vector is normalized

        // Launch the character in the slide direction
        LaunchCharacter(SlideDirection * SlideSpeed, true, true);

        // Play slide animation
        if (SlideMontage)
        {
            PlayAnimMontage(SlideMontage);
        }

        // Set a timer to stop sliding after SlideDuration
        GetWorld()->GetTimerManager().SetTimer(SlideStopTimer, this, &AMyCharacter::StopSlide, SlideDuration, false);

        UE_LOG(Player, Log, TEXT("Slide started in direction: %s"), *SlideDirection.ToString());
    }
}



// Stop sliding function
void AMyCharacter::SlideJump()
{
    if (bCanSlideJump) // Ensure slide jump is allowed
    {
        // Get the character's current velocity
        FVector CurrentVelocity = GetCharacterMovement()->Velocity;

        // Scale the velocity to adjust the horizontal distance
        float MomentumScalingFactor = 1.0f; // horizontal distance value 
        FVector JumpDirection = CurrentVelocity * MomentumScalingFactor;

        // Set a reduced upward force
        JumpDirection.Z = FMath::Clamp(SlideJumpForce.Z, 200.0f, 600.0f); // Vertical distance value

        // Launch the character
        LaunchCharacter(JumpDirection, true, true);

        // Stop sliding after the jump
        StopSlide();

        bCanSlideJump = false; // Disable slide jump until the next slide
        UE_LOG(Player, Log, TEXT("Slide jump performed: %s"), *JumpDirection.ToString());
    }
}



void AMyCharacter::StopSlide()
{
    if (bIsSliding)
    {
        // Restore capsule height and speed
        GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f); // Default height
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; // Restore walk speed

        bIsSliding = false; // Reset sliding state

        UE_LOG(Player, Log, TEXT("Slide stopped"));
    }
}


// Rolling Function or dodge
void AMyCharacter::StartRoll()
{
    if (bCanRoll) // Ensure rolling is allowed
    {
        bCanRoll = false; // Prevent spamming rolls

        // Determine the roll direction based on last movement input
        FVector RollDirection = GetLastMovementInputVector(); // Get the last input direction
        if (RollDirection.IsZero())
        {
            // Default to forward direction if no movement input
            RollDirection = GetActorForwardVector();
        }
        RollDirection.Normalize(); // Ensure the direction vector is normalized

        // Launch the character in the roll direction
        LaunchCharacter(RollDirection * 900.0f + FVector(0, 0, 300.0f), true, true); // Adjust speed and height

        // Play roll animation
        if (RollAnimMontage)
        {
            PlayAnimMontage(RollAnimMontage, 1.0f);
        }

        bIsDodging = true;

        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // disable hit

        // Set a cooldown timer for rolling
        const float RollCooldown = 2.0f; // Adjust cooldown time
        GetWorld()->GetTimerManager().SetTimer(RollCooldownTimer, this, &AMyCharacter::EnableRolling, RollCooldown, false);

        UE_LOG(Player, Log, TEXT("Started Roll in direction: %s"), *RollDirection.ToString());
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("Roll is on cooldown"));
    }
}

// Mostly to stop animation roll
void AMyCharacter::StopRoll()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    UE_LOG(Player, Log, TEXT("StopRoll triggered"));

    // Allow rolling again after cooldown
    //GetWorld()->GetTimerManager().SetTimer(RollCooldownTimer, this, &AMyCharacter::EnableRolling, 1.0f, false);
}

// Spam Prevention on rolling, needs testing.
void AMyCharacter::EnableRolling()
{
    bCanRoll = true;
    UE_LOG(Player, Log, TEXT("Rolling re-enabled"));

    bIsDodging = false;
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // restore hit

}

// Toggle Inventory UI (placeholder, can be linked to an actual UI)
void AMyCharacter::ToggleInventory(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("ToggleInventory function triggered!"));
    if (!Value.Get<bool>()) return;

    bIsInventoryOpen = !bIsInventoryOpen;
    UE_LOG(LogTemp, Log, TEXT("========Inventory %s========="), bIsInventoryOpen ? TEXT("Opened") : TEXT("Closed"));

    if (bIsInventoryOpen && InventoryComponent)
    {
        // Log weapon slots
        const TArray<FName>& WeaponSlots = InventoryComponent->GetWeaponSlots();
        for (int32 i = 0; i < 3; ++i)
        {
            FString WeaponName = (i < WeaponSlots.Num() && !WeaponSlots[i].IsNone()) ? WeaponSlots[i].ToString() : TEXT("Empty");
            UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d: %s"), i + 1, *WeaponName);
        }

        // Always show Medkit slot
        bool bHasMedkit = InventoryComponent->HasItem("MedKit", 1);
        FString MedkitStatus = bHasMedkit ? TEXT("Full") : TEXT("Empty");
        FString EquippedStatus = (bIsHoldingMedKit && bHasMedkit) ? TEXT(" (Equipped)") : TEXT("");
        UE_LOG(LogTemp, Log, TEXT("Medkit Slot: %s%s"), *MedkitStatus, *EquippedStatus);
    }
}





// Logs current inventory items when "O" is pressed
void AMyCharacter::LogInventory(const FInputActionValue& Value)
{
    if (!Value.Get<bool>()) return; // Ensure input is valid

    if (!InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Inventory Component Found!"));
        return;
    }

    TMap<FName, FInventoryItem> InventoryItems = InventoryComponent->GetInventory();
    
    UE_LOG(Player, Log, LOG_TEXT("---- Current Inventory ----"));
    for (const auto &ItemPair : InventoryItems)
    {
        FInventoryItem Item = ItemPair.Value;
        UE_LOG(Player, Log, LOG_TEXT("Item: %s, Quantity: %d"), *Item.ItemID.ToString(), Item.Quantity);
    }
}

void AMyCharacter::Mantle()
{
    // Define the start point (player's eye level) and end point (forward and up)
    FVector Start = GetActorLocation() + FVector(0, 0, 50.0f); // Slightly above character's feet
    FVector ForwardVector = GetActorForwardVector(); 
    FVector End = Start + (ForwardVector * 100.0f) + FVector(0, 0, 100.0f); // Forward and upwards

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Perform a line trace to detect a ledge
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        // Check if a ledge is detected
        if (Hit.bBlockingHit)
        {
            // Launch the character upwards and slightly forward to simulate mantling
            FVector MantleTarget = Hit.ImpactPoint + FVector(0, 0, 50.0f); // Adjust to move onto the ledge
            FVector LaunchVelocity = (MantleTarget - GetActorLocation()) * 2.0f; // Scale for speed
            LaunchCharacter(LaunchVelocity, true, true);

            // Play mantling animation
            if (MantleAnimMontage)
            {
                PlayAnimMontage(MantleAnimMontage);
            }
            UE_LOG(Player, Log, TEXT("Mantle triggered: Target = %s"), *MantleTarget.ToString());
        }
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("Mantle failed: No ledge detected"));
    }
}



// Function determines whether a mantle action is currently possible by checking for ledge presence.
bool AMyCharacter::CanMantle() const
{
    FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 100.0f) + FVector(0, 0, 100.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Perform a line trace to check for a ledge
    return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.bBlockingHit;
}


// Function checks if the character is close enough to a wall, a prerequisite for initiating mantling.
bool AMyCharacter::IsNearWall() const
{
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 50.0f); // Shorter distance to detect walls

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Perform a line trace to check for walls
    return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.bBlockingHit;
}


// Function is triggered when the player begins grabbing a ledge.
void AMyCharacter::StartLedgeGrab()
{
    if (CanMantle())
    {
        // Disable movement temporarily
        GetCharacterMovement()->DisableMovement();

        // play a ledge grab animation
        if (LedgeGrabAnimMontage)
        {
            PlayAnimMontage(LedgeGrabAnimMontage);
        }
    }
}


// Function completes the ledge grab and moves the character onto the ledge.
void AMyCharacter::PullUpFromLedge()
{
    FVector TargetLocation = GetActorLocation() + FVector(0, 0, 100.0f); // Adjust height
    SetActorLocation(TargetLocation);

    // Re-enable movement
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    UE_LOG(Player, Log, TEXT("PullUpFromLedge triggered: New Location = %s"), *TargetLocation.ToString());

    // play a pull-up animation
    if (PullUpAnimMontage)
    {
        PlayAnimMontage(PullUpAnimMontage);
    }
}


// Function identifies whether there�s a ledge above the character that can be climbed.
bool AMyCharacter::DetectLedge()
{
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 100.0f) + FVector(0, 0, 100.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.bBlockingHit;
}


// Wall Jump Function
void AMyCharacter::WallJump()
{
    FVector Start = GetActorLocation();
    FVector End = Start + (GetActorForwardVector() * 200.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        LaunchCharacter(FVector(0, 0, 600), true, true);
        JumpCount = 1; // Reset jump count after wall jump
        UE_LOG(Player, Log, TEXT("WallJump triggered"));
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("WallJump failed: No wall detected"));
    }
}

// reset movement debuffs
void AMyCharacter::ResetMovementDebuffs()
{
    leg_shot_speed_multiplier = 1.0f;
    foot_shot_speed_multiplier = 1.0f;
    UE_LOG(LogTemp, Log, TEXT("Movement debuffs reset."));
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Smooth zoom interpolation
    float EffectiveZoomSpeed = ZoomInterpSpeed;
    if (CurrentWeapon)
    {
        EffectiveZoomSpeed = CurrentWeapon->ZoomInterpSpeed;
    }
    Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, EffectiveZoomSpeed));


}


void AMyCharacter::restart_current_level() const
{
    const UWorld* world = GetWorld();

    if (world == nullptr)
    {
        UE_LOG(Player, Error, LOG_TEXT("Could not get world to start reset timer"));
        return;
    }
    
    const ULevel* current_level = world->GetCurrentLevel();
    const FName current_level_name = *current_level->GetPackage()->GetName();
    UGameplayStatics::OpenLevel(this, current_level_name);
    UE_LOG(Player, Log, LOG_TEXT("Restarting level"));
}


void AMyCharacter::UpdateCameraPosition(const float value)
{
    const FVector new_camera_location = FMath::Lerp(player_death_camera_start_location, player_death_camera_end_location, value); 
    const FRotator new_camera_rotation = FMath::Lerp(player_death_camera_start_rotation, player_death_camera_end_rotation, value); 
    UE_LOG(Player, VeryVerbose, LOG_TEXT("Updating camera position for dead player '%s' (value=%f, position=%s, rotation=%s)"), *actor_name, value, *(new_camera_location.ToString()), *(new_camera_rotation.ToString()));
    Camera->SetRelativeLocationAndRotation(new_camera_location, new_camera_rotation);
}



void AMyCharacter::on_death(const AActor* killed_by, const bool play_death_sound)
{
    // Play random death sound
    const TArray<TObjectPtr<USoundBase>> *sounds = sound_map.Find("dead");
    // Don't have to worry about nullptr in second condition because of short circuit evaluation
    if (sounds == nullptr || sounds->Num() == 0)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No death sounds for '%s'"), *actor_name);
    } else
    {
        if (play_death_sound)
        {
            const int num_sounds = sounds->Num();
            const int sound_to_play = FMath::RandHelper(num_sounds);
            USoundBase* sound = (*sounds)[sound_to_play];
            UGameplayStatics::PlaySound2D(GetWorld(), sound, 1, 1, 0, nullptr, this, false);
            UE_LOG(Enemy, Log, LOG_TEXT("Playing death sound '%s' for enemy '%s'"), *(sound->GetPathName()), *actor_name);
        }
    }

    UE_LOG(Player, Log, LOG_TEXT("Player dead"));

    const UWorld* world = GetWorld();

    if (world == nullptr)
    {
        UE_LOG(Player, Error, LOG_TEXT("Could not get world to start reset timer"));
        return;
    }
    
    // Start level reset timer
    FTimerHandle restart_level_timer_handle;
    world->GetTimerManager().SetTimer(restart_level_timer_handle, this, &AMyCharacter::restart_current_level, 5, false);


    // Move camera up to look down at player
    player_death_camera_start_location = Camera->GetRelativeLocation();
    player_death_camera_end_location = player_death_camera_start_location + FVector(0, 0, 300);

    player_death_camera_start_rotation = Camera->GetRelativeRotation();
    player_death_camera_end_rotation = FRotator(-90, 0, 0);

    // Disable player input when they are dead
    if (APlayerController* player_controller = GetWorld()->GetFirstPlayerController())
    {
        UE_LOG(Player, Log, LOG_TEXT("Disabling input for dead player '%s'"), *actor_name);
        this->DisableInput(player_controller);    
    } else
    {
        UE_LOG(Player, Error, LOG_TEXT("No player controller for player '%s'"), *actor_name);
    }
    
    if (camera_curve)
    {
        FOnTimelineFloat timeline_callback;
        timeline_callback.BindUFunction(this, FName("UpdateCameraPosition"));
        camera_timeline->AddInterpFloat(camera_curve, timeline_callback);

        UE_LOG(Player, Log, LOG_TEXT("Beginning camera transition for dead player '%s'"), *actor_name);
        // Set this to false so that the camera can rotate
        Camera->bUsePawnControlRotation = false;
        camera_timeline->PlayFromStart();
    } else
    {
        UE_LOG(Player, Warning, LOG_TEXT("No camera curve for player '%s'"), *actor_name);
    }
    

    // Make player "dead"
    USkeletalMeshComponent* mesh = GetMesh();

    if (mesh == nullptr)
    {
        UE_LOG(Player, Error, LOG_TEXT("Player '%s' has no USkeletalMeshComponent"), *actor_name);
    }
    else
    {
        // Make it un-shootable
        GetMesh()->SetCollisionResponseToChannel(ShootableChannel, ECollisionResponse::ECR_Ignore);
        UE_LOG(Player, Verbose, LOG_TEXT("Player '%s' is no longer shootable"), *actor_name);

        // Stop all animations
        UAnimInstance *anim_instance = GetMesh()->GetAnimInstance();
        if (anim_instance == nullptr)
        {
            UE_LOG(Player, Warning, LOG_TEXT("Player '%s' has no animation instance"), *actor_name);
        }
        else
        {
            anim_instance->StopAllMontages(0);
        }
        
        UE_LOG(Player, Verbose, LOG_TEXT("Stopped all animation montages for '%s'"), *actor_name);

        // Ragdoll player
        mesh->SetCollisionProfileName(FName(TEXT("Ragdoll")));
        mesh->SetSimulatePhysics(true);
        UE_LOG(Player, Log, LOG_TEXT("Player '%s' made ragdoll"), *actor_name);
    }
}


UHealthComponent* AMyCharacter::get_health_component() const
{
    return HealthComponent;
}

// This should return a location to start a hitscan from
FVector AMyCharacter::get_hitscan_start_location() const
{
    return Camera->GetComponentLocation();
}

// This should return a Vector in the direction to fire
FVector AMyCharacter::get_hitscan_direction() const
{
    return Camera->GetForwardVector();
}


TObjectPtr<UInventoryComponent> AMyCharacter::get_inventory_component() const
{
    return InventoryComponent;
}


FString AMyCharacter::get_default_actor_name() const
{
    return FString("Player");
}


const TMap<FName, TObjectPtr<UStatsModifier>>* AMyCharacter::get_bone_collider_stats_modifiers() const
{
    return &stats_modifiers;
}



void AMyCharacter::hit_bone(const AActor* hit_by, const FName bone_name, float weapon_damage)
{
    // If the player is currently dodging, ignore the hit entirely
    if (bIsDodging)
    {
        UE_LOG(Player, Log, TEXT("Dodged attack! Ignored hit to bone: %s"), *bone_name.ToString());
        return;
    }

    if (!stats_modifiers.Contains(bone_name))
    {
        UE_LOG(BoneCollision, Error, LOG_TEXT("Enemy '%s' does not have modifier for bone '%s'.  Its possible its capsule component is blocking the shot, set trace channel Shootable to Ignore on the capsule component."), *actor_name, *(bone_name.ToString()));
        return;
    }
    
    // Get the modifier
    const TObjectPtr<UStatsModifier> modifier = stats_modifiers[bone_name];
    
    UE_LOG(BoneCollision, Log, LOG_TEXT("Player '%s' hit in bone '%s'"), *actor_name, *(bone_name.ToString()));

    // Apply other modifier stats, like speed, accuracy, etc.
    UCharacterMovementComponent* movement_component = GetCharacterMovement();
    const float old_movement_speed = movement_component->MaxWalkSpeed;
    
    movement_component->MaxWalkSpeed *= modifier->get_speed_multiplier();

    UE_LOG(BoneCollision, Log, LOG_TEXT("Changing player '%s' speed: %f -> %f"), *actor_name, old_movement_speed, movement_component->MaxWalkSpeed);

    const float old_damage = weapon_damage;
    weapon_damage *= modifier->get_damage_multiplier();
    UE_LOG(BoneCollision, Log, LOG_TEXT("Stats modifiers for '%s' in group '%s': DAMAGE_MUL=%f"), *actor_name, *(bone_name.ToString()), modifier->get_damage_multiplier());
    UE_LOG(BoneCollision, Log, LOG_TEXT("Damage for '%s' modified from %f -> %f"), *actor_name, old_damage, weapon_damage);
    HealthComponent->deal_damage(hit_by, weapon_damage);

    // Here play audio
    // We only play audio if we are not dead, because then there will be two audio effects
    // This one and the dead one
    if (!HealthComponent->dead())
    {
        // Play random sound depending on where we were hit
        const TArray<TObjectPtr<USoundBase>> *sounds = sound_map.Find(bone_name);
        // Don't have to worry about nullptr in second condition because of short circuit evaluation
        if (sounds == nullptr || sounds->Num() == 0)
        {
            UE_LOG(Enemy, Error, LOG_TEXT("No damage sounds for bone '%s' on player '%s'"), *(bone_name.ToString()), *actor_name);
        } else
        {
            const int num_sounds = sounds->Num();
            const int sound_to_play = FMath::RandHelper(num_sounds);
            USoundBase* sound = (*sounds)[sound_to_play];
            UGameplayStatics::PlaySound2D(GetWorld(), sound, 1, 1, 0, nullptr, this, false);
            UE_LOG(Enemy, Verbose, LOG_TEXT("Playing damage sound '%s' for bone '%s' on player '%s'"), *(sound->GetPathName()), *(bone_name.ToString()), *actor_name);
        }
    }
    
}



// UPROPERTY for input mapping context.
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* MoveForwardAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* MoveRightAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* JumpAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* SprintAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* CrouchAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* SlideAction;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
UInputAction* RollAction;

UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
UAnimMontage* SlideJumpAnimMontage;