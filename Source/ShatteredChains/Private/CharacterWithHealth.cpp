// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWithHealth.h"
#include "ShatteredChains/Logging.h"

 DEFINE_LOG_CATEGORY(Health);
 

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
	/*
	Heals charachter up to a maxiumim of max_health
	*/
	current_health += health;
	if (current_health > max_health)
	{
		UE_LOG(Health, Log, TEXT("Overhealed, resetting health to max"));
		current_health = max_health;
	}
}


void ACharacterWithHealth::set_health(float health)
{
	/*
	Sets health up to a maximum of max_health
	*/
	current_health = (health > max_health) ? max_health : health;
}


void ACharacterWithHealth::deal_damage(AActor* dealt_by, float damage)
{
	/*
	Applies damage, if health becomes negative, it gets set to 0
	*/
	current_health -= damage;

	if (current_health <= 0)
	{
		current_health = 0;
		dead(dealt_by);
		return;
	}
}