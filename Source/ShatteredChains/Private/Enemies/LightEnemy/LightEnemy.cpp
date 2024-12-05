// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"

// Sets default values
ALightEnemy::ALightEnemy() : AEnemy()
{
    attack_damage = 0;
    // Default attack range
    attack_range = 100;

    attack_animation_montage = nullptr;
    attack_animation_exit_blend = 0.2f;
}

// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(Enemy, Log, LOG_TEXT("LightEnemy: attack_range=%f attack_damage=%f"), attack_range, attack_damage);

    // Make sure we have an animation montage
    if (attack_animation_montage == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No attack animation montage"));
    }

}


float ALightEnemy::get_attack_damage() const
{
    return attack_damage;
}


float ALightEnemy::get_attack_range() const
{
    return attack_range;
}


UAnimMontage* ALightEnemy::get_attack_animation_montage() const
{
    return attack_animation_montage;
}

float ALightEnemy::get_attack_animation_exit_blend() const
{
    return attack_animation_exit_blend;
}


void ALightEnemy::on_death(AActor* killed_by)
{
    AEnemy::on_death(killed_by);
}