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

    float get_damage_multiplier() const;
    float get_speed_multiplier() const;
    float get_accuracy_multiplier() const;

    void set_damage_multiplier(const float new_damage_multiplier);
    void set_speed_multiplier(const float new_speed_multiplier);
    void set_accuracy_multiplier(const float new_accuracy_multiplier);

private:
    float damage_multiplier;
    float speed_multiplier;
    float accuracy_multiplier;
};