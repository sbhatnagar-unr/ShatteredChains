// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponUser.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWeaponUser : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SHATTEREDCHAINS_API IWeaponUser
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // This should return a location to start a hitscan from
    virtual FVector get_hitscan_start_location() const PURE_VIRTUAL(IWeaponUser::get_hitscan_start_location, return FVector(0,0,0););

    // This should return a Vector in the direction to fire
    virtual FVector get_hitscan_direction() const PURE_VIRTUAL(IWeaponUser::get_hitscan_start_location, return FVector(0,0,0););
};
