// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackPlayer.h"
#include "Enemies/LightEnemy/LightEnemy.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


UBTT_AttackPlayer::UBTT_AttackPlayer()
{
    NodeName = FString(TEXT("Attack Player"));
}


EBTNodeResult::Type UBTT_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /*
    Plays attack montage
    Actual "attacking", as in applying damage, is done in Animation Notifiers
    */
    static const FName in_attacking_range_field(TEXT("in_attacking_range"));

    // Enemy AI Controller
    AAIController* ai_controller = OwnerComp.GetAIOwner();
    if (ai_controller == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI Controller"));
        return EBTNodeResult::Aborted;
    }

    // Enemy Actor
    ALightEnemy* enemy_actor = Cast<ALightEnemy>(ai_controller->GetPawn());
    if (enemy_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get enemy actor belonging to this AI"));
        return EBTNodeResult::Aborted;
    }
    
    const FString enemy_actor_name = enemy_actor->get_actor_name();

    // Enemy AI Blackboard
    UBlackboardComponent* blackboard = ai_controller->GetBlackboardComponent();
    if (blackboard == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI blackboard for enemy %s"), *enemy_actor_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's target actor
    const AMyCharacter* target_actor = Cast<AMyCharacter>(enemy_actor->get_target());
    if (target_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy AI could not get target actor for enemy %s"), *enemy_actor_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's animation instance
    UAnimInstance* anim_instance = enemy_actor->GetMesh()->GetAnimInstance();
    if (anim_instance == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No anim instance found for enemy %s"), *enemy_actor_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's attack animation montage
    UAnimMontage* attack_animation_montage = enemy_actor->get_attack_animation_montage();
    if (attack_animation_montage == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No animation montage for attack for enemy %s"), *enemy_actor_name);
        return EBTNodeResult::Aborted;
    }



    // Get locations of enemy and target
    const FVector enemy_location = enemy_actor->GetActorLocation();
    const FVector target_location = target_actor->GetActorLocation();

    // Calculate the distance
    const float distance = FVector::Dist(enemy_location, target_location);

    // If we are withing attacking range
    const float attack_range = enemy_actor->get_attack_range();
    UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy distance to target %f (attack range: %f)"), distance, attack_range);


    // Don't shoot dead things
    if (const IHasHealth* target_health = Cast<IHasHealth>(target_actor))
    {
        if (target_health->get_health_component()->dead())
        {
            UE_LOG(Enemy, Log, LOG_TEXT("Light enemy '%s' target is already dead"), *enemy_actor_name);
            return EBTNodeResult::Succeeded;
        }
    }
    
    if (distance <= attack_range)
    {
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy is attacking"));

        // Rotate to face the player
        const FRotator rotation_to_player = UKismetMathLibrary::FindLookAtRotation(enemy_location, target_location);
        enemy_actor->SetActorRotation(rotation_to_player);
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Rotating to look at player"));

        // If the animation is not active, start a new one
        if (!anim_instance->Montage_IsActive(attack_animation_montage))
        {
            // Play the animation and get its duration in seconds
            UE_LOG(Enemy, Verbose, LOG_TEXT("Starting new attack animation montage"));
            const float duration = anim_instance->Montage_Play(attack_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

            // If duration == 0.f that means an error
            if (duration == 0.f)
            {
                UE_LOG(Enemy, Error, LOG_TEXT("Could not play attack animation montage"));
                // return early
                return EBTNodeResult::Type::Failed;
            }
        }
    }
    // Otherwise
    else
    {
        UE_LOG(Enemy, Verbose, LOG_TEXT("Target is out of attacking range"));

        // Stop the montage
        if (anim_instance->Montage_IsActive(attack_animation_montage))
        {
            anim_instance->Montage_Stop(enemy_actor->get_attack_animation_exit_blend(), attack_animation_montage);
            UE_LOG(Enemy, Verbose, LOG_TEXT("Stopping montage"));
            // Set the end time so next time this gets called it can start instantly
        }

        // Go back to chase player
        blackboard->SetValueAsBool(in_attacking_range_field, false);
    }

    // Success
    return EBTNodeResult::Type::Succeeded;
}
