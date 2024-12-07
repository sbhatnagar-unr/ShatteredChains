// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DealDamage.h"
#include "ShatteredChains/Logging.h"
#include "Enemies/LightEnemy/LightEnemy.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"

void UAN_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("No mesh for animation notify"));
        return;
    }
    
    // The enemy that's attacking
    ALightEnemy* enemy_actor = Cast<ALightEnemy>(MeshComp->GetOwner());
    if (enemy_actor == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get enemy actor from MeshComp"));
        return;
    }

    const FString enemy_actor_name = (enemy_actor->Tags.Num() > 0) ? enemy_actor->Tags[0].ToString() : FString(TEXT("UNTAGGED"));

    const IHasHealth* player = Cast<IHasHealth>(enemy_actor->get_target());
    if (player == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get target actor (as IHasHealth) from enemy %s"), *enemy_actor_name);
        return;
    }
    
    // The targets health component
    UHealthComponent* target_health_component = player->get_health_component();
    if (target_health_component == nullptr)
    {
        UE_LOG(Enemy, Error, LOG_TEXT("Could not get health component from enemy %s's target"), *enemy_actor_name);
        return;
    }


    // Do damage to the player
    const float attack_damage = enemy_actor->get_attack_damage();
    const float player_health = target_health_component->get_health();

    if (player_health > 0)
    {
        UE_LOG(Enemy, Log, LOG_TEXT("Enemy %s dealing %f damage to player (player has %f health remaining)"), *enemy_actor_name, attack_damage, player_health);
        target_health_component->deal_damage(Cast<AActor>(enemy_actor), attack_damage);
    }
    else
    {
        UE_LOG(Enemy, Log, LOG_TEXT("Player is already dead"));
    }

}
