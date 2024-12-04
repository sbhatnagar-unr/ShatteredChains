// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Weapons/Weapon.h"
#include "UtilityActors/AnchorPoint/AnchorPoint.h"
#include "Interfaces/WeaponUser/WeaponUser.h"
#include "RangedEnemy.generated.h"

UCLASS()
class SHATTEREDCHAINS_API ARangedEnemy : public AEnemy, public IWeaponUser
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ARangedEnemy();

    AWeapon* get_weapon() const;

    AAnchorPoint* get_anchor_point() const;

    FVector get_location_to_go_to() const;

    float get_anchor_tolerance() const;

    bool is_location_to_go_to_set() const;
    
    // Weapon user functions
    virtual FVector get_hitscan_start_location() const override final;
    virtual FVector get_hitscan_direction() const override final;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category="Weapon", meta=(ToolTip="What weapon the enemy is using"))
    TSubclassOf<AWeapon> weapon_class;

    UPROPERTY(EditAnywhere, Category="Weapon", meta=(ToolTip="How accurate the enemy can shoot (0-100]"))
    float accuracy;

    UPROPERTY()
    TObjectPtr<AWeapon> weapon;
    
    UPROPERTY(EditInstanceOnly, Category="Anchor", meta=(ToolTip="Reference to an achor point instance"))
    TObjectPtr<AAnchorPoint> anchor_point;

    UPROPERTY(EditAnywhere, Category="Anchor", meta=(ToolTip="Tolerance for enemy to be considred \"in place\" when it reaches its location inside the anchor point"))
    float anchor_tolerance;
    
    bool location_to_go_to_set;
    FVector location_to_go_to;
};
