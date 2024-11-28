// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayer.h"
#include "Enemies/RangedEnemy/RangedEnemy.h"
#include "AIController.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "Weapons/Weapon.h"


UShootPlayer::UShootPlayer()
{
    NodeName = FString(TEXT("Shoot Player"));
}


EBTNodeResult::Type UShootPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    // Enemy AI Controller
    AAIController* ai_controller;
    // Enemy AI Blackboard
    UBlackboardComponent* blackboard;
    // Enemy Actor
    ARangedEnemy* enemy_actor;
    // Enemy's target actor
    AActor* target_actor;
    // Enemy's weapon
    AWeapon* weapon;
    try
    {
        ai_controller = Validity::check_value<AAIController>(OwnerComp.GetAIOwner(), "Could not get AI Controller");
        blackboard = Validity::check_value<UBlackboardComponent>(ai_controller->GetBlackboardComponent(), "Could not get AI blackboard");
        enemy_actor = Validity::check_value<ARangedEnemy>(Cast<ARangedEnemy>(ai_controller->GetPawn()), "Could not get enemy actor belonging to this AI");
        target_actor = Validity::check_value<AActor>(enemy_actor->get_target(), "Enemy AI could not get target actor");
        weapon = Validity::check_value<AWeapon>(enemy_actor->get_weapon(), "Enemy AI doesn't have a weapon");
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
        return EBTNodeResult::Type::Failed;
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