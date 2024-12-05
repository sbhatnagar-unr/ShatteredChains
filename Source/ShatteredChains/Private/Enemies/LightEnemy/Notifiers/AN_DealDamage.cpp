// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DealDamage.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "Enemies/LightEnemy/LightEnemy.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"

void UAN_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    // The enemy that's attacking
    ALightEnemy* enemy_actor;
    // The targets health component
    UHealthComponent* target_health_component;
    try
    {
        MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
        enemy_actor = Validity::check_value<ALightEnemy>(Cast<ALightEnemy>(MeshComp->GetOwner()), "Could not get enemy actor from MeshComp");
        const IHasHealth* player = Validity::check_value<IHasHealth>(Cast<IHasHealth>(enemy_actor->get_target()), "Could not get target actor (as IHasHealth) from enemy");
        target_health_component = Validity::check_value<UHealthComponent>(player->get_health_component(), "Could not get health component from target");
        
    }
    catch (const Validity::NullPointerException& e)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
        return;
    }

    // Do damage to the player
    const float attack_damage = enemy_actor->get_attack_damage();
    const float player_health = target_health_component->get_health();

    if (player_health > 0)
    {
        UE_LOG(Enemy, Log, LOG_TEXT("Dealing %f damage to player (player has %f health remaining)"), attack_damage, player_health);
        target_health_component->deal_damage(Cast<AActor>(enemy_actor), attack_damage);
    }
    else
    {
        UE_LOG(Enemy, Log, LOG_TEXT("Player is already dead"));
    }

}
