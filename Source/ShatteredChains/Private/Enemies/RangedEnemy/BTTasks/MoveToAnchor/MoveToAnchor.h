// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MoveToAnchor.generated.h"

/**
 * 
 */
UCLASS()
class SHATTEREDCHAINS_API UMoveToAnchor : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UMoveToAnchor();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
};
