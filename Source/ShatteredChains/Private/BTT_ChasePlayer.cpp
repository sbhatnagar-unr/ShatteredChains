// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "ShatteredChains/Logging.h"
#include "AIController.h"


UBTT_ChasePlayer::UBTT_ChasePlayer()
{
	NodeName = FString(TEXT("Chase Player"));
}


EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player actor
	APawn *target_actor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Check if it was successful
	if (target_actor == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("Enemy AI could not get player actor"));
		return EBTNodeResult::Type::Failed;
	}

	// Get AI Controller
	AAIController *ai_controller = OwnerComp.GetAIOwner();

	// Make sure AI Controller is valid
	if (ai_controller == nullptr)
	{
		UE_LOG(Enemy, Warning, TEXT("Enemy AI could not get AI Controller"));
		return EBTNodeResult::Type::Failed;
	}

	// APawn subclasses AActor
	UE_LOG(Enemy, Log, TEXT("Enemy AI chasing player"));
	ai_controller->MoveToActor((AActor*) target_actor, 50.0f);


	return EBTNodeResult::Type::Succeeded;
}