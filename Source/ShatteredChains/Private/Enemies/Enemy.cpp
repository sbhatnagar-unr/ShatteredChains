// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "AIController.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/CustomTraceChannels.h"

DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    target = nullptr;
    health_component = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Get the target if one was not set in the editor
    if (target == nullptr)
    {
        UE_LOG(Enemy, Log, LOG_TEXT("%s had no target set in editor, using player as target"), *GetActorLabel());
        target = GetWorld()->GetFirstPlayerController()->GetPawn();

        if (target == nullptr)
        {
            UE_LOG(Enemy, Error, LOG_TEXT("%s could not locate target (player)"), *GetActorLabel());
            return;
        }
        UE_LOG(Enemy, Verbose, LOG_TEXT("Found target (player)"));
    }
}
    
// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

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
    UE_LOG(Enemy, Log, LOG_TEXT("%s was just killed by %s"), *GetActorLabel(), (killed_by == nullptr) ? *FString("UNKNOWN") : *killed_by->GetActorLabel());

    USkeletalMeshComponent* mesh = GetMesh();

    if (mesh == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy %s has no USkeletalMeshComponent"), *GetActorLabel());
    }
    else
    {
        // Make it un-shootable
        GetMesh()->SetCollisionResponseToChannel(ShootableChannel, ECollisionResponse::ECR_Ignore);
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy %s is no longer shootable"), *GetActorLabel());

        // Stop all animations
        UAnimInstance *anim_instance = GetMesh()->GetAnimInstance();
        if (anim_instance == nullptr)
        {
            UE_LOG(Enemy, Warning, LOG_TEXT("Enemy %s has no animation instance"), *GetActorLabel());
        }
        else
        {
            anim_instance->StopAllMontages(0);
        }
        
        UE_LOG(Enemy, Verbose, LOG_TEXT("Stopped all animation montages for %s"), *GetActorLabel());
    }
    

    
    // Get the AI Controller to destroy it
    AAIController *ai_controller = Cast<AAIController>(GetController());
    if (ai_controller == nullptr)
    {
        UE_LOG(Enemy, Warning, LOG_TEXT("Enemy %s has no AI Controller"), *GetActorLabel());
    }
    else
    {
        ai_controller->StopMovement();
        ai_controller->UnPossess();
        if (ai_controller->Destroy())
        {
            UE_LOG(Enemy, Log, LOG_TEXT("AIController for %s destroyed"), *GetActorLabel());
        } else
        {
            UE_LOG(Enemy, Error, LOG_TEXT("Can't destroy AIController for %s"), *GetActorLabel());
            return;
        }
    }

    mesh->SetCollisionProfileName(FName(TEXT("Ragdoll")));
    mesh->SetSimulatePhysics(true);
}