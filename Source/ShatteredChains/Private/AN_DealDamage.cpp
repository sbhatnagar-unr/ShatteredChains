// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DealDamage.h"
#include "ShatteredChains/Logging.h"
#include "ShatteredChains/Utility.h"
#include "LightEnemy.h"

bool UAN_DealDamage::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
{
	
	ALightEnemy* enemy_actor;
	AActor *target_actor;
	try
	{
		MeshComp = Validity::check_value<USkeletalMeshComponent>(MeshComp, "No mesh for animation notify");
		enemy_actor = (ALightEnemy*) Validity::check_value<AActor>(MeshComp->GetOwner(), "Could not get enemy actor from MeshComp");
		target_actor = Validity::check_value<AActor>(enemy_actor->get_target(), "Could not get target actor from enemy");
	}
	catch (const Validity::NullPointerException& e)
	{
		return false;
	}
	return true;
}
