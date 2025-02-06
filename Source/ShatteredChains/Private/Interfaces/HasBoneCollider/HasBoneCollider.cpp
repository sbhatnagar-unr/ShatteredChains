// Fill out your copyright notice in the Description page of Project Settings.


#include "HasBoneCollider.h"
#include "PhysicsEngine/PhysicsAsset.h"


// Add default functionality here for any IHasBoneCollider functions that are not pure virtual.
void IHasBoneCollider::add_stats_modifiers(UPhysicsAsset* physics_asset, TMap<FName, TObjectPtr<UStatsModifier>> *stats_modifiers)
{
    for (int32 i = 0; i < physics_asset->SkeletalBodySetups.Num(); i++)
    {
        stats_modifiers->Add(physics_asset->SkeletalBodySetups[i]->BoneName, NewObject<UStatsModifier>());
    }
}