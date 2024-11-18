// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

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

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind axis mappings for movement
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

    // Bind action mappings for jumping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);

    // Bind action mappings for sprinting and crouching
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
    PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &AMyCharacter::ToggleCrouch);

    // Bind action mappings for slide and roll
    PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMyCharacter::StartSlide);
    PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMyCharacter::StartRoll);
}





void AMyCharacter::UpdateStamina(float DeltaTime)
{
}

bool AMyCharacter::CanMantle() const
{
	return false;
}

bool AMyCharacter::IsNearWall() const
{
	return false;
}

void AMyCharacter::ToggleProne()
{
}

void AMyCharacter::StartLedgeGrab()
{
}

void AMyCharacter::PullUpFromLedge()
{
}

bool AMyCharacter::DetectLedge()
{
	return false;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::Landed(const FHitResult& Hit)
{
}

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

void AMyCharacter::StartJump()
{
}

void AMyCharacter::StopJump()
{
}

void AMyCharacter::StartSprint()
{
}

void AMyCharacter::StopSprint()
{
}

void AMyCharacter::ToggleCrouch()
{
}

void AMyCharacter::StartSlide()
{
}

void AMyCharacter::StopSlide()
{
}

void AMyCharacter::SlideJump()
{
}

void AMyCharacter::StartRoll()
{
}

void AMyCharacter::StopRoll()
{
}

void AMyCharacter::EnableRolling()
{
}

void AMyCharacter::Mantle()
{
}

void AMyCharacter::WallJump()
{
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
