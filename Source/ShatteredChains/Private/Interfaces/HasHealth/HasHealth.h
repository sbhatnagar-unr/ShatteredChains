// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent/HealthComponent.h"
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
    virtual void on_death(const AActor* killed_by, const bool play_death_sound=true) PURE_VIRTUAL(IHasHealth::on_death, return;);
    virtual UHealthComponent* get_health_component() const PURE_VIRTUAL(IHasHealth::get_health_component, return nullptr;);
};
