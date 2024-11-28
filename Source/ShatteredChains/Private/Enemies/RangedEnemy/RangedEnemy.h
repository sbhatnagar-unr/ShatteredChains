// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Weapons/Weapon.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "RangedEnemy.generated.h"

UCLASS()
class SHATTEREDCHAINS_API ARangedEnemy : public AEnemy
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ARangedEnemy();

    AWeapon* get_weapon() const;

    AAnchorPoint* get_anchor_point() const;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TSubclassOf<AWeapon> weapon_class;

    UPROPERTY()
    TObjectPtr<AWeapon> weapon;
    
    UPROPERTY(EditInstanceOnly, Category="Anchor")
    TObjectPtr<AAnchorPoint> anchor_point;
};
