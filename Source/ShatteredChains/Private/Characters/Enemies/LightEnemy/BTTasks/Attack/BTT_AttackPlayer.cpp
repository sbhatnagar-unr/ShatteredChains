// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackPlayer.h"
#include "../../LightEnemy.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"
#include "../../../../Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShatteredChains/Utility.h"


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
    AAIController* ai_controller;
    // Enemy AI Blackboard
    UBlackboardComponent* blackboard;
    // Enemy Actor
    ALightEnemy* enemy_actor;
    // Enemy's target actor
    AMyCharacter* target_actor;
    // Enemy's animation instance
    UAnimInstance* anim_instance;
    // Enemy's attack animation montage
    UAnimMontage* attack_animation_montage;
    try
    {
        ai_controller = Validity::check_value<AAIController>(OwnerComp.GetAIOwner(), "Could not get AI Controller");
        blackboard = Validity::check_value<UBlackboardComponent>(ai_controller->GetBlackboardComponent(), "Could not get  AI blackboard");
        enemy_actor = Validity::check_value<ALightEnemy>((ALightEnemy*)ai_controller->GetPawn(), "Could not get enemy actor belonging to this AI");
        target_actor = (AMyCharacter*) Validity::check_value<AActor>(enemy_actor->get_target(), "Enemy AI could not get player actor");
        anim_instance = Validity::check_value<UAnimInstance>(enemy_actor->GetMesh()->GetAnimInstance(), "No anim instance found");
        attack_animation_montage = Validity::check_value<UAnimMontage>(enemy_actor->get_attack_animation_montage(), "No animation montage for attack");
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
        UE_LOG(Enemy, Verbose, LOG_TEXT("Enemy is attacking"));

        // Rotate to face the player
        FRotator rotation_to_player = UKismetMathLibrary::FindLookAtRotation(enemy_location, target_location);
        enemy_actor->SetActorRotation(rotation_to_player);
        UE_LOG(Enemy, Verbose, LOG_TEXT("Rotating to look at player"));

        // If the animatino is not active, start a new one
        if (!anim_instance->Montage_IsActive(attack_animation_montage))
        {
            // Play the animation and get its duration in seconds
            UE_LOG(Enemy, Verbose, LOG_TEXT("Starting new attack animation montage"));
            float duration = anim_instance->Montage_Play(attack_animation_montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

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
			UE_LOG(Enemy, VeryVerbose, LOG_TEXT("Stopping montage"));
			// Set the end time so next time this gets called it can start instantly
		}

		// Go back to chase player
		blackboard->SetValueAsBool(in_attacking_range_field, false);
	}

	// Succsess
	return EBTNodeResult::Type::Succeeded;
}
