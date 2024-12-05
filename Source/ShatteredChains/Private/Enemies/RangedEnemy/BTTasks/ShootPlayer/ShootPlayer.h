// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ShootPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SHATTEREDCHAINS_API UShootPlayer : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UShootPlayer();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
};
