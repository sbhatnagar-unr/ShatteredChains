// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"

#include "AIController.h"
#include "ShatteredChains/Logging.h"

// Sets default values
ALightEnemy::ALightEnemy() : AEnemy()
{
    attack_damage = 10;
    // Default attack range
    attack_range = 100;

    attack_animation_montage = nullptr;
    attack_animation_exit_blend = 0.2f;

    should_chase_target = false;
}

// Called when the game starts or when spawned
void ALightEnemy::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(Enemy, Log, LOG_TEXT("LightEnemy (%s): attack_range=%f attack_damage=%f"), *actor_name, attack_range, attack_damage);

    // Make sure we have an animation montage
    if (attack_animation_montage == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No attack animation montage for %s"), *actor_name);
    }

    // Here we set all the bone collision modifier values
    stats_modifiers["head"]->set_damage_multiplier(head_shot_damage_multiplier);
    stats_modifiers["head"]->set_speed_multiplier(head_shot_speed_multiplier);
    stats_modifiers["head"]->set_accuracy_multiplier(head_shot_accuracy_multiplier);

    stats_modifiers["pelvis"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["pelvis"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["pelvis"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["cc_base_pelvis"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["cc_base_pelvis"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["cc_base_pelvis"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["spine_03"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["spine_03"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["spine_03"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);
    
    stats_modifiers["upperarm_l"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_l"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["upperarm_l"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);
    
    stats_modifiers["hand_l"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["hand_l"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["hand_l"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);
    
    stats_modifiers["upperarm_r"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_r"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["upperarm_r"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);
    
    stats_modifiers["hand_r"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["hand_r"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["hand_r"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);
    
    stats_modifiers["thigh_l"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_l"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["thigh_l"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["calf_l"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["calf_l"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["calf_l"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["thigh_r"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_r"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["thigh_r"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    stats_modifiers["calf_r"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["calf_r"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["calf_r"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);
    
    sound_map["head"] = head_shot_sounds;
    sound_map["pelvis"] = torso_shot_sounds;
    sound_map["cc_base_pelvis"] = torso_shot_sounds;
    sound_map["spine_03"] = torso_shot_sounds;
    sound_map["upperarm_l"] = arm_shot_sounds;
    sound_map["hand_l"] = hand_shot_sounds;
    sound_map["upperarm_r"] = arm_shot_sounds;
    sound_map["hand_r"] = hand_shot_sounds;
    sound_map["thigh_l"] = leg_shot_sounds;
    sound_map["calf_l"] = leg_shot_sounds;
    sound_map["thigh_r"] = leg_shot_sounds;
    sound_map["calf_r"] = leg_shot_sounds;

    pawn_sensing_component->OnSeePawn.AddDynamic(this, &ALightEnemy::on_see_pawn);

}

// IDE says that parameter "pawn" should be const, but it can't be because the delegate's function signatures DOES NOT have const parameter
void ALightEnemy::on_see_pawn(APawn* pawn)
{
    if (pawn == target.Get())
    {
        should_chase_target = true;
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy '%s' sees target"), *actor_name);
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


void ALightEnemy::on_death(const AActor* killed_by)
{
    AEnemy::on_death(killed_by);
}


bool ALightEnemy::get_should_chase_target() const
{
    return should_chase_target;
}


void ALightEnemy::set_should_chase_target(const bool chase_target)
{
    should_chase_target = chase_target;
}
