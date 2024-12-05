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

    max_health = 0.f;
    current_health = 0.f;
    damage_multiplier = 1.f;
    is_dead = false;

    owner_name = FString("UNKNOWN");
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // Get owner
    IHasHealth* owner = GetOwner<IHasHealth>();
    
    // Set owner name instance variable
    owner_name = GetOwner()->GetActorLabel();
    
    // If our owner doesn't have the interface
    // It needs to have the interface so we can attach its on_dead method to the delegate
    if (owner == nullptr)
    {
        UE_LOG(Health, Warning, LOG_TEXT("Health component was added to Actor that does not implement IHasHealth interface (%s)"), *owner_name);
        return;
    }
    // Otherwise attach the delegate
    else
    {
        // Here we already know that owner implements the IHasHealth interface
        on_death_delegate.AddDynamic(owner, &IHasHealth::on_death);
        UE_LOG(Health, Verbose, LOG_TEXT("Added %s on_death delegate"), *owner_name);
    }

    // If the max_health is 0 (unset)
    if (max_health <= 0.f)
    {
        UE_LOG(Health, Warning, TEXT("Max health for %s was never set"), *owner_name);
    }
    
    current_health = max_health;
    is_dead = false;
    
    
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

    current_health -= damage * damage_multiplier;

    if (dealt_by == nullptr)
    {
        UE_LOG(Health, Log, LOG_TEXT("%s received %f damage from UNKNOWN (%f remaining)"), *owner_name, damage, current_health);
        UE_LOG(Health, Warning, LOG_TEXT("No dealt_by AActor for damage applied to %s (%f remaining)"), *owner_name, current_health);
    }
    else
    {
        UE_LOG(Health, Log, LOG_TEXT("%s received %f damage from %s (%f remaining)"), *owner_name, damage, *dealt_by->GetActorLabel(), current_health);
    }

    
    if (current_health <= 0 && !is_dead)
    {
        UE_LOG(Health, Log, LOG_TEXT("%s is dead"), *owner_name);
        current_health = 0;
        is_dead = true;

        if (on_death_delegate.IsBound())
        {
            UE_LOG(Health, VeryVerbose, LOG_TEXT("Broadcasting death delegate for %s"), *owner_name);
            on_death_delegate.Broadcast(dealt_by);
        }
        else
        {
            UE_LOG(Health, Warning, LOG_TEXT("No functions bound to death delegate for %s"), *owner_name)
        }
    }
}