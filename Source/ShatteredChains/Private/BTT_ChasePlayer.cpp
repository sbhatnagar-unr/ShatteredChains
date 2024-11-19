// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "ShatteredChains/Logging.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "LightEnemy.h"

UBTT_ChasePlayer::UBTT_ChasePlayer()
{
	NodeName = FString(TEXT("Chase Player"));
}


EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller
	AAIController* ai_controller = OwnerComp.GetAIOwner();
	// Make sure AI Controller is valid
	if (ai_controller == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("Enemy AI could not get AI Controller"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// Get the blackboard
	UBlackboardComponent* blackboard = ai_controller->GetBlackboardComponent();
	// Make sure the blackboard is valid
	if (blackboard == nullptr)
	{
		UE_LOG(Enemy, Error, TEXT("Could not get Light Enemy AI blackboard"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// Field names that dont change
	static const FName in_attacking_range_field(TEXT("in_attacking_range"));


	// The enemy that this AI belongs to
	ALightEnemy* enemy_actor = (ALightEnemy*) ai_controller->GetPawn();
	// Make sure the enemy_actor is valid
	if (enemy_actor == nullptr)
	{
		UE_LOG(Enemy, Error, TEXT("Could not get enemy actor belonging to this AI"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// The target to chase
	AActor* target_actor = enemy_actor->get_target();

	// Check if there is a target
	if (target_actor == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("Enemy AI could not get player actor"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// Get locations of enemy and target
	FVector enemy_location = enemy_actor->GetActorLocation();
	FVector target_location = target_actor->GetActorLocation();
	
	// Calculate the distance
	float distance = FVector::Dist(enemy_location, target_location);

	// If we are withing attacking range
	float attack_range = enemy_actor->get_attack_range();
	UE_LOG(Enemy, Log, TEXT("Enemy distance to target %f (attack range: %f)"), distance, attack_range);
	if (distance <= attack_range)
	{
		UE_LOG(Enemy, Log, TEXT("Enemy is in attacking range"));
		ai_controller->StopMovement();
		blackboard->SetValueAsBool(in_attacking_range_field, true);
		return EBTNodeResult::Type::Aborted;
	}
	// Otherwise
	else
	{
		// Chase the player
		UE_LOG(Enemy, Log, TEXT("Enemy AI chasing player"));
		ai_controller->MoveToActor(target_actor);
	}

	return EBTNodeResult::Type::Succeeded;
}
