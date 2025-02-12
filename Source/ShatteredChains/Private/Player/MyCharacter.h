#pragma once
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent/HealthComponent.h"
#include "Components/InventoryComponent/InventoryComponent.h"
#include "Interfaces/HasBoneCollider/HasBoneCollider.h"
#include "Interfaces/HasBoneCollider/StatsModifier/StatsModifier.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Interfaces/HasInventory/HasInventory.h"
#include "Interfaces/WeaponUser/WeaponUser.h"
#include "EnhancedInputComponent.h"  // Include enhanced input system
#include "Interfaces/NamedActor/NamedActor.h"
#include "MyCharacter.generated.h"   // Must be the last include

/**
 * AMyCharacter
 *
 * don't delete this, breaks the code for some reason at generated_body()
 */

 // Forward declaration for AWeapon
class AWeapon;

UCLASS()
class SHATTEREDCHAINS_API AMyCharacter : public ACharacter, public IHasHealth, public IWeaponUser, public IHasInventory, public INamedActor, public IHasBoneCollider
{
    GENERATED_BODY()

    // Camera
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* Camera;


public:
    // Constructor
    AMyCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void on_death(AActor* killed_by) override final;

    virtual UHealthComponent* get_health_component() const override final;
    virtual TObjectPtr<UInventoryComponent> get_inventory_component() const override final;

    void EquipWeapon(AWeapon* Weapon);
    void PickUpWeapon(AWeapon* Weapon);

    // This should return a location to start a hitscan from
    virtual FVector get_hitscan_start_location() const override final;

    // This should return a Vector in the direction to fire
    virtual FVector get_hitscan_direction() const override final;

    virtual FString get_default_actor_name() const override final;

    // IHasBoneCollider
    virtual void hit_bone(const FName bone_name) override final;
    virtual const TMap<FName, TObjectPtr<UStatsModifier>>* get_bone_collider_stats_modifiers() const override final;


    virtual USoundBase* get_damage_sound() const override final;

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

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* CrouchAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* SlideAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* RollAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* ProneAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput")
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* FireAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* ReloadAction;



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

    // Ends jump action
    void StopJump();

    // Starts sprinting
    void StartSprint();

    // Stops sprinting
    void StopSprint();

    // Toggle crouch state
    void ToggleCrouch();

    // Begins slide motion
    void StartSlide();

    // Ends sliding motion
    void StopSlide();

    // Executes a jump from sliding
    void SlideJump();

    // Initiates a roll sequence
    void StartRoll();

    // Stops roll sequence
    void StopRoll();

    // Enables rolling after a cooldown
    void EnableRolling();

    // Allows the Character to mantle/climb over obstacles
    void Mantle();

    // Allows the character to perform a wall jump
    void WallJump();

    UFUNCTION()
    void Interact();


    UPROPERTY()
    AWeapon* CurrentWeapon = nullptr;

    void FireWeapon();

    void ReloadWeapon();

    /*--------------------- State Variables ---------------------*/

    // Tracks if character is currently crouched
    bool bIsCrouched = false;

    // Tracks if character is sprinting
    bool bIsSprinting = false;

    // Determines if the character is eligible to roll
    bool bCanRoll = true;


    /*--------------------- Input Actions ---------------------*/

    // Input actions for sprinting, can be editable in editor
    /*
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SprintAction;
    */

    /*------------------ Slide Variables ------------------*/
    
    // Can slide
    bool bIsSliding = false;

    // Slide speed
    UPROPERTY(EditAnywhere, Category = "Slide")
    float SlideSpeed = 1500.0f;

    // Slide duration
    UPROPERTY(EditAnywhere, Category = "Slide")
    float SlideDuration = 0.75f;

    // Timer to stop sliding
    FTimerHandle SlideStopTimer;

    // Can Slide Jump
    bool bCanSlideJump = false;

    // Slide jump force
    UPROPERTY(EditAnywhere, Category = "Slide Jump")
    FVector SlideJumpForce = FVector(0.0f, 0.0f, 400.0f);

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

    // Timer for managing roll cooldown 
    FTimerHandle RollCooldownTimer;

    // Stores the last input direction
    FVector LastInputDirection = FVector::ZeroVector;

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

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere)
    UInventoryComponent* InventoryComponent;



    // Sound effects
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> jump_sound;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> take_damage_sound;
    
    // Bone Collision
    // Stats modifiers map
    UPROPERTY()
    TMap<FName, TObjectPtr<UStatsModifier>> stats_modifiers;

    // Adders/Multipliers
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a leg shot is hit (+ increases and - decreases)"))
    float leg_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a leg shot is hit"))
    float leg_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a leg shot is hit (+ increases and - decreases)"))
    float leg_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a leg shot is hit"))
    float leg_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a foot shot is hit (+ increases and - decreases)"))
    float foot_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a foot shot is hit"))
    float foot_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a foot shot is hit (+ increases and - decreases)"))
    float foot_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a foot shot is hit"))
    float foot_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a arm shot is hit (+ increases and - decreases)"))
    float arm_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a arm shot is hit"))
    float arm_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a arm shot is hit (+ increases and - decreases)"))
    float arm_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a arm shot is hit"))
    float arm_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a hand shot is hit (+ increases and - decreases)"))
    float hand_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a hand shot is hit"))
    float hand_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a hand shot is hit (+ increases and - decreases)"))
    float hand_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a hand shot is hit"))
    float hand_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a torso shot is hit (+ increases and - decreases)"))
    float torso_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a torso shot is hit"))
    float torso_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a torso shot is hit (+ increases and - decreases)"))
    float torso_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a torso shot is hit"))
    float torso_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a head shot is hit (+ increases and - decreases)"))
    float head_shot_damage_adder;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a head shot is hit"))
    float head_shot_damage_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a head shot is hit (+ increases and - decreases)"))
    float head_shot_speed_adder;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a head shot is hit"))
    float head_shot_speed_multiplier;

};