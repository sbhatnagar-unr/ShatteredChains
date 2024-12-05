// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayer.h"
#include "Enemies/RangedEnemy/RangedEnemy.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"


UShootPlayer::UShootPlayer()
{
    NodeName = FString(TEXT("Shoot Player"));
}


EBTNodeResult::Type UShootPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    static const FName near_anchor_field(TEXT("NearAnchor"));

    // Enemy AI Blackboard
    UBlackboardComponent* blackboard;
    // Enemy Actor
    ARangedEnemy* enemy_actor;
    // Enemy's target actor
    AActor* target_actor;
    // Enemy's weapon
    AWeapon* weapon;
    // Enemy's anchor point
    AAnchorPoint* anchor_point;
    try
    {
        AAIController* ai_controller = Validity::check_value<AAIController>(OwnerComp.GetAIOwner(), "Could not get AI Controller");
        blackboard = Validity::check_value<UBlackboardComponent>(ai_controller->GetBlackboardComponent(), "Could not get AI blackboard");
        enemy_actor = Validity::check_value<ARangedEnemy>(Cast<ARangedEnemy>(ai_controller->GetPawn()), "Could not get enemy actor belonging to this AI");
        target_actor = Validity::check_value<AActor>(enemy_actor->get_target(), "Enemy AI could not get target actor");
        weapon = Validity::check_value<AWeapon>(enemy_actor->get_weapon(), "Enemy AI doesn't have a weapon");
        anchor_point = Validity::check_value<AAnchorPoint>(enemy_actor->get_anchor_point(), "Enemy AI doesn't have an anchor point");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
        return EBTNodeResult::Type::Failed;
    }
    
    // Rotate to face the player
    const FRotator rotation_to_player = UKismetMathLibrary::FindLookAtRotation(enemy_actor->GetActorLocation(), target_actor->GetTargetLocation());
    enemy_actor->SetActorRotation(rotation_to_player);

    // Get locations of enemy and anchor point
    const FVector enemy_location = enemy_actor->GetActorLocation();

    // Calculate distance to anchor point
    const float distance = FVector::Dist(enemy_location, anchor_point->GetActorLocation());

    if (distance >= anchor_point->get_anchor_radius())
    {
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy %s has moved away from anchor point %s"), *enemy_actor->GetActorLabel(), *anchor_point->GetActorLabel());
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