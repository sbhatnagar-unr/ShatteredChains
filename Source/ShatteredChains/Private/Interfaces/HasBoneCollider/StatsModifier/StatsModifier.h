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

    float get_additive_damage_modifier() const;
    float get_multiplicative_damage_modifier() const;

    void set_additive_damage_modifier(const float new_additive_damage_modifier);
    void set_multiplicative_damage_modifier(const float new_multiplicative_damage_modifier);

private:
    // These are applied to the base damage
    // Additive is applied first
    // Then multiplicative is applied to the result
    float additive_damage_modifier;
    float multiplicative_damage_modifier;
};