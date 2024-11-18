// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::MoveForward(float Value)
{
}

void AMyCharacter::MoveRight(float Value)
{
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

