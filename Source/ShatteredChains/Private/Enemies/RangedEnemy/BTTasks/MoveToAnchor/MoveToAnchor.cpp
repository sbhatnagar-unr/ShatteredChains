// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToAnchor.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "Enemies/RangedEnemy/RangedEnemy.h"
#include "ShatteredChains/Utility.h"

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
    AAIController* ai_controller;
    // Enemy AI Blackboard
    UBlackboardComponent* blackboard;
    // The enemy actor
    ARangedEnemy* enemy_actor;
    // The anchor point
    AAnchorPoint* anchor_point;
    // The location the enemy should move to
    FVector anchor_location;
    // The enemy's name, for logging purposes
    FString enemy_name;
    try
    {
        ai_controller = Validity::check_value<AAIController>(OwnerComp.GetAIOwner(), "Could not get AI Controller");
        blackboard = Validity::check_value<UBlackboardComponent>(ai_controller->GetBlackboardComponent(), "Could not get AI blackboard");
        enemy_actor = Validity::check_value<ARangedEnemy>(Cast<ARangedEnemy>(ai_controller->GetPawn()), "Could not get enemy actor belonging to this AI");
        anchor_point = Validity::check_value<AAnchorPoint>(enemy_actor->get_anchor_point(), "Could not get anchor point");
        anchor_location = enemy_actor->get_location_to_go_to();
        enemy_name = enemy_actor->GetActorLabel();
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
        return EBTNodeResult::Type::Failed;
    }

    // Get locations of enemy
    const FVector enemy_location = enemy_actor->GetActorLocation();

    // Calculate distance to anchor location
    const float distance = FVector::Dist(enemy_location, anchor_location);

    if (distance <= enemy_actor->get_anchor_tolerance())
    {
        UE_LOG(Enemy, Log, LOG_TEXT("%s has reached anchor point %s"), *enemy_name, *anchor_point->GetActorLabel());
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
