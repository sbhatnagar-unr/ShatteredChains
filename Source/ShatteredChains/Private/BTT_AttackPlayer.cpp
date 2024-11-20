// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackPlayer.h"
#include "LightEnemy.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


UBTT_AttackPlayer::UBTT_AttackPlayer()
{
	NodeName = FString(TEXT("Attack Player"));
}


EBTNodeResult::Type UBTT_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI Controller
	AAIController* ai_controller = OwnerComp.GetAIOwner();
	// Make sure AI Controller is valid
	if (ai_controller == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("BTT_AttackPlayer enemy AI could not get AI Controller"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// Get the blackboard
	UBlackboardComponent* blackboard = ai_controller->GetBlackboardComponent();
	// Make sure the blackboard is valid
	if (blackboard == nullptr)
	{
		UE_LOG(Enemy, Error, TEXT("BTT_AttackPlayer could not get Light Enemy AI blackboard"));
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
		UE_LOG(Enemy, Error, TEXT("BTT_AttackPlayer could not get enemy actor belonging to this AI"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// The target to chase
	AActor* target_actor = enemy_actor->get_target();

	// Check if there is a target
	if (target_actor == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("BTT_AttackPlayer enemy AI could not get player actor"));
		// In here, blackboard in_attacking_range is already false, or else this function wouldnt have ran to begin with
		return EBTNodeResult::Type::Failed;
	}

	// Get the anim instance
	UAnimInstance* anim_instance = enemy_actor->GetMesh()->GetAnimInstance();
	if (anim_instance == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("BTT_AttackPlayer no anim instance for Light Enemy"));
		// return early
		return EBTNodeResult::Type::Failed;
	}

	// Get the attack animation
	UAnimMontage* attack_animation_montage = enemy_actor->get_attack_animation_montage();
	if (attack_animation_montage == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("BTT_AttackPlayer no animation montage for Light Enemy attack"));
		// return early
		return EBTNodeResult::Type::Failed;
	}


	// Get locations of enemy and target
	FVector enemy_location = enemy_actor->GetActorLocation();
	FVector target_location = target_actor->GetActorLocation();

	// Calculate the distance
	float distance = FVector::Dist(enemy_location, target_location);

	// If we are withing attacking range
	float attack_range = enemy_actor->get_attack_range();
	UE_LOG(Enemy, VeryVerbose, TEXT("BTT_AttackPlayer enemy distance to target %f (attack range: %f)"), distance, attack_range);

	if (distance <= attack_range)
	{
		UE_LOG(Enemy, Log, TEXT("BTT_AttackPlayer enemy is attacking"));	

		// Rotate to face the player
		FRotator rotation_to_player = UKismetMathLibrary::FindLookAtRotation(enemy_location, target_location);
		enemy_actor->SetActorRotation(rotation_to_player);
		UE_LOG(Enemy, Verbose, TEXT("BTT_AttackPlayer rotating to look at player"));

		// Play the animation
		float duration = anim_instance->Montage_Play(attack_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// If duration == 0.f that means an error
		if (duration == 0.f)
		{
			UE_LOG(Enemy, Error, TEXT("BTT_AttackPlayer Light Enemy could not play attack animation montage"));
			// return early
			return EBTNodeResult::Type::Failed;
		}	
	}
	// Otherwise
	else
	{
		// Stop the montage
		if (anim_instance->Montage_IsPlaying(attack_animation_montage))
		{
			anim_instance->Montage_Stop(enemy_actor->get_attack_animation_exit_blend(), attack_animation_montage);
			UE_LOG(Enemy, Verbose, TEXT("BTT_AttackPlayer stopping montage"));
		}

		// Go back to chase player
		UE_LOG(Enemy, Log, TEXT("BTT_AttackPlayer target is out of range"));
		blackboard->SetValueAsBool(in_attacking_range_field, false);

	}
	return EBTNodeResult::Type::Succeeded;
}