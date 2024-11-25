// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "ShatteredChains/Logging.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShatteredChains/Utility.h"
#include "../../LightEnemy.h"


UBTT_ChasePlayer::UBTT_ChasePlayer()
{
    NodeName = FString(TEXT("Chase Player"));
}


EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    static const FName in_attacking_range_field(TEXT("in_attacking_range"));

    // Enemy AI Controller
    AAIController* ai_controller;
    // Enemy AI Blackboard
    UBlackboardComponent* blackboard;
    // Enemy Actor
    ALightEnemy* enemy_actor;
    // Enemy's target actor
    AActor* target_actor;
    // Enemy's animation instance
    try
    {
        ai_controller = Validity::check_value<AAIController>(OwnerComp.GetAIOwner(), "Could not get AI Controller");
        blackboard = Validity::check_value<UBlackboardComponent>(ai_controller->GetBlackboardComponent(), "Could not get AI blackboard");
        enemy_actor = Validity::check_value<ALightEnemy>((ALightEnemy*)ai_controller->GetPawn(), "Could not get enemy actor belonging to this AI");
        target_actor = Validity::check_value<AActor>(enemy_actor->get_target(), "Enemy AI could not get player actor");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
        return EBTNodeResult::Type::Failed;
    }

    // Get locations of enemy and target
    FVector enemy_location = enemy_actor->GetActorLocation();
    FVector target_location = target_actor->GetActorLocation();

    // Calculate the distance
    float distance = FVector::Dist(enemy_location, target_location);

    // If we are withing attacking range
    float attack_range = enemy_actor->get_attack_range();
    UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Enemy distance to target %f (attack range: %f)"), distance, attack_range);
    if (distance <= attack_range)
    {
        UE_LOG(Enemy, Verbose, LOG_TEXT("Target is in attacking range"));
        ai_controller->StopMovement();
        blackboard->SetValueAsBool(in_attacking_range_field, true);
    }
    // Otherwise
    else
    {
        // Chase the player
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy AI chasing player"));
        ai_controller->MoveToActor(target_actor);
    }

    return EBTNodeResult::Type::Succeeded;
}
