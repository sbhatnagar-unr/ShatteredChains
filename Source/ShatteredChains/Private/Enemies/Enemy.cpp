// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/CustomTraceChannels.h"

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    target = nullptr;
    health_component = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

    pawn_sensing_component = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
    
    leg_shot_damage_multiplier = 1;
    leg_shot_speed_multiplier = 1;
    leg_shot_accuracy_multiplier = 1;
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

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Set actor name
    actor_name = default_actor_name;
    
    // Get the target if one was not set in the editor
    if (target == nullptr)
    {
        UE_LOG(Enemy, Log, LOG_TEXT("%s had no target set in editor, using player as target"), *actor_name);
        target = GetWorld()->GetFirstPlayerController()->GetPawn();

        if (target == nullptr)
        {
            UE_LOG(Enemy, Error, LOG_TEXT("%s could not locate target (player)"), *actor_name);
            return;
        }
        UE_LOG(Enemy, Verbose, LOG_TEXT("Found target (player)"));
    }

    const TObjectPtr<USkeletalMeshComponent> mesh = GetMesh();
    if (mesh == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy '%s' has no skeletal mesh"), *actor_name);
        return;
    }

    const TObjectPtr<UPhysicsAsset> physics_asset = mesh->GetPhysicsAsset();
    if (physics_asset == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy '%s' has no physics assets"), *actor_name);
        return;
    }

    // Get all collision physics assets and create stats modifiers for them
    add_stats_modifiers(physics_asset, &stats_modifiers);

    // Check audio
    if (take_damage_sound == nullptr)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No take damage sound effect for enemy %s"), *actor_name);
    }

    if (death_sound == nullptr)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("No death sound effect for enemy %s"), *actor_name);
    }
}


AActor* AEnemy::get_target() const
{
    return target;
}


UHealthComponent* AEnemy::get_health_component() const
{
    return health_component;
}


void AEnemy::on_death(AActor* killed_by)
{
    const INamedActor* const killed_by_na = Cast<INamedActor>(killed_by);

    UGameplayStatics::PlaySound2D(GetWorld(), death_sound, 1, 1, 0, nullptr, this, false);

    
    if (killed_by_na != nullptr)
    {
        UE_LOG(Enemy, Log, LOG_TEXT("%s was just killed by %s"), *actor_name, *(killed_by_na->get_actor_name()));
    }
    else
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%s was killed by UNNAMED"), *actor_name);
    }

    USkeletalMeshComponent* mesh = GetMesh();

    if (mesh == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy %s has no USkeletalMeshComponent"), *actor_name);
    }
    else
    {
        // Make it un-shootable
        GetMesh()->SetCollisionResponseToChannel(ShootableChannel, ECollisionResponse::ECR_Ignore);
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy %s is no longer shootable"), *actor_name);

        // Stop all animations
        UAnimInstance *anim_instance = GetMesh()->GetAnimInstance();
        if (anim_instance == nullptr)
        {
            UE_LOG(Enemy, Warning, LOG_TEXT("Enemy %s has no animation instance"), *actor_name);
        }
        else
        {
            anim_instance->StopAllMontages(0);
        }
        
        UE_LOG(Enemy, Verbose, LOG_TEXT("Stopped all animation montages for %s"), *actor_name);
    }
    

    
    // Get the AI Controller to destroy it
    AAIController *ai_controller = Cast<AAIController>(GetController());
    if (ai_controller == nullptr)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("Enemy %s has no AI Controller"), *actor_name);
    }
    else
    {
        ai_controller->StopMovement();
        ai_controller->UnPossess();
        if (ai_controller->Destroy())
        {
            UE_LOG(Enemy, Log, LOG_TEXT("AIController for %s destroyed"), *actor_name);
        } else
        {
            UE_LOG(Enemy, Error, LOG_TEXT("Can't destroy AIController for %s"), *actor_name);
            return;
        }
    }

    mesh->SetCollisionProfileName(FName(TEXT("Ragdoll")));
    mesh->SetSimulatePhysics(true);
    UE_LOG(Enemy, Log, LOG_TEXT("Enemy %s made ragdoll"), *actor_name);
}


FString AEnemy::get_default_actor_name() const
{
    return default_actor_name;
}


const TMap<FName, TObjectPtr<UStatsModifier>>* AEnemy::get_bone_collider_stats_modifiers() const
{
    return &stats_modifiers;
}


void AEnemy::hit_bone(const FName bone_name)
{
    /*
    Apply speed modifier
    Further modifier use can be defined in subclasses
    */
    // Get the modifier
    const TObjectPtr<UStatsModifier> modifier = stats_modifiers[bone_name];

    UE_LOG(BoneCollision, Log, LOG_TEXT("Enemy '%s' got hit in bone '%s'"), *actor_name, *(bone_name.ToString()));
    
    // Apply speed modifier
    UCharacterMovementComponent* movement_component = GetCharacterMovement();
    const float old_movement_speed = movement_component->MaxWalkSpeed;
    
    movement_component->MaxWalkSpeed *= modifier->get_speed_multiplier();

    UE_LOG(BoneCollision, Log, LOG_TEXT("Changing enemy '%s' speed: %f -> %f"), *actor_name, old_movement_speed, movement_component->MaxWalkSpeed);
}


USoundBase* AEnemy::get_damage_sound() const
{
    return take_damage_sound;
}

UPawnSensingComponent* AEnemy::get_pawn_sensing_component() const
{
    return pawn_sensing_component;
}
