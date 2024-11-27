// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class AWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWeapon();

    void fire() const;
    void reload() const;

    void decrement_mag_ammo_count();
    
    unsigned int get_magazine_size() const;
    unsigned int get_current_magazine_ammo_count() const;

    unsigned int get_max_ammo_stock_pile_count() const;
    unsigned int get_current_ammo_stock_pile_count() const;

    unsigned int get_scan_distance() const;

    float get_weapon_damage() const;

    virtual void refill_magazine();
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USceneComponent> root_scene_component;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USkeletalMeshComponent> weapon_skeletal_mesh_component;

    UPROPERTY()
    TObjectPtr<UAnimInstance> anim_instance;

    UPROPERTY(EditDefaultsOnly, Category="Gun Stats", meta=(ToolTip="How far to check for hitscan"))
    unsigned int scan_distance;

    UPROPERTY(EditDefaultsOnly, Category = "Gun Stats", meta = (ToolTip = "How much damage is done per hit"))
    float weapon_damage;

    UPROPERTY(EditDefaultsOnly, Category="Gun Stats", meta = (ToolTip = "How much ammunition fits in one magazine"))
    unsigned int magazine_size;

    UPROPERTY(EditDefaultsOnly, Category = "Gun Stats", meta = (ToolTip = "How much ammunition is in the stockpile for this weapon (before first magazine is loaded)"))
    unsigned int max_ammo_stock_pile_count;

    unsigned int current_magazine_ammo_count;
    unsigned int current_ammo_stock_pile_count;

    // Animation montages
    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    TObjectPtr<UAnimMontage> fire_animation_montage;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    TObjectPtr<UAnimMontage> reload_animation_montage;
};
