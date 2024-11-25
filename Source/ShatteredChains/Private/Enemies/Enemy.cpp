// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(Enemy);

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    // Default attack range
    attack_range = 100;

    attack_animation_montage = nullptr;
    attack_animation_exit_blend = 0.2f;
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


float AEnemy::get_attack_range() const
{
    return attack_range;
}


UAnimMontage* AEnemy::get_attack_animation_montage() const
{
    return attack_animation_montage;
}

float AEnemy::get_attack_animation_exit_blend() const
{
    return attack_animation_exit_blend;
}
