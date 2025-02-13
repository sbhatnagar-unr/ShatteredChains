// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"

// Sets default values
ALightEnemy::ALightEnemy() : AEnemy()
{
    attack_damage = 10;
    // Default attack range
    attack_range = 100;

    attack_animation_montage = nullptr;
    attack_animation_exit_blend = 0.2f;
    
    target_detection_range = 3000;
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
    stats_modifiers["head"]->set_additive_damage_modifier(head_shot_damage_adder);
    stats_modifiers["head"]->set_multiplicative_damage_modifier(head_shot_damage_multiplier);
    stats_modifiers["head"]->set_additive_speed_modifier(head_shot_speed_adder);
    stats_modifiers["head"]->set_multiplicative_speed_modifier(head_shot_speed_multiplier);

    stats_modifiers["pelvis"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["pelvis"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["pelvis"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["pelvis"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["cc_base_pelvis"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["cc_base_pelvis"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["cc_base_pelvis"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["cc_base_pelvis"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);

    stats_modifiers["spine_03"]->set_additive_damage_modifier(torso_shot_damage_adder);
    stats_modifiers["spine_03"]->set_multiplicative_damage_modifier(torso_shot_damage_multiplier);
    stats_modifiers["spine_03"]->set_additive_speed_modifier(torso_shot_speed_adder);
    stats_modifiers["spine_03"]->set_multiplicative_speed_modifier(torso_shot_speed_multiplier);
    
    stats_modifiers["upperarm_l"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["upperarm_l"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_l"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["upperarm_l"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);
    
    stats_modifiers["hand_l"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["hand_l"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["hand_l"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["hand_l"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);
    
    stats_modifiers["upperarm_r"]->set_additive_damage_modifier(arm_shot_damage_adder);
    stats_modifiers["upperarm_r"]->set_multiplicative_damage_modifier(arm_shot_damage_multiplier);
    stats_modifiers["upperarm_r"]->set_additive_speed_modifier(arm_shot_speed_adder);
    stats_modifiers["upperarm_r"]->set_multiplicative_speed_modifier(arm_shot_speed_multiplier);
    
    stats_modifiers["hand_r"]->set_additive_damage_modifier(hand_shot_damage_adder);
    stats_modifiers["hand_r"]->set_multiplicative_damage_modifier(hand_shot_damage_multiplier);
    stats_modifiers["hand_r"]->set_additive_speed_modifier(hand_shot_speed_adder);
    stats_modifiers["hand_r"]->set_multiplicative_speed_modifier(hand_shot_speed_multiplier);
    
    stats_modifiers["thigh_l"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["thigh_l"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_l"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["thigh_l"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["calf_l"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["calf_l"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["calf_l"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["calf_l"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["thigh_r"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["thigh_r"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["thigh_r"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["thigh_r"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);
    
    stats_modifiers["calf_r"]->set_additive_damage_modifier(leg_shot_damage_adder);
    stats_modifiers["calf_r"]->set_multiplicative_damage_modifier(leg_shot_damage_multiplier);
    stats_modifiers["calf_r"]->set_additive_speed_modifier(leg_shot_speed_adder);
    stats_modifiers["calf_r"]->set_multiplicative_speed_modifier(leg_shot_speed_multiplier);

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


float ALightEnemy::get_target_detection_range() const
{
    return target_detection_range;
}
