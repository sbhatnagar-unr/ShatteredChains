// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsModifier.h"
UStatsModifier::UStatsModifier()
{
    // Default to values that won't modify anything
    additive_health_modifier = 0;
    multiplicative_health_modifier = 1;
}



float UStatsModifier::get_additive_health_modifier() const
{
    return additive_health_modifier;
}


float UStatsModifier::get_multiplicative_health_modifier() const
{
    return multiplicative_health_modifier;
}


void UStatsModifier::set_additive_health_modifier(const float new_additive_health_modifier)
{
    additive_health_modifier = new_additive_health_modifier;
}


void UStatsModifier::set_multiplicative_health_modifier(const float new_multiplicative_health_modifier)
{
    multiplicative_health_modifier = new_multiplicative_health_modifier;
}