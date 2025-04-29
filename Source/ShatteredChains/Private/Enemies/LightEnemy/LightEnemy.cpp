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
    stats_modifiers["Neck"]->set_damage_multiplier(head_shot_damage_multiplier);
    stats_modifiers["Neck"]->set_speed_multiplier(head_shot_speed_multiplier);
    stats_modifiers["Neck"]->set_accuracy_multiplier(head_shot_accuracy_multiplier);

    stats_modifiers["Head"]->set_damage_multiplier(head_shot_damage_multiplier);
    stats_modifiers["Head"]->set_speed_multiplier(head_shot_speed_multiplier);
    stats_modifiers["Head"]->set_accuracy_multiplier(head_shot_accuracy_multiplier);
    
    stats_modifiers["hips"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["hips"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["hips"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["LeftUpLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftUpLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftUpLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["LeftLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["LeftFoot"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftFoot"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftFoot"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["LeftToeBase"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["LeftToeBase"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["LeftToeBase"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightUpLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightUpLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightUpLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightLeg"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightLeg"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightLeg"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightFoot"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightFoot"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightFoot"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["RightToeBase"]->set_damage_multiplier(leg_shot_damage_multiplier);
    stats_modifiers["RightToeBase"]->set_speed_multiplier(leg_shot_speed_multiplier);
    stats_modifiers["RightToeBase"]->set_accuracy_multiplier(leg_shot_accuracy_multiplier);

    stats_modifiers["Spine"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["Spine1"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine1"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine1"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["Spine2"]->set_damage_multiplier(torso_shot_damage_multiplier);
    stats_modifiers["Spine2"]->set_speed_multiplier(torso_shot_speed_multiplier);
    stats_modifiers["Spine2"]->set_accuracy_multiplier(torso_shot_accuracy_multiplier);

    stats_modifiers["LeftShoulder"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["LeftShoulder"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["LeftShoulder"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["LeftArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["LeftArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["LeftArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["LeftForeArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["LeftForeArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["LeftForeArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["RightShoulder"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["RightShoulder"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["RightShoulder"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["RightArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["RightArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["RightArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["RightForeArm"]->set_damage_multiplier(arm_shot_damage_multiplier);
    stats_modifiers["RightForeArm"]->set_speed_multiplier(arm_shot_speed_multiplier);
    stats_modifiers["RightForeArm"]->set_accuracy_multiplier(arm_shot_accuracy_multiplier);

    stats_modifiers["LeftHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandIndex1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandIndex1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandIndex1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandIndex2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandIndex2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandIndex2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandMiddle1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandMiddle1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandMiddle1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandMiddle2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandMiddle2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandMiddle2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandRing1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandRing1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandRing1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandRing2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandRing2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandRing2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandThumb1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandThumb1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandThumb1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["LeftHandThumb2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["LeftHandThumb2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["LeftHandThumb2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHand"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHand"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHand"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandIndex1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandIndex1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandIndex1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandIndex2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandIndex2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandIndex2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandMiddle1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandMiddle1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandMiddle1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandMiddle2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandMiddle2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandMiddle2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandRing1"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandRing1"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandRing1"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandRing2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandRing2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandRing2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);

    stats_modifiers["RightHandThumb2"]->set_damage_multiplier(hand_shot_damage_multiplier);
    stats_modifiers["RightHandThumb2"]->set_speed_multiplier(hand_shot_speed_multiplier);
    stats_modifiers["RightHandThumb2"]->set_accuracy_multiplier(hand_shot_accuracy_multiplier);
    
    sound_map["Neck"] = head_shot_sounds;
    sound_map["Head"] = head_shot_sounds;
    sound_map["hips"] = torso_shot_sounds;
    sound_map["LeftUpLeg"] = leg_shot_sounds;
    sound_map["LeftLeg"] = leg_shot_sounds;
    sound_map["LeftFoot"] = leg_shot_sounds;
    sound_map["LeftToeBase"] = leg_shot_sounds;
    sound_map["RightUpLeg"] = leg_shot_sounds;
    sound_map["RightLeg"] = leg_shot_sounds;
    sound_map["RightFoot"] = leg_shot_sounds;
    sound_map["RightToeBase"] = leg_shot_sounds;
    sound_map["Spine"] = torso_shot_sounds;
    sound_map["Spine1"] = torso_shot_sounds;
    sound_map["Spine2"] = torso_shot_sounds;
    sound_map["LeftShoulder"] = arm_shot_sounds;
    sound_map["LeftArm"] = arm_shot_sounds;
    sound_map["LeftForeArm"] = arm_shot_sounds;
    sound_map["RightShoulder"] = arm_shot_sounds;
    sound_map["RightArm"] = arm_shot_sounds;
    sound_map["RightForeArm"] = arm_shot_sounds;
    sound_map["LeftHand"] = hand_shot_sounds;
    sound_map["LeftHandIndex1"] = hand_shot_sounds;
    sound_map["LeftHandIndex2"] = hand_shot_sounds;
    sound_map["LeftHandMiddle1"] = hand_shot_sounds;
    sound_map["LeftHandMiddle2"] = hand_shot_sounds;
    sound_map["LeftHandRing1"] = hand_shot_sounds;
    sound_map["LeftHandRing2"] = hand_shot_sounds;
    sound_map["LeftHandThumb1"] = hand_shot_sounds;
    sound_map["LeftHandThumb2"] = hand_shot_sounds;
    sound_map["RightHand"] = hand_shot_sounds;
    sound_map["RightHandIndex1"] = hand_shot_sounds;
    sound_map["RightHandIndex2"] = hand_shot_sounds;
    sound_map["RightHandMiddle1"] = hand_shot_sounds;
    sound_map["RightHandMiddle2"] = hand_shot_sounds;
    sound_map["RightHandRing1"] = hand_shot_sounds;
    sound_map["RightHandRing2"] = hand_shot_sounds;
    sound_map["RightHandThumb2"] = hand_shot_sounds;
    
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


void ALightEnemy::on_death(const AActor* killed_by, const bool play_death_sound)
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
