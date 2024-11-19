// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackPlayer.h"
#include "ShatteredChains/Logging.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_AttackPlayer::UBTT_AttackPlayer()
{
	NodeName = FString(TEXT("Attack Player"));
}


EBTNodeResult::Type UBTT_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//// Get the blackboard
	//TObjectPtr<UBlackboardComponent> blackboard = OwnerComp.GetBlackboardComponent();

	//// Field names that dont change
	//static const FName in_attacking_range_field(TEXT("in_attacking_range"));
	//static const FName enemy_actor_field(TEXT("SelfActor"));

	//// The enemy that this AI belongs to
	//ACharacter *enemy_actor = (ACharacter*) blackboard->GetValueAsObject(enemy_actor_field);
	//
	//// Get the enemys location
	UE_LOG(Enemy, Log, TEXT("ATTACK!!!!!"));

	return EBTNodeResult::Type::Succeeded;
}