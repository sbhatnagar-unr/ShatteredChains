#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ShatteredChains/Logging.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY(Player);

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

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
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
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
    PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &AMyCharacter::ToggleCrouch);

    // Bind action mappings for slide and roll
    PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMyCharacter::StartSlide);
    PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMyCharacter::StartRoll);

    // Prone
    PlayerInputComponent->BindAction("ToggleProne", IE_Pressed, this, &AMyCharacter::ToggleProne);

    // Slide Jump
    PlayerInputComponent->BindAction("SlideJump", IE_Pressed, this, &AMyCharacter::SlideJump);

}

void AMyCharacter::Move(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (IsValid(Controller)) 
    {
        // Get Forward Direction
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add Movement Input
        AddMovementInput(ForwardDirection, InputVector.Y);
        AddMovementInput(RightDirection, InputVector.X);
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

void AMyCharacter::Jump()
{
    ACharacter::Jump();
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

// Jump Functions with double Jump Logic
void AMyCharacter::StartJump()
{
    if (JumpCount < MaxJumpCount)
    {
        bPressedJump = true;
        JumpCount++;
    }
}

// Stop Jump Function
void AMyCharacter::StopJump()
{
    bPressedJump = false;
}

// Function to register landing
void AMyCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    JumpCount = 0; // Reset jump count on landing
}

// Start Sprint Function with Stamina Management
void AMyCharacter::StartSprint()
{
    if (CurrentStamina > 0.0f && !bIsSprinting)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

// Stop Sprint Function
void AMyCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Stamina Caculation Function
void AMyCharacter::UpdateStamina(float DeltaTime)
{
    if (bIsSprinting && CurrentStamina > 0.0f)
    {
        CurrentStamina -= StaminaDrainRate * DeltaTime;
        if (CurrentStamina <= 0.0f)
        {
            StopSprint();
        }
    }
    else if (CurrentStamina < Stamina)
    {
        CurrentStamina += StaminaRecoveryRate * DeltaTime;
    }
}

// Crouch Toggle Function
void AMyCharacter::ToggleCrouch()
{
    if (bIsCrouched)
    {
        UnCrouch();
        bIsCrouched = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
    else
    {
        Crouch();
        bIsCrouched = true;
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
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

        // Optionally, play prone exit animation
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
        GetCapsuleComponent()->SetCapsuleHalfHeight(44.0f); // Reduced height for prone
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed / 2; // Half crouch speed for prone

        // Optionally, play prone enter animation
        if (ProneAnimMontage)
        {
            PlayAnimMontage(ProneAnimMontage, 1.0f);
        }
    }
}

// Sliding Function
void AMyCharacter::StartSlide()
{
    if (bIsSprinting && !bIsSliding)
    {
        bIsSliding = true;
        GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
        if (SlideMontage)
        {
            PlayAnimMontage(SlideMontage, 1.0f);
        }
        GetWorld()->GetTimerManager().SetTimer(SlideStopTimer, this, &AMyCharacter::StopSlide, 1.0f, false);
    }
}

// Stop sliding function
void AMyCharacter::StopSlide()
{
    bIsSliding = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetWorld()->GetTimerManager().ClearTimer(SlideStopTimer);
}

// Slide Jump Function
void AMyCharacter::SlideJump()
{
    if (bIsSliding)
    {
        // Launch the character upwards and forwards
        FVector LaunchVelocity = GetActorForwardVector() * 800.0f + FVector(0, 0, 600.0f); // Adjust values for forward and upward force
        LaunchCharacter(LaunchVelocity, true, true);

        // End sliding state
        StopSlide();

        // Optionally, play slide jump animation
        if (SlideJumpAnimMontage)
        {
            PlayAnimMontage(SlideJumpAnimMontage, 1.0f);
        }
    }
}


// Rolling Function
void AMyCharacter::StartRoll()
{
    if (bCanRoll && RollAnimMontage)
    {
        bCanRoll = false;
        PlayAnimMontage(RollAnimMontage, 1.0f);

        // Temporarily disable character movement during roll
        GetCharacterMovement()->DisableMovement();

        // Schedule StopRoll to be called after the montage finishes
        const float RollDuration = RollAnimMontage->GetPlayLength();
        GetWorld()->GetTimerManager().SetTimer(RollCooldownTimer, this, &AMyCharacter::StopRoll, RollDuration, false);
    }
}

// Mostly to stop animation roll
void AMyCharacter::StopRoll()
{
    // Re-enable character movement
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    // Allow rolling again after cooldown
    GetWorld()->GetTimerManager().SetTimer(RollCooldownTimer, this, &AMyCharacter::EnableRolling, 1.0f, false);
}

// Spam Prevention on rolling, needs testing.
void AMyCharacter::EnableRolling()
{
    bCanRoll = true;
}

// Mantle Function for Climbing Ledges
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

            // Optionally, play a mantling animation
            if (MantleAnimMontage)
            {
                PlayAnimMontage(MantleAnimMontage);
            }
        }
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

        // Optionally, play a ledge grab animation
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

    // Optionally, play a pull-up animation
    if (PullUpAnimMontage)
    {
        PlayAnimMontage(PullUpAnimMontage);
    }
}


// Function identifies whether there’s a ledge above the character that can be climbed.
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
    }
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyCharacter::dead(AActor* killed_by)
{
    UE_LOG(Player, Log, LOG_TEXT("Player dead"));
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

