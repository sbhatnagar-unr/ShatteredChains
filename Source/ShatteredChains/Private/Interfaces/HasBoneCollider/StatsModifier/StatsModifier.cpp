// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsModifier.h"

UStatsModifier::UStatsModifier()
{
    // Default to values that won't modify anything
    additive_damage_modifier = 0;
    multiplicative_damage_modifier = 1;

    additive_speed_modifier = 0;
    multiplicative_speed_modifier = 1;
}



float UStatsModifier::get_additive_damage_modifier() const
{
    return additive_damage_modifier;
}


float UStatsModifier::get_multiplicative_damage_modifier() const
{
    return multiplicative_damage_modifier;
}


void UStatsModifier::set_additive_damage_modifier(const float new_additive_damage_modifier)
{
    additive_damage_modifier = new_additive_damage_modifier;
}


void UStatsModifier::set_multiplicative_damage_modifier(const float new_multiplicative_damage_modifier)
{
    multiplicative_damage_modifier = new_multiplicative_damage_modifier;
}

float UStatsModifier::get_additive_speed_modifier() const
{
    return additive_speed_modifier;
}


float UStatsModifier::get_multiplicative_speed_modifier() const
{
    return multiplicative_speed_modifier;
}


void UStatsModifier::set_additive_speed_modifier(const float new_additive_speed_modifier)
{
    additive_speed_modifier = new_additive_speed_modifier;
}


void UStatsModifier::set_multiplicative_speed_modifier(const float new_multiplicative_speed_modifier)
{
    multiplicative_speed_modifier = new_multiplicative_speed_modifier;
}

