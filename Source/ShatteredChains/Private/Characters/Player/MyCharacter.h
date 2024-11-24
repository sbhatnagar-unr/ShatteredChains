#pragma once

#include "CoreMinimal.h"
#include "../CharacterWithHealth.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"  // Include enhanced input system
#include "MyCharacter.generated.h"   // Must be the last include

/**
 * AMyCharacter
 *
 * don't delete this, breaks the code for some reason at generated_body()
 */
UCLASS()
class SHATTEREDCHAINS_API AMyCharacter : public ACharacterWithHealth
{
    GENERATED_BODY()


public:
    // Constructor
    AMyCharacter();

    // Camera
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* Camera;

    // Called every frame
    virtual void Tick(float DeltaTime) override;


    void deal_damage(AActor* dealt_by, float damage) override final;

protected:

    // Input mapping for enhanced input
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* InputMapping;

    // References for inputs
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* LookAction;

    void Move(const FInputActionValue& InputValue);
    void Look(const FInputActionValue& InputValue);
    void Jump();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Detects landing to reset jump count
    virtual void Landed(const FHitResult& Hit) override;

    /*--------------------- Core Movement Functions ---------------------*/

    /**
    *
    *
    * Main Character Class implementing various movement capabilities such as sprinting, crouching,
    * mantling, etc.
    */

    // Moves the Character Forward or Backward based on input, -1 for backwards 1 for forwards
    void MoveForward(float Value);

    // Moves the Character Left or Right based on input, -1 for left 1 for right
    void MoveRight(float Value);

    // Initiates Jump sequence, supports double jump
    void StartJump();

    // Ends jump action, needed most likely to prevent infinite jumping
    void StopJump();

    // Starts sprinting, increasing speed and consuming stamina
    void StartSprint();

    // Stops sprinting, restores speed to normal
    void StopSprint();

    // Toggle crouch state, adjusting movement speed
    void ToggleCrouch();

    // Begins slide motion, triggered when sprinting
    void StartSlide();

    // Ends sliding motion, restoring speed and height
    void StopSlide();

    // Executes a jump from sliding, similar to slide-jumping from apex legends or other various games
    void SlideJump();

    // Initiates a roll sequence
    void StartRoll();

    // Stops roll sequence
    void StopRoll();

    // Enables rolling after a cooldown, needed to prevent infinite rolling
    void EnableRolling();

    // Allows the Character to mantle/climb over obstacles
    void Mantle();

    // Allows the character to perform a wall jump
    void WallJump();


    /*--------------------- State Variables ---------------------*/

    // Tracks if character is currently crouched
    bool bIsCrouched = false;

    // Tracks if character is sprinting
    bool bIsSprinting = false;

    // Tracks if character is in a sliding state
    bool bIsSliding = false;

    // Determines if the character is eligible to roll
    bool bCanRoll = true;


    /*--------------------- Input Actions ---------------------*/

    // Input actions for sprinting, can be editable in editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SprintAction;


    /*--------------------- Movement Speeds ---------------------*/

    // Base movement speed when walking
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 600.0f;

    // Sprinting speed
    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeed = 1200.0f;

    // Speed while crouched
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CrouchSpeed = 300.0f;

    // Speed during sliding
    UPROPERTY(EditAnywhere, Category = "Movement")
    float SlideSpeed = 1500.0f;


    /*--------------------- Jump Mechanics ---------------------*/

    // Counts current jumps for double jump functionality
    int32 JumpCount = 0;

    // Maximum number of jumps allowed (e.g., double jump)
    UPROPERTY(EditAnywhere, Category = "Jump")
    int32 MaxJumpCount = 2;


    /*--------------------- Stamina Properties ---------------------*/

    // Total stamina available for sprinting
    UPROPERTY(EditAnywhere, Category = "Stamina")
    float Stamina = 100.0f;

    // Current stamina level
    float CurrentStamina;

    // Rate at which stamina drains while sprinting
    UPROPERTY(EditAnywhere, Category = "Stamina")
    float StaminaDrainRate = 10.0f;

    // Rate at which stamina recovers when not sprinting
    UPROPERTY(EditAnywhere, Category = "Stamina")
    float StaminaRecoveryRate = 5.0f;


    /*--------------------- Animation Properties ---------------------*/

    // Animation montage for rolling action
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RollAnimMontage;

    // Animation montage for sliding action
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SlideMontage;


    /*--------------------- Timer Handles ---------------------*/

    // Timer handle for stopping the slide after a duration
    FTimerHandle SlideStopTimer;

    // Timer for managing roll cooldown
    FTimerHandle RollCooldownTimer;


    /*--------------------- Helper Functions ---------------------*/

    /*
     Updates stamina based on sprinting or resting state.
     Called within the tick function.
     */
    void UpdateStamina(float DeltaTime);

    // Checks for ledges above the character to allow mantling.
    bool CanMantle() const;

    // Determines if character can perform a wall jump based on nearby wall proximity.
    bool IsNearWall() const;


    // Additional variables and methods for prone and ledge grab
    //
    // Character prone state
    bool bIsProne;  // Tracks if the character is in a prone position

    // Function to toggle prone state
    void ToggleProne();

    // Additional Ledge Grab properties
    bool bIsHanging;  // Tracks if the character is currently grabbing a ledge

    // Function to initiate ledge grab mechanics
    void StartLedgeGrab();

    // Function to complete the ledge grab (pull-up)
    void PullUpFromLedge();

    // Function to detect ledge presence for grabbing
    bool DetectLedge();


    // Animation montages for each movement type
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* WalkAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SilentWalkAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SprintAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ProneAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* CrouchAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* MantleAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* LedgeGrabAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* PullUpAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SlideAnimMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* SlideJumpAnimMontage;

    /*          currently leave commented until fixaround.
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RollAnimMontage;
    */
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* JumpAnimMontage;


private:
    void dead(AActor* killed_by) override;

};
