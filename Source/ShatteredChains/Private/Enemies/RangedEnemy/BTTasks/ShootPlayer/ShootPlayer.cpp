// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayer.h"
#include "Enemies/RangedEnemy/RangedEnemy.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShatteredChains/Logging.h"

#include "Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"


UShootPlayer::UShootPlayer()
{
    NodeName = FString(TEXT("Shoot Player"));
}


EBTNodeResult::Type UShootPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    static const FName near_anchor_field(TEXT("NearAnchor"));
    
    AAIController* ai_controller = OwnerComp.GetAIOwner();
    if (ai_controller == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI Controller"));
        return EBTNodeResult::Aborted;
    }

    // Enemy Actor
    ARangedEnemy* enemy_actor = Cast<ARangedEnemy>(ai_controller->GetPawn());
    if (enemy_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get enemy actor belonging to this AI"));
        return EBTNodeResult::Aborted;
    }

    const FString enemy_name = enemy_actor->get_actor_name();
    
    // Enemy AI Blackboard
    UBlackboardComponent* blackboard = ai_controller->GetBlackboardComponent();
    if (blackboard == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get AI blackboard for %s"), *enemy_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's target actor
    const AActor* target_actor = enemy_actor->get_target();
    if (target_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy AI (%s) could not get target actor"), *enemy_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's weapon
    const AWeapon* weapon = enemy_actor->get_weapon();
    if (weapon == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy AI (%s) doesn't have a weapon"), *enemy_name);
        return EBTNodeResult::Aborted;
    }

    // Enemy's anchor point
    const AAnchorPoint* anchor_point = enemy_actor->get_anchor_point();
    if (anchor_point == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Enemy AI (%s) doesn't have an anchor point"), *enemy_name);
        return EBTNodeResult::Aborted;
    }
    
    // Rotate to face the player
    const FRotator rotation_to_player = UKismetMathLibrary::FindLookAtRotation(enemy_actor->GetActorLocation(), target_actor->GetTargetLocation());
    enemy_actor->SetActorRotation(rotation_to_player);


    // Get locations of enemy and anchor point
    const FVector enemy_location = enemy_actor->GetActorLocation();

    // Calculate distance to anchor point
    const float distance = FVector::Dist(enemy_location, anchor_point->GetActorLocation());

    if (distance > anchor_point->get_anchor_radius() + enemy_actor->get_anchor_tolerance())
    {
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy %s has moved away from anchor point %s"), *enemy_name, *(anchor_point->get_actor_name()));
        blackboard->SetValueAsBool(near_anchor_field, false);
        return EBTNodeResult::Succeeded;
    }
    
    if (weapon->get_current_magazine_ammo_count() > 0)
    {
        weapon->fire();
    }
    else
    {
        weapon->reload();
    }
    return EBTNodeResult::Succeeded;
}