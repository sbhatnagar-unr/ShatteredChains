// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "ShatteredChains/Logging.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemies/LightEnemy/LightEnemy.h"


UBTT_ChasePlayer::UBTT_ChasePlayer()
{
    NodeName = FString(TEXT("Chase Player"));
}


EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get enemy actor from task"));
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
    AActor* target_actor = enemy_actor->get_target();
    if (target_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy AI could not get player actor for enemy %s"), *enemy_actor_name);
        return EBTNodeResult::Aborted;
    }
    

    // Get locations of enemy and target
    const FVector enemy_location = enemy_actor->GetActorLocation();
    const FVector target_location = target_actor->GetActorLocation();

    // Calculate the distance
    const float distance = FVector::Dist(enemy_location, target_location);

    // If we are withing attacking range
    const float attack_range = enemy_actor->get_attack_range();
    const float sensing_radius = enemy_actor->get_pawn_sensing_component()->SightRadius;
    UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy '%s' distance to target %f (attack range=%f, sensing_radius=%f, should_chase_target=%d)"), *enemy_actor_name, distance, attack_range, sensing_radius, enemy_actor->get_should_chase_target());
    if (distance <= attack_range)
    {
        UE_LOG(Enemy, Verbose, LOG_TEXT("Target is in attacking range"));
        ai_controller->StopMovement();
        blackboard->SetValueAsBool(in_attacking_range_field, true);
    }
    // If target moved outside the sensing radius
    else if (distance > sensing_radius)
    {
        enemy_actor->set_should_chase_target(false);
        ai_controller->StopMovement();
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy '%s' stopped chasing target, it has moved outside of detection range (detection_range=%f, distance_to_target=%f)"), *enemy_actor_name, sensing_radius, distance);
    }
    // Otherwise
    else
    {
        // Chase the player
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy '%s"" chasing target"), *enemy_actor_name);
        ai_controller->MoveToActor(target_actor);
    }

    return EBTNodeResult::Type::Succeeded;
}
