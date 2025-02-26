// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToAnchor.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "Enemies/RangedEnemy/RangedEnemy.h"

UMoveToAnchor::UMoveToAnchor()
{
    NodeName = FString("Move to anchor");
}


EBTNodeResult::Type UMoveToAnchor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /*
    Yes, I know this function is very similar to the chase player behavior tree task
    But the naming of things is different and there may be different functionality in
    the future so they are separated in case of this.
    */
    static const FName near_anchor_field(TEXT("NearAnchor"));

    // Enemy AI Controller
    AAIController* ai_controller = OwnerComp.GetAIOwner();
    if (ai_controller == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI Controller"));
        return EBTNodeResult::Aborted;
    }

    // Enemy AI Blackboard
    UBlackboardComponent* blackboard = ai_controller->GetBlackboardComponent();
    if (blackboard == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI blackboard"));
        return EBTNodeResult::Aborted;
    }

    // The enemy actor
    ARangedEnemy* enemy_actor = Cast<ARangedEnemy>(ai_controller->GetPawn());
    if (enemy_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get enemy actor belonging to this AI"));
        return EBTNodeResult::Aborted;
    }

    // The anchor point
    AAnchorPoint* anchor_point = enemy_actor->get_anchor_point();
    if (anchor_point == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get anchor point"));
        return EBTNodeResult::Aborted;
    }

    // The location the enemy should move to
    const FVector anchor_location = enemy_actor->get_location_to_go_to();

    // The enemy's name, for logging purposes
    const FString enemy_name = enemy_actor->get_actor_name();

    // Get locations of enemy
    const FVector enemy_location = enemy_actor->GetActorLocation();

    // Calculate distance to anchor location
    const float distance = FVector::Dist(enemy_location, anchor_location);

    if (distance <= enemy_actor->get_anchor_tolerance())
    {
        UE_LOG(Enemy, Log, LOG_TEXT("%s has reached anchor point %s"), *enemy_name, *(anchor_point->get_actor_name()));
        ai_controller->StopMovement();
        blackboard->SetValueAsBool(near_anchor_field, true);
    }
    else
    {
        // Acceptance radius is 0 because that will be handled by the logic in this function
        ai_controller->MoveToLocation(anchor_location, 0, false, true, false, true);
        UE_LOG(Enemy, VeryVerbose, LOG_TEXT("%s moving to anchor (%s -> %s) %f units away"), *enemy_name, *enemy_location.ToString(), *anchor_location.ToString(), distance);
    }
    
    return EBTNodeResult::Succeeded;
}
