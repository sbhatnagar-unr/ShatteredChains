// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class APistol : public AWeapon
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

};