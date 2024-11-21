// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ChasePlayer();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};