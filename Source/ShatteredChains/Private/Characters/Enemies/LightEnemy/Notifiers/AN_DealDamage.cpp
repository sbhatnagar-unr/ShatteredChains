// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DealDamage.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "../LightEnemy.h"
#include "../../../Player/MyCharacter.h"

void UAN_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// The enemy that's attacking
	ALightEnemy* enemy_actor;
	// The enemy's target
	AMyCharacter* player;
	try
	{
		MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
		enemy_actor = Validity::check_value<ALightEnemy>(Cast<ALightEnemy>(MeshComp->GetOwner()), "Could not get enemy actor from MeshComp");
		player = Validity::check_value<AMyCharacter>(Cast<AMyCharacter>(enemy_actor->get_target()), "Could not get target actor from enemy");
	}
	catch (const Validity::NullPointerException& e)
	{
		UE_LOG(Enemy, Error, LOG_TEXT("%hs"), e.what());
		return;
	}

	// Do damage to the player
	const float attack_damage = enemy_actor->get_attack_damage();
	const float player_health = player->get_health();

	if (player_health > 0)
	{
		UE_LOG(Enemy, Log, LOG_TEXT("Dealing %f damage to player (player has %f health remaining)"), attack_damage, player_health);
		player->deal_damage((AActor*) enemy_actor, attack_damage);
	}
	else
	{
		UE_LOG(Enemy, Log, LOG_TEXT("Player is already dead"));
	}

}
