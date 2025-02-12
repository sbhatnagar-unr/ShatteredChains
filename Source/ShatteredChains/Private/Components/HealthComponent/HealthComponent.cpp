// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent/HealthComponent.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Interfaces/NamedActor/NamedActor.h"
#include "ShatteredChains/Logging.h"
#include "Kismet/GameplayStatics.h"

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
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // Get owner
    IHasHealth* owner = GetOwner<IHasHealth>();
    const INamedActor* const owner_na = GetOwner<INamedActor>();
    if (owner_na == nullptr)
    {
        UE_LOG(Health, Error, LOG_TEXT("Health Component owner must inherit from INamedActor, owner of type %s does not"), *(GetOwner()->GetName()));
        return;
    }
    
    const FString owner_name = owner_na->get_default_actor_name();

    // If our owner doesn't have the interface
    // It needs to have the interface so we can attach its on_dead method to the delegate
    if (owner == nullptr)
    {
        UE_LOG(Health, Warning, LOG_TEXT("Health component was added to Actor that does not implement IHasHealth interface (%s)"), *owner_name);
        return;
    }
    // Otherwise attach the delegate
    // Here we already know that owner implements the IHasHealth interface
    on_death_delegate.AddDynamic(owner, &IHasHealth::on_death);
    UE_LOG(Health, Verbose, LOG_TEXT("Added %s on_death delegate"), *owner_name);

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
    
    const INamedActor* const dealt_by_na = Cast<INamedActor>(dealt_by);
    const FString owner_name = dealt_by_na->get_actor_name();

    UGameplayStatics::PlaySound2D(GetWorld(), GetOwner<IHasHealth>()->get_damage_sound(), 1, 1, 0, nullptr, GetOwner(), false);

    if (dealt_by_na == nullptr)
    {
        UE_LOG(Health, Error, LOG_TEXT("Damage dealt_by actor is not an INamedActor (%s)"), *(dealt_by->GetName()));
        dealt_by = nullptr;
    }

    if (dealt_by == nullptr)
    {
        UE_LOG(Health, Error, LOG_TEXT("%s received %f damage from UNKNOWN (%f remaining)"), *owner_name, damage, current_health);
    }
    else
    {
        UE_LOG(Health, Log, LOG_TEXT("%s received %f damage from %s (%f remaining)"), *owner_name, damage, *(dealt_by_na->get_actor_name()), current_health);
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