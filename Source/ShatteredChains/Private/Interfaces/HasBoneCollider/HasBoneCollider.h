// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsModifier/StatsModifier.h"
#include "UObject/Interface.h"
#include "HasBoneCollider.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UHasBoneCollider : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SHATTEREDCHAINS_API IHasBoneCollider
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual const TMap<FName, TObjectPtr<UStatsModifier>>* get_bone_collider_stats_modifiers() const PURE_VIRTUAL(IHasBoneCollider::get_bone_collider_stats_modifiers, return nullptr;);
};
