// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatsModifier.generated.h"

/**
 * 
 */
UCLASS()
class SHATTEREDCHAINS_API UStatsModifier : public UObject
{
    GENERATED_BODY()
public:
    UStatsModifier();

    float get_additive_health_modifier() const;
    float get_multiplicative_health_modifier() const;

    void set_additive_health_modifier(const float new_additive_health_modifier);
    void set_multiplicative_health_modifier(const float new_multiplicative_health_modifier);

private:
    float additive_health_modifier;
    float multiplicative_health_modifier;
};
