#include "MyCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"              
#include "Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ShatteredChains/Logging.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

    actor_name = "Player";


    leg_shot_damage_adder = 0;
    leg_shot_damage_multiplier = 1;
    leg_shot_speed_adder = 0;
    leg_shot_speed_multiplier = 1;
    foot_shot_damage_adder = 0;
    foot_shot_damage_multiplier = 1;
    foot_shot_speed_adder = 0;
    foot_shot_speed_multiplier = 1;
    arm_shot_damage_adder = 0;
    arm_shot_damage_multiplier = 1;
    arm_shot_speed_adder = 0;
    arm_shot_speed_multiplier = 1;
    hand_shot_damage_adder = 0;
    hand_shot_damage_multiplier = 1;
    hand_shot_speed_adder = 0;
    hand_shot_speed_multiplier = 1;
    torso_shot_damage_adder = 0;
    torso_shot_damage_multiplier = 1;
    torso_shot_speed_adder = 0;
    torso_shot_speed_multiplier = 1;
    head_shot_damage_adder = 0;
    head_shot_damage_multiplier = 1;
    head_shot_speed_adder = 0;
    head_shot_speed_multiplier = 1;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

    add_stats_modifiers(GetMesh()->GetPhysicsAsset(), &stats_modifiers);

    stats_modifiers["Hips"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["Hips"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["Hips"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["Hips"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["LeftUpLeg"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["LeftUpLeg"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["LeftUpLeg"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["LeftUpLeg"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);

    stats_modifiers["LeftLeg"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["LeftLeg"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["LeftLeg"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["LeftLeg"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["LeftFoot"]->set_additive_damage_modifier(foot_shot_damage_adder);
    stats_modifiers["LeftFoot"]->set_multiplicative_damage_modifier(foot_shot_damage_multiplier);
    stats_modifiers["LeftFoot"]->set_additive_speed_modifier(foot_shot_speed_adder);
    stats_modifiers["LeftFoot"]->set_multiplicative_speed_modifier(foot_shot_speed_multiplier);

    stats_modifiers["RightUpLeg"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["RightUpLeg"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["RightUpLeg"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["RightUpLeg"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);

    stats_modifiers["RightLeg"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["RightLeg"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["RightLeg"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["RightLeg"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["RightFoot"]->set_additive_damage_modifier(foot_shot_damage_adder);
    stats_modifiers["RightFoot"]->set_multiplicative_damage_modifier(foot_shot_damage_multiplier);
    stats_modifiers["RightFoot"]->set_additive_speed_modifier(foot_shot_speed_adder);
    stats_modifiers["RightFoot"]->set_multiplicative_speed_modifier(foot_shot_speed_multiplier);

    stats_modifiers["Spine1"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["Spine1"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["Spine1"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["Spine1"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);
    
    stats_modifiers["Spine2"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["Spine2"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["Spine2"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["Spine2"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["LeftArm"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["LeftArm"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["LeftArm"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["LeftArm"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);
    
    stats_modifiers["LeftHand"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["LeftHand"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHand"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["LeftHand"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);

    stats_modifiers["Head"]->set_additive_damage_modifier(head_shot_damage_adder);
    stats_modifiers["Head"]->set_multiplicative_damage_modifier(head_shot_damage_multiplier);
    stats_modifiers["Head"]->set_additive_speed_modifier(head_shot_speed_adder);
    stats_modifiers["Head"]->set_multiplicative_speed_modifier(head_shot_speed_multiplier);

    stats_modifiers["RightArm"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["RightArm"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["RightArm"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["RightArm"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);

    stats_modifiers["RightHand"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["RightHand"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["RightHand"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["RightHand"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);

    // Check audio
    if (take_damage_sound == nullptr)
    {
        UE_LOG(Player, Warning, LOG_TEXT("No take damage sound effect for player %s"), *actor_name);
    }
    if (jump_sound == nullptr)
    {
        UE_LOG(Player, Warning, LOG_TEXT("No jump sound effect for player %s"), *actor_name);
    }
}

// Player Input bindings
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Add input mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        // Get Local Player Subsystem
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            // Add input context
            Subsystem->AddMappingContext(InputMapping, 0);
    }

    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Fire Weapon
        Input->BindAction(FireAction, ETriggerEvent::Started, this, &AMyCharacter::FireWeapon);
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

        // Bind roll action
        Input->BindAction(RollAction, ETriggerEvent::Started, this, &AMyCharacter::StartRoll);
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

void AMyCharacter::FireWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->fire(); // Calls the fire function in Weapon.cpp
        UE_LOG(Player, Log, TEXT("Fired weapon: %s"), *CurrentWeapon->GetName());
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("No weapon equipped to fire."));
    }
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

void AMyCharacter::PickUpWeapon(AWeapon* weapon)
{
    EquipWeapon(weapon);
}


// Interact Function
void AMyCharacter::Interact()
{
    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + (Camera->GetForwardVector() * 1500.0f);
    FHitResult HitResult;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Perform the line trace
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(Player, Log, TEXT("Line Trace Hit Actor: %s"), *HitActor->GetName());

            // Check if the hit actor is a weapon
            if (AWeapon* weapon = Cast<AWeapon>(HitActor))
            {
                UE_LOG(Player, Log, TEXT("Weapon Detected: %s"), *weapon->GetName());

                EquipWeapon(weapon); // Attempt to equip the weapon
                weapon->SetActorHiddenInGame(true); // Hide the weapon
                weapon->SetActorEnableCollision(false);
            }
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
        // Convert 2D input to a 3D direction vector
        FVector ForwardDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

        FVector CurrentInputDirection = ForwardDirection * MovementInput.Y + RightDirection * MovementInput.X;

        // Normalize the direction
        CurrentInputDirection.Normalize();

        // Log only if the direction changes significantly
        if (!CurrentInputDirection.Equals(LastInputDirection, 0.01f)) 
        {
            UE_LOG(Player, VeryVerbose, TEXT("Moving in direction: %s"), *CurrentInputDirection.ToString());
            LastInputDirection = CurrentInputDirection;
        }

        // Apply the movement
        AddMovementInput(ForwardDirection, MovementInput.Y);
        AddMovementInput(RightDirection, MovementInput.X);
    }
    else
    {
        // Reset the last direction if no movement input is provided
        LastInputDirection = FVector::ZeroVector;
    }
}


void AMyCharacter::Look(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (IsValid(Controller))
    {
        AddControllerYawInput(InputVector.X);
        AddControllerPitchInput(InputVector.Y);
    }
}

// Jump Function
void AMyCharacter::Jump()
{
    ACharacter::Jump();
    UE_LOG(Player, Log, TEXT("Jump triggered"));
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
        UE_LOG(Player, Log, TEXT("StartJump triggered: JumpCount = %d"), JumpCount);
    }
    else
    {
        UE_LOG(Player, Warning, TEXT("StartJump failed: MaxJumpCount reached"));
    }
}

// Stop Jump Function
void AMyCharacter::StopJump()
{
    bPressedJump = false;
    UE_LOG(Player, Log, TEXT("StopJump triggered"));
}

// Function to register landing
void AMyCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    JumpCount = 0; // Reset jump count on landing
    UE_LOG(Player, Log, TEXT("Landed: JumpCount reset to %d"), JumpCount);
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
    // Get the capsule component for modifying its height
    UCapsuleComponent* Capsule = GetCapsuleComponent();

    if (bIsCrouched)
    {
        // Restore original height and speed when uncrouching
        Capsule->SetCapsuleHalfHeight(88.0f); // Default height
        UnCrouch();
        bIsCrouched = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        UE_LOG(Player, Log, TEXT("UnCrouch: Height restored to %f"), Capsule->GetUnscaledCapsuleHalfHeight());
    }
    else
    {
        // Halve the height and reduce movement speed when crouching
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
        GetCapsuleComponent()->SetCapsuleHalfHeight(22.0f); // Reduced height for prone
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


// Rolling Function
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
        LaunchCharacter(RollDirection * 600.0f + FVector(0, 0, 200.0f), true, true); // Adjust speed and height

        // Play roll animation
        if (RollAnimMontage)
        {
            PlayAnimMontage(RollAnimMontage, 1.0f);
        }

        // Set a cooldown timer for rolling
        const float RollCooldown = 1.0f; // Adjust cooldown time
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
    GetWorld()->GetTimerManager().SetTimer(RollCooldownTimer, this, &AMyCharacter::EnableRolling, 1.0f, false);
}

// Spam Prevention on rolling, needs testing.
void AMyCharacter::EnableRolling()
{
    bCanRoll = true;
    UE_LOG(Player, Log, TEXT("Rolling re-enabled"));
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


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyCharacter::on_death(AActor* killed_by)
{
    UE_LOG(Player, Log, LOG_TEXT("Player dead.... restarting level"));

    const UWorld* world = GetWorld();

    if (world == nullptr)
    {
        UE_LOG(Player, Error, LOG_TEXT("Could not get world to restart level"));
        return;
    }

    const ULevel* current_level = world->GetCurrentLevel();
    const FName current_level_name = *current_level->GetPackage()->GetName();
    UGameplayStatics::OpenLevel(this, current_level_name);
    UE_LOG(Player, Log, LOG_TEXT("Level restarted"));
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



void AMyCharacter::hit_bone(const FName bone_name)
{
    const TObjectPtr<UStatsModifier> modifier = stats_modifiers[bone_name];
    
    UE_LOG(BoneCollision, Log, LOG_TEXT("Player '%s' hit in bone '%s'"), *actor_name, *(bone_name.ToString()));

    // Apply other modifier stats, like speed, accuracy, etc.
    UCharacterMovementComponent* movement_component = GetCharacterMovement();
    const float old_movement_speed = movement_component->MaxWalkSpeed;
    
    movement_component->MaxWalkSpeed += modifier->get_additive_speed_modifier();
    movement_component->MaxWalkSpeed *= modifier->get_multiplicative_speed_modifier();

    UE_LOG(BoneCollision, Log, LOG_TEXT("Changing player '%s' speed: %f -> %f"), *actor_name, old_movement_speed, movement_component->MaxWalkSpeed);
}


USoundBase* AMyCharacter::get_damage_sound() const
{
    return take_damage_sound;    
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