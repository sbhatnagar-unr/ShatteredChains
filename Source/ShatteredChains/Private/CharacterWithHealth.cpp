// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWithHealth.h"


float ACharacterWithHealth::get_health() const
{
	return current_health;
}


float ACharacterWithHealth::get_max_health() const
{
	return max_health;
}


void ACharacterWithHealth::heal(float health)
{
	current_health += health;
	if (current_health > max_health)
	{
	}
}


void ACharacterWithHealth::set_health(float health)
{

}


void ACharacterWithHealth::deal_damage(float damage)
{

}