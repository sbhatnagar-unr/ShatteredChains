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
#include "Weapons/MeleeWeapon.h"
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

    ApplyMovementSpeed();

    if (FistWeaponClass)
    {
        FActorSpawnParameters Params;
        Params.Owner = this;
        AMeleeWeapon* Fist = GetWorld()->SpawnActor<AMeleeWeapon>(FistWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        if (Fist)
        {
            Fist->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
            Fist->SetActorHiddenInGame(true); 
            Fist->SetActorEnableCollision(false);
            CurrentWeapon = nullptr;
            FistWeapon = Fist;
        }
    }

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

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMapping, 0);
            UE_LOG(LogTemp, Warning, TEXT("MappingContext applied again on BeginPlay"));
        }
    }

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
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::CancelLedgeLatch);

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

        // equip melee slot
        Input->BindAction(IA_WeaponSlot5, ETriggerEvent::Triggered, this, &AMyCharacter::HandleWeaponSlotInput, 5);

        // quick melee action
        Input->BindAction(IA_QuickMelee, ETriggerEvent::Started, this, &AMyCharacter::QuickMelee);

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

            ApplyMovementSpeed();
            ResetMovementDebuffs();
            bIsHoldingMedKit = false;

            UE_LOG(LogTemp, Log, TEXT("Used MedKit and restored health."));
            return;
        }

        UE_LOG(LogTemp, Warning, TEXT("Tried to use MedKit but none left."));
        return;
    }

    // Sword Fire
    if (CurrentEquippedWeaponSlot == 5 && EquippedMeleeWeapon)
    {
        EquippedMeleeWeapon->Punch();
        return;
    }

    // Gun Fire
    if (CurrentWeapon)
    {
        if (CurrentWeapon->get_current_magazine_ammo_count() == 0)
        {
            if (!has_fired_weapon)
            {
                CurrentWeapon->fire();
                UE_LOG(Player, Log, LOG_TEXT("Fired weapon with no ammo: %s"), *CurrentWeapon->GetName());
                has_fired_weapon = true;
            }
        }
        else if (!has_fired_weapon || CurrentWeapon->is_full_auto())
        {
            CurrentWeapon->fire();
            UE_LOG(Player, Log, LOG_TEXT("Fired weapon: %s"), *CurrentWeapon->GetName());
            has_fired_weapon = true;
        }
    }
    // Bare fists fallback (only if no weapon/sword equipped)
    else if (FistWeapon)
    {
        FistWeapon->Punch();
        UE_LOG(LogTemp, Log, TEXT("Punch executed using fists."));
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
    TargetFOV = ZoomedFOV;  // Lower FOV to "zoom in"

    if (CurrentWeapon)
    {
        USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetWeaponMesh();
        if (WeaponMesh)
        {
            WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

            // Attach weapon to CameraComponent directly
            WeaponMesh->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

            // Move and rotate the weapon relative to camera
            WeaponMesh->SetRelativeLocation(ZoomedWeaponOffset);
            WeaponMesh->SetRelativeRotation(ZoomedWeaponRotation);
        }
    }
}




//scope out
void AMyCharacter::StopZoom()
{
    bIsZooming = false;
    TargetFOV = DefaultFOV;  // Reset FOV

    if (CurrentWeapon)
    {
        USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetWeaponMesh();
        if (WeaponMesh)
        {
            WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

            // Reattach weapon back to player WeaponSocket
            WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
        }
    }
}


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

    // Unequip melee sword
    if (EquippedMeleeWeapon)
    {
        EquippedMeleeWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        EquippedMeleeWeapon->SetActorHiddenInGame(true);
        EquippedMeleeWeapon->SetActorEnableCollision(false);
        EquippedMeleeWeapon = nullptr;
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
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            EquippedMedKit = GetWorld()->SpawnActor<AMedKit>(AMedKit::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        }

        if (EquippedMedKit)
        {
            EquippedMedKit->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
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

    ResetMovementDebuffs();
    ApplyMovementSpeed();

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

bool AMyCharacter::PickUpWeapon(AWeapon* PickedUpWeapon)
{
    if (PickedUpWeapon && InventoryComponent)
    {
        FName WeaponID = FName(*PickedUpWeapon->GetName());

        // Check if we have space for the weapon before picking it up
        if (InventoryComponent->AddWeapon(WeaponID))
        {
            UE_LOG(LogTemp, Log, TEXT("Picked up weapon: %s"), *WeaponID.ToString());

            if (item_pickup_sound)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
            }

            const TArray<FName>& WeaponSlots = InventoryComponent->GetWeaponSlots();
            int32 AssignedSlot = -1;
            for (int32 i = 0; i < WeaponSlots.Num(); ++i)
            {
                if (WeaponSlots[i] == WeaponID)
                {
                    AssignedSlot = i + 1;
                    break;
                }
            }

            PickedUpWeapon->SetActorEnableCollision(false);
            return true; 
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Weapon slots full! Cannot pick up %s"), *WeaponID.ToString());
        }
    }

    return false; 
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

    // Always detach and hide currently equipped weapon or melee weapon
    if (CurrentWeapon)
    {
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon = nullptr;
    }

    if (EquippedMeleeWeapon)
    {
        EquippedMeleeWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        EquippedMeleeWeapon->SetActorHiddenInGame(true);
        EquippedMeleeWeapon->SetActorEnableCollision(false);
        EquippedMeleeWeapon = nullptr;
    }

    // Handle melee weapon (slot 5)
    if (Slot == 5)
    {
        FName SwordID = InventoryComponent->GetMeleeWeaponID();
        if (SwordID.IsNone())
        {
            UE_LOG(LogTemp, Warning, TEXT("No melee weapon to equip."));
            CurrentEquippedWeaponSlot = -1;
            return;
        }

        for (TActorIterator<AMeleeWeapon> It(GetWorld()); It; ++It)
        {
            if (It->GetName() == SwordID.ToString())
            {
                EquippedMeleeWeapon = *It;
                EquippedMeleeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("MeleeSocket"));
                EquippedMeleeWeapon->SetActorHiddenInGame(false);
                EquippedMeleeWeapon->SetActorEnableCollision(false);

                if (EquippedMeleeWeapon->MeshComponent)
                {
                    EquippedMeleeWeapon->MeshComponent->SetSimulatePhysics(false);
                }

                EquippedMeleeWeapon->SetOwner(this);
                EquippedMeleeWeapon->SetOwnerPawn(this);
                CurrentEquippedWeaponSlot = 5;

                if (EquippedMeleeWeapon->SwordDrawSound)
                {
                    UGameplayStatics::PlaySound2D(GetWorld(), EquippedMeleeWeapon->SwordDrawSound);
                }

                UE_LOG(LogTemp, Log, TEXT("[Slot 5][MeleeWeapon][EQUIPPED]"));
                return;
            }
        }
        return;
    }

    // Handle regular guns (slots 1-3)
    const int32 SlotIndex = Slot - 1;
    const TArray<FName>& WeaponSlots = InventoryComponent->GetWeaponSlots();

    if (SlotIndex >= WeaponSlots.Num() || WeaponSlots[SlotIndex].IsNone())
    {
        CurrentEquippedWeaponSlot = -1;
        UE_LOG(Player, Log, TEXT("[Slot %d][Empty][UNEQUIPPED]"), Slot);
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
        CurrentWeapon = FoundWeapon;
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->SetActorHiddenInGame(false);

        CurrentEquippedWeaponSlot = Slot;

        UE_LOG(Player, Log, TEXT("[Slot %d][%s][EQUIPPED]"), Slot, *FoundWeapon->GetName());

        if (weapon_equip_sound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), weapon_equip_sound);
        }
    }
}




// quick melee
void AMyCharacter::QuickMelee()
{
    if (!FistWeapon) return;

    if (CurrentEquippedWeaponSlot == 5 && EquippedMeleeWeapon)
    {
        EquippedMeleeWeapon->Punch();
    }
    else if (!FistWeapon->bIsPunching)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && FistWeapon->PunchMontage1)
        {
            float Duration = AnimInstance->Montage_Play(FistWeapon->PunchMontage1);

            FistWeapon->bIsPunching = true;

            // Reset after animation
            FTimerHandle PunchResetTimer;
            GetWorldTimerManager().SetTimer(PunchResetTimer, [this]()
                {
                    if (FistWeapon)
                    {
                        FistWeapon->bIsPunching = false;
                    }
                }, Duration, false);

            // Delay punch damage
            FTimerHandle DamageTimer;
            GetWorldTimerManager().SetTimer(DamageTimer, FistWeapon, &AMeleeWeapon::ApplyDamage, FistWeapon->DamageDelay, false);

            if (FistWeapon->PunchSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), FistWeapon->PunchSound, GetActorLocation());
            }
        }
    }
}




//drop weapon
void AMyCharacter::DropWeapon()
{
    if (CurrentEquippedWeaponSlot == -1)
    {
        UE_LOG(Player, Warning, TEXT("No weapon equipped to drop."));
        return;
    }

    const UWorld* World = GetWorld();
    if (!World) return;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (World->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - FVector(0, 0, 1000), ECC_Visibility, QueryParams))
    {
        const FVector GunDropLocation = HitResult.Location + GetActorForwardVector() * 200.0f + FVector(0, 0, 2.0f); // Guns
        const FVector MeleeDropLocation = HitResult.Location + GetActorForwardVector() * 150.0f + FVector(0, 0, 100.0f); // Melee
        FRotator DropRotation = GetActorRotation();
        DropRotation.Pitch += 90.0f;

        // Handle melee weapon (sword)
        if (CurrentEquippedWeaponSlot == 5 && EquippedMeleeWeapon)
        {
            UE_LOG(Player, Log, TEXT("[Slot 5][%s][DROPPED]"), *EquippedMeleeWeapon->GetName());

            EquippedMeleeWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            EquippedMeleeWeapon->SetActorHiddenInGame(false);
            EquippedMeleeWeapon->SetActorEnableCollision(true);

            if (EquippedMeleeWeapon->MeshComponent)
            {
                // Move with teleport at correct melee location
                EquippedMeleeWeapon->MeshComponent->SetWorldLocation(MeleeDropLocation, false, nullptr, ETeleportType::TeleportPhysics);
                EquippedMeleeWeapon->MeshComponent->SetWorldRotation(DropRotation, false, nullptr, ETeleportType::TeleportPhysics);

                EquippedMeleeWeapon->MeshComponent->SetSimulatePhysics(true);

                // FIX COLLISION: make sure it blocks the world so it lands
                EquippedMeleeWeapon->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EquippedMeleeWeapon->MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
                EquippedMeleeWeapon->MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

                // (Optional) Add slight impulse
                //EquippedMeleeWeapon->MeshComponent->AddImpulse(FVector(0, 0, 300), NAME_None, true);
              //EquippedMeleeWeapon->MeshComponent->AddAngularImpulseInDegrees(FVector(0, 0, 500), NAME_None, true);
            }

            EquippedMeleeWeapon->SetOwner(nullptr);

            if (InventoryComponent)
            {
                InventoryComponent->RemoveMeleeWeapon();
            }
            EquippedMeleeWeapon = nullptr;
            CurrentEquippedWeaponSlot = -1;
        }
        // Handle regular guns
        else if (CurrentWeapon)
        {
            UE_LOG(Player, Log, TEXT("[Slot %d][%s][DROPPED]"), CurrentEquippedWeaponSlot, *CurrentWeapon->GetName());

            CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            CurrentWeapon->SetActorLocation(GunDropLocation);
            CurrentWeapon->SetActorRotation(DropRotation);
            CurrentWeapon->SetActorEnableCollision(true);
            CurrentWeapon->SetActorHiddenInGame(false);
            CurrentWeapon->SetOwner(nullptr);

            if (InventoryComponent)
            {
                InventoryComponent->RemoveWeapon(CurrentEquippedWeaponSlot - 1);
            }
            CurrentWeapon = nullptr;
            CurrentEquippedWeaponSlot = -1;
        }
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("Could not find location to drop weapon."));
    }
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

        // Sword Pickup 
        if (AMeleeWeapon* HitMelee = Cast<AMeleeWeapon>(HitActor))
        {
            FName SwordID = FName(*HitMelee->GetName());

            if (!InventoryComponent->GetMeleeWeaponID().IsNone())
            {
                UE_LOG(LogTemp, Warning, TEXT("Already carrying melee weapon."));
                return;
            }

            if (InventoryComponent->AddWeapon(SwordID))
            {
                UE_LOG(LogTemp, Log, TEXT("Picked up sword: %s"), *SwordID.ToString());
                HitMelee->SetActorHiddenInGame(true);
                HitMelee->SetActorEnableCollision(false);
                if (item_pickup_sound)
                {
                    UGameplayStatics::PlaySound2D(GetWorld(), item_pickup_sound);
                }
            }
            return;
        }

        // Medkit Pickup 
        if (AMedKit* MedKit = Cast<AMedKit>(HitActor))
        {
            if (!InventoryComponent->HasItem("MedKit", 1) && InventoryComponent->AddItem("MedKit", EItemType::HealthKit, 1, 1))
            {
                UE_LOG(Player, Log, TEXT("Picked up MedKit via Interact"));
                MedKit->Destroy();
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

        // Gun Pickup 
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
    if (!weapon) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy(); // Optional: or Detach
    }

    CurrentWeapon = weapon;

    // Make sure ownership and visibility are correct
    CurrentWeapon->SetOwner(this); // ← CRITICAL so it's "yours"
    CurrentWeapon->SetActorHiddenInGame(false); // ← Force visible before attach

    CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
    CurrentWeapon->SetActorEnableCollision(false);

    // Play equip sound
    if (weapon_equip_sound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), weapon_equip_sound);
    }

    // Find and assign equipped slot
    const TArray<FName>& WeaponSlots = InventoryComponent->GetWeaponSlots();
    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        if (WeaponSlots[i] == FName(*weapon->GetName()))
        {
            CurrentEquippedWeaponSlot = i + 1;
            break;
        }
    }

    UE_LOG(Player, Log, TEXT("Weapon equipped: %s"), *weapon->GetName());
}




// WASD Movement Function
void AMyCharacter::Move(const FInputActionValue& Value)
{
    if (!Controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Move() called without Controller valid"));
        return;
    }


    FVector2D MovementInput = Value.Get<FVector2D>();

    if (MovementInput.IsNearlyZero())
    {
        return;
    }

    FRotator ControlRotation = Controller->GetControlRotation();
    ControlRotation.Pitch = 0.0f;

    FVector ForwardDirection = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);
    FVector RightDirection = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementInput.Y);
    AddMovementInput(RightDirection, MovementInput.X);

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
        SlideJump();
        return;
    }

    if (bIsLatchedToLedge)
    {
        PullUpFromLedge();
        return;
    }

    if (bIsCrouched || bIsProne)
    {
        UCapsuleComponent* Capsule = GetCapsuleComponent();
        UCharacterMovementComponent* MovementComp = GetCharacterMovement();

        if (Capsule)
        {
            Capsule->SetCapsuleHalfHeight(88.0f);
        }
        if (MovementComp)
        {
            MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
        }

        ApplyMovementSpeed();
        UnCrouch(); // UnCrouch is safe — Unreal guards internally
    }

    if (GetCharacterMovement() && !GetCharacterMovement()->IsFalling() && CanJump())
    {
        ACharacter::Jump();
        bHasJumpedOnce = true;
        /*
        if (JumpAnimMontage)
            PlayAnimMontage(JumpAnimMontage);
            */
        UE_LOG(Player, Log, TEXT("Jump successful"));

    }
}



void AMyCharacter::CancelLedgeLatch()
{
    if (bIsLatchedToLedge)
    {
        bIsLatchedToLedge = false;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling); // Resume falling
        UE_LOG(Player, Log, TEXT("Latch canceled: dropped from ledge"));
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
        /*
        if (JumpAnimMontage)
        {
            PlayAnimMontage(JumpAnimMontage);
            UE_LOG(Player, Log, TEXT("StartJump triggered: Jump animation playing"));
        }
        */
    }
}



// Stop Jump Function
void AMyCharacter::StopJump()
{
    bPressedJump = false;
    /*
    // Stop jump animation (if necessary, adjust based on behavior)
    if (JumpAnimMontage)
    {
        StopAnimMontage(JumpAnimMontage);
    }
    */
    UE_LOG(Player, Log, TEXT("StopJump triggered"));
}

// Function to register landing
void AMyCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    bHasJumpedOnce = false;
    JumpCount = 0;
    bIsLatchedToLedge = false;

    // Safety: Reset capsule height if still crouched
    if (bIsCrouched)
    {
        GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
    }
    if (bIsProne)
    {
        GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsProne = false;
    }
}


// Start Sprint Function with Stamina Management
void AMyCharacter::StartSprint()
{
    if (CurrentStamina > 0.0f && !bIsSprinting) // checks stamina
    {
        bIsSprinting = true;
        ApplyMovementSpeed(); // increase speed
        UE_LOG(Player, Log, TEXT("Sprint started: CurrentStamina = %f"), CurrentStamina);
    }
}


// Stop Sprint Function
void AMyCharacter::StopSprint()
{
    bIsSprinting = false;
    ApplyMovementSpeed(); // restore normal speed
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

    if (!Capsule || !MovementComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("ToggleCrouch(): CapsuleComponent or MovementComponent is NULL!"));
        return;
    }

    if (bIsCrouched)
    {
        /*
        if (StandUpMontage)
        {
            PlayAnimMontage(StandUpMontage);
        }
        */
        Capsule->SetCapsuleHalfHeight(88.0f);
        UnCrouch();
        bIsCrouched = false;
        ApplyMovementSpeed();
    }
    else
    {
        /*
        if (CrouchEnterMontage)
        {
            PlayAnimMontage(CrouchEnterMontage);
        }
        */
        Capsule->SetCapsuleHalfHeight(44.0f);
        Crouch();
        bIsCrouched = true;
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
    }
}





// Toggle Prone Function
void AMyCharacter::ToggleProne()
{
    UCapsuleComponent* Capsule = GetCapsuleComponent();
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

    if (!Capsule || !MovementComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("ToggleProne(): CapsuleComponent or MovementComponent is NULL!"));
        return;
    }

    if (bIsProne)
    {
        // Exit prone
        MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsProne = false;

        Capsule->SetCapsuleHalfHeight(88.0f); // Reset height
        ApplyMovementSpeed();

        UE_LOG(Player, Log, TEXT("ToggleProne: Exited prone mode"));
        /*
        if (ProneAnimMontage)
        {
            PlayAnimMontage(ProneAnimMontage, 1.0f);
        }
        */
    }
    else
    {
        // Enter prone
        MovementComponent->SetMovementMode(EMovementMode::MOVE_NavWalking);
        bIsProne = true;

        Capsule->SetCapsuleHalfHeight(20.0f); // Smaller but **NOT 0** (zero height breaks physics)

        MovementComponent->MaxWalkSpeed = CrouchSpeed / 2;

        UE_LOG(Player, Log, TEXT("ToggleProne: Entered prone mode"));
        /*
        if (ProneAnimMontage)
        {
            PlayAnimMontage(ProneAnimMontage, 1.0f);
        }
        */
    }
}


// Sliding Function
void AMyCharacter::StartSlide()
{
    if (bIsSprinting && !bIsSliding && GetCharacterMovement())
    {
        bIsSliding = true;
        bCanSlideJump = true;

        if (UCapsuleComponent* Capsule = GetCapsuleComponent())
        {
            Capsule->SetCapsuleHalfHeight(22.0f);
        }

        GetCharacterMovement()->MaxWalkSpeed = SlideSpeed * MovementDebuffMultiplier;

        FVector SlideDirection = GetLastMovementInputVector();
        if (SlideDirection.IsZero())
        {
            SlideDirection = GetActorForwardVector();
        }
        SlideDirection.Normalize();

        LaunchCharacter(SlideDirection * SlideSpeed, true, true);
        /*
        if (SlideMontage)
        {
            PlayAnimMontage(SlideMontage);
        }
        */
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
        ApplyMovementSpeed(); // Restore walk speed

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
        /*
        // Play roll animation
        if (RollAnimMontage)
        {
            PlayAnimMontage(RollAnimMontage, 1.0f);
        }
        */

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
    FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
    FVector Forward = GetActorForwardVector();
    FVector WallCheckEnd = Start + Forward * 100.0f;

    FHitResult WallHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(WallHit, Start, WallCheckEnd, ECC_Visibility, Params))
    {
        FVector LedgeStart = WallHit.ImpactPoint + FVector(0, 0, 100.0f);
        FVector LedgeEnd = LedgeStart - FVector(0, 0, 150.0f);

        FHitResult LedgeHit;
        if (GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeStart, LedgeEnd, ECC_Visibility, Params))
        {
            FVector MantleTarget = LedgeHit.ImpactPoint + FVector(0, 0, 50.0f);
            FVector LaunchVelocity = (MantleTarget - GetActorLocation()) * 2.0f;

            LaunchCharacter(LaunchVelocity, true, true);
            /*
            if (MantleAnimMontage)
            {
                PlayAnimMontage(MantleAnimMontage);
            }
            */
            UE_LOG(Player, Log, TEXT("Mantle successful: %s"), *MantleTarget.ToString());
            return;
        }
    }

    UE_LOG(Player, Warning, TEXT("Mantle failed: No ledge found"));
}




// Function determines whether a mantle action is currently possible by checking for ledge presence.
bool AMyCharacter::CanMantle() const
{
    FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
    FVector Forward = GetActorForwardVector();
    FVector WallCheckEnd = Start + Forward * 100.0f;

    FHitResult WallHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Check for wall
    if (GetWorld()->LineTraceSingleByChannel(WallHit, Start, WallCheckEnd, ECC_Visibility, Params))
    {
        FVector LedgeStart = WallHit.ImpactPoint + FVector(0, 0, 100.0f); // Check above wall
        FVector LedgeEnd = LedgeStart - FVector(0, 0, 150.0f);            // Trace down to ledge

        FHitResult LedgeHit;
        if (GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeStart, LedgeEnd, ECC_Visibility, Params))
        {
            return true;
        }
    }

    return false;
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
        /*
        // play a ledge grab animation
        if (LedgeGrabAnimMontage)
        {
            PlayAnimMontage(LedgeGrabAnimMontage);
        }
        */
    }
}


// Function completes the ledge grab and moves the character onto the ledge.
void AMyCharacter::PullUpFromLedge()
{
    SetActorLocation(LatchedLedgeLocation);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    bIsLatchedToLedge = false;
    /*
    if (PullUpAnimMontage)
        PlayAnimMontage(PullUpAnimMontage);
        */
    UE_LOG(Player, Log, TEXT("Climbed to ledge: %s"), *LatchedLedgeLocation.ToString());
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
    bHasAppliedSpeedDebuff = false;
    MovementDebuffMultiplier = 1.0f;
    UE_LOG(LogTemp, Log, TEXT("Movement debuffs reset."));
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Camera) // <- important null check!
    {
        float EffectiveZoomSpeed = ZoomInterpSpeed;
        if (CurrentWeapon)
        {
            EffectiveZoomSpeed = CurrentWeapon->ZoomInterpSpeed;
        }
        Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, EffectiveZoomSpeed));
    }

    if (bIsLatchedToLedge || !GetCharacterMovement() || !GetCharacterMovement()->IsFalling()) return;

    FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
    FVector Forward = GetActorForwardVector();
    FVector WallEnd = Start + Forward * 100.0f;

    FHitResult WallHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(WallHit, Start, WallEnd, ECC_Visibility, Params))
    {
        FVector LedgeStart = WallHit.ImpactPoint + FVector(0, 0, 100.0f);
        FVector LedgeEnd = LedgeStart - FVector(0, 0, 150.0f);

        FHitResult LedgeHit;
        if (GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeStart, LedgeEnd, ECC_Visibility, Params))
        {
            FVector Normal = LedgeHit.ImpactNormal;
            float SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Normal, FVector::UpVector)));

            if (SlopeAngle < 45.0f)
            {
                bIsLatchedToLedge = true;
                LatchedLedgeLocation = LedgeHit.ImpactPoint + FVector(0, 0, 90.0f);

                if (GetCharacterMovement())
                {
                    GetCharacterMovement()->StopMovementImmediately();
                    GetCharacterMovement()->DisableMovement();
                }
                /*
                if (LedgeGrabAnimMontage)
                {
                    PlayAnimMontage(LedgeGrabAnimMontage);
                }
                */
                UE_LOG(Player, Log, TEXT("Latched to ledge at %s"), *LatchedLedgeLocation.ToString());
            }
        }
    }
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
    
    const FString BoneNameStr = bone_name.ToString();

    if (!bHasAppliedSpeedDebuff &&
        (BoneNameStr.Contains("Leg") || BoneNameStr.Contains("Foot") || BoneNameStr.Contains("UpLeg")))
    {
        MovementDebuffMultiplier = modifier->get_speed_multiplier(); // Save the slow multiplier
        ApplyMovementSpeed();
        bHasAppliedSpeedDebuff = true;

        UE_LOG(Player, Log, TEXT("Speed debuff applied once due to bone %s. New debuff multiplier = %f"), *BoneNameStr, MovementDebuffMultiplier);
    }

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

void AMyCharacter::ApplyMovementSpeed()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();

    if (!Movement) return;

    if (bIsSprinting)
    {
        Movement->MaxWalkSpeed = SprintSpeed * MovementDebuffMultiplier;
    }
    else if (bIsCrouched)
    {
        Movement->MaxWalkSpeed = CrouchSpeed * MovementDebuffMultiplier;
    }
    else if (bIsSliding)
    {
        Movement->MaxWalkSpeed = SlideSpeed * MovementDebuffMultiplier;
    }
    else if (bIsProne)
    {
        Movement->MaxWalkSpeed = (CrouchSpeed / 2) * MovementDebuffMultiplier;
    }
    else
    {
        Movement->MaxWalkSpeed = WalkSpeed * MovementDebuffMultiplier;
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