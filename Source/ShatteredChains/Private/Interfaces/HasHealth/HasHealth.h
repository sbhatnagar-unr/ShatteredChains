// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"
#include "UObject/Interface.h"
#include "HasHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHasHealth : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class IHasHealth
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION()
    virtual void on_death(AActor* killed_by) PURE_VIRTUAL(IHasHealth::on_death, return;);
    
    virtual UHealthComponent* get_health_component() PURE_VIRTUAL(IHasHealth::on_death, return nullptr;);
};
