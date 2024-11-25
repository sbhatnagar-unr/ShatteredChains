// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorComponents/HealthComponent/HealthComponent.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(Health);

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	max_health = 0;
	current_health = 0;
	is_dead = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	current_health = max_health;
	is_dead = false;

	// Attach dead delegate to owner dead function
	IHasHealth* owner = GetOwner<IHasHealth>();
	const FString owner_name = GetOwner()->GetName();
	// If our owner doesn't have the interface
	// It needs to have the interface so we can attach its on_dead method to the delegate
	if (owner == nullptr)
	{
		UE_LOG(Health, Error, LOG_TEXT("Health component was added to Actor that does not implement IHasHealth interface (%s)"), *owner_name);
	}
	// Otherwise attach the delegate
	else
	{
		// Here we already know that owner implements the IHasHealth interface
		on_death_delegate.AddDynamic(owner, &IHasHealth::on_death);
		UE_LOG(Health, Verbose, LOG_TEXT("Added %s on_death delegate"), *owner_name);
	}

	
}


float UHealthComponent::get_health() const
{
	return current_health;
}


float UHealthComponent::get_max_health() const
{
	return max_health;
}


void UHealthComponent::heal(const float health)
{
	/*
	Heals character up to a maximum of max_health
	*/
	current_health += health;
	if (current_health > max_health)
	{
		UE_LOG(Health, Log, TEXT("Over-healed, resetting health to max"));
		current_health = max_health;
	}
}


void UHealthComponent::set_health(const float health)
{
	/*
	Sets health up to a maximum of max_health
	*/
	current_health = (health > max_health) ? max_health : health;
}


void UHealthComponent::deal_damage(AActor* dealt_by, const float damage)
{
	/*
	Applies damage, if health becomes negative, it gets set to 0
	*/
	current_health -= damage;

	if (current_health <= 0 && !is_dead)
	{
		current_health = 0;
		is_dead = true;
		UE_LOG(Health, VeryVerbose, LOG_TEXT("Broadcasting death delegate for %s"), *GetOwner()->GetName());
		on_death_delegate.Broadcast(dealt_by);
		return;
	}
}