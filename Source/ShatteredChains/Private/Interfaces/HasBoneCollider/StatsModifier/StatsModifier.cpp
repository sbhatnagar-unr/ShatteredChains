// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsModifier.h"

UStatsModifier::UStatsModifier()
{
    // Default to values that won't modify anything
    damage_multiplier = 1;
    speed_multiplier = 1;
    accuracy_multiplier = 1;
}

float UStatsModifier::get_damage_multiplier() const
{
    return damage_multiplier;
}


float UStatsModifier::get_speed_multiplier() const
{
    return speed_multiplier;
}


float UStatsModifier::get_accuracy_multiplier() const
{
    return accuracy_multiplier;
}



void UStatsModifier::set_damage_multiplier(const float new_damage_multiplier)
{
    damage_multiplier = new_damage_multiplier;
}


void UStatsModifier::set_speed_multiplier(const float new_speed_multiplier)
{
    speed_multiplier = new_speed_multiplier;
}


void UStatsModifier::set_accuracy_multiplier(const float new_accuracy_multiplier)
{
    accuracy_multiplier = new_accuracy_multiplier;
}