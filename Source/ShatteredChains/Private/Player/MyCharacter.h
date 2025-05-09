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
#include "Components/TimelineComponent.h"
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

    virtual void on_death(const AActor* killed_by, const bool play_death_sound=true) override final;

    virtual UHealthComponent* get_health_component() const override final;
    virtual TObjectPtr<UInventoryComponent> get_inventory_component() const override final;

    void EquipWeapon(AWeapon* Weapon);
    bool PickUpWeapon(AWeapon* Weapon);


    // This should return a location to start a hitscan from
    virtual FVector get_hitscan_start_location() const override final;

    // This should return a Vector in the direction to fire
    virtual FVector get_hitscan_direction() const override final;

    virtual FString get_default_actor_name() const override final;

    // IHasBoneCollider
    virtual void hit_bone(const AActor* hit_by, const FName bone_name, float weapon_damage) override final;
    virtual const TMap<FName, TObjectPtr<UStatsModifier>>* get_bone_collider_stats_modifiers() const override final;

    
    //movement reset for medkit
    void ResetMovementDebuffs();

    bool bHasAppliedSpeedDebuff = false;

    float MovementDebuffMultiplier = 1.0f;

    void ApplyMovementSpeed();

protected:

    bool bIsMedkitEquipped = false;

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
    
    int32 CurrentEquippedWeaponSlot = -1;

    //medkit handling
    UPROPERTY()
    class AMedKit* NearbyMedKit = nullptr;

    UPROPERTY()
    AMedKit* EquippedMedKit = nullptr;


    void Move(const FInputActionValue& InputValue);
    void Look(const FInputActionValue& InputValue);
    void Jump();

    bool bHasJumpedOnce = false;

    bool bIsLatchedToLedge = false; // are we hanging on a ledge
    FVector LatchedLedgeLocation;   // target ledge top to pull up to

    void CancelLedgeLatch();

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

    UFUNCTION()
    void UseEquippedMedkit();

    // Offset when aiming (relative to camera)
    UPROPERTY(EditAnywhere, Category = "Zoom")
    FVector ZoomedWeaponOffset = FVector(50.f, 20.f, -24.f);

    // Rotation to rotate the weapon when zooming
    UPROPERTY(EditAnywhere, Category = "Zoom")
    FRotator ZoomedWeaponRotation = FRotator(0.0f, -95.0f, -2.0f); // yaw, pitch, roll

    UPROPERTY(EditAnywhere, Category = "Zoom")
    float ZoomedFOV = 60.0f;

    UPROPERTY()
    AWeapon* CurrentWeapon = nullptr;

    void FireWeapon();
    void EndFireWeapon();

    void ReloadWeapon();

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<class AMeleeWeapon> FistWeaponClass;

    UPROPERTY()
    AMeleeWeapon* FistWeapon;

    UPROPERTY()
    AMeleeWeapon* EquippedMeleeWeapon = nullptr;

    void QuickMelee();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput")
    UInputAction* IA_WeaponSlot5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput")
    UInputAction* IA_QuickMelee;

    /*--------------------- State Variables ---------------------*/

    // Tracks if character is currently crouched
    bool bIsCrouched = false;

    // Tracks if character is sprinting
    bool bIsSprinting = false;

    // Determines if the character is eligible to roll
    bool bCanRoll = true;

    // Tracks whether the player has a medkit equipped (slot 4)
    UPROPERTY()
    bool bIsHoldingMedKit = false;

    // Zoom
    UPROPERTY(EditAnywhere)
    float DefaultFOV = 90.0f;

    float TargetFOV;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ZoomInterpSpeed = 15.0f;

    bool bIsZooming = false;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* ScopeAction;

    UFUNCTION()
    void StartZoom();

    UFUNCTION()
    void StopZoom();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scope")
    float ZoomMouseSensitivity = 0.5f; // reduced mouse speed while scoped


    // dodging
    bool bIsDodging = false;


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
    float SlideSpeed = 1000.0f;

    // Slide duration
    UPROPERTY(EditAnywhere, Category = "Slide")
    float SlideDuration = 1.0f;

    // Timer to stop sliding
    FTimerHandle SlideStopTimer;

    // Can Slide Jump
    bool bCanSlideJump = false;

    // Slide jump force
    UPROPERTY(EditAnywhere, Category = "Slide Jump")
    FVector SlideJumpForce = FVector(0.0f, 0.0f, 500.0f);

    /*--------------------- Movement Speeds ---------------------*/

    // Base movement speed when walking
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 450.0f;

    // Sprinting speed 
    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeed = 750.0f;

    // Speed while crouched
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CrouchSpeed = 200.0f;

    // mouse speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MouseSensitivity = 1.0f;


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
    /*
    // Animation montage for rolling action 
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RollAnimMontage;

    // Animation montage for sliding action
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* SlideMontage;

    // Crouch Animations
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* CrouchEnterMontage; // Animation for entering crouch

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* CrouchIdleMontage;  // Animation for crouch idle

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* CrouchWalkMontage;  // Animation for crouch walking

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* StandUpMontage;     // Animation for standing up

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool bIsCrouching;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool bIsInAir;
    */
    /*--------------------- Weapon Handling ---------------------*/

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_WeaponSlot1;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_WeaponSlot2;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_WeaponSlot3;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_DropWeapon;

    /*--------------------- Sound Effects ---------------------*/

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* item_pickup_sound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* medkit_heal_sound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* scope_in_sound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* weapon_equip_sound;


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

    // inventory is open
    bool bIsInventoryOpen = false;

    // toggle inventory input action
    UFUNCTION()
    void ToggleInventory(const FInputActionValue& Value);

    //inventory log
    UFUNCTION()
    void LogInventory(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    UInventoryComponent* GetInventoryComponent() const;
    /*
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
    
   //       currently leave commented until fixaround.
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RollAnimMontage;
   
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* JumpAnimMontage;
    */


    UPROPERTY(VisibleAnywhere)
    UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere)
    UInventoryComponent* InventoryComponent;

    // inventory toggle
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_ToggleInventory;

    // log inventory 
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_LogInventory;    

    // Sound effects
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> jump_sound;

    //declare medkit input
    UFUNCTION()
    void ToggleMedKit(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    UInputAction* IA_UseHealthKit; // Bound to "4"
    
    // Bone Collision
    // Stats modifiers map
    UPROPERTY()
    TMap<FName, TObjectPtr<UStatsModifier>> stats_modifiers;

    // Adders/Multipliers
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a leg shot is hit"))
    float leg_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a leg shot is hit"))
    float leg_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a leg shot is hit"))
    float leg_shot_accuracy_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a foot shot is hit"))
    float foot_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a foot shot is hit"))
    float foot_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a foot shot is hit"))
    float foot_shot_accuracy_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a arm shot is hit"))
    float arm_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a arm shot is hit"))
    float arm_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a arm shot is hit"))
    float arm_shot_accuracy_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a hand shot is hit"))
    float hand_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a hand shot is hit"))
    float hand_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a hand shot is hit"))
    float hand_shot_accuracy_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a torso shot is hit"))
    float torso_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a torso shot is hit"))
    float torso_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a torso shot is hit"))
    float torso_shot_accuracy_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a head shot is hit"))
    float head_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a head shot is hit"))
    float head_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for accuracy whenever a head shot is hit"))
    float head_shot_accuracy_multiplier;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> death_sounds;
    
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> leg_shot_sounds;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> foot_shot_sounds;
    
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> arm_shot_sounds;
    
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> hand_shot_sounds;
    
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> torso_shot_sounds;
    
    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TArray<TObjectPtr<USoundBase>> head_shot_sounds;

    TMap<FName, TArray<TObjectPtr<USoundBase>>> sound_map;


private:
    void restart_current_level() const;

    FVector player_death_camera_start_location;
    FVector player_death_camera_end_location;
    FRotator player_death_camera_start_rotation;
    FRotator player_death_camera_end_rotation;
    // Function to update the camera position
    UFUNCTION()
    void UpdateCameraPosition(float value);

    // Timeline component to handle interpolation
    TObjectPtr<UTimelineComponent> camera_timeline;

    // Curve to define the interpolation
    UPROPERTY(EditDefaultsOnly, Category="Camera")
    UCurveFloat* camera_curve;
    
    
    void HandleWeaponSlotInput(int32 Slot);
    void DropWeapon();


    bool has_fired_weapon = false;


};