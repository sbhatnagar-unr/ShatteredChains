// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Interfaces/NamedActor/NamedActor.h"
#include "Weapon.generated.h"

UCLASS()
class AWeapon : public AActor, public INamedActor
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

    void set_magazine_size(const unsigned int new_magazine_size);
    void set_max_ammo_stock_pile_count(const unsigned int new_max_ammo_stock_pile_count);
    void set_current_ammo_stock_pile_count(const unsigned int new_current_ammo_stock_pile_count);

    virtual FString get_default_actor_name() const override final;

    bool is_full_auto() const;

    // zoom getter
    float GetZoomFOV() const { return ZoomFOV; }
    
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

    // Interactable collision sphere
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* InteractionSphere;

    // Overlap Event
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Animation montages
    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    TObjectPtr<UAnimMontage> fire_animation_montage;
    bool has_fire_animation_montage;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    TObjectPtr<UAnimMontage> reload_animation_montage;
    bool has_reload_animation_montage;

    UPROPERTY(EditDefaultsOnly, Category="Gun Stats")
    bool full_auto;

    // Custom Zoom Level for this weapon
    UPROPERTY(EditDefaultsOnly, Category = "Scope")
    float ZoomFOV = 90.0f; // Default (no zoom) — override in BP_Pistol, BP_Rifle, BP_Sniper

private:
    UPROPERTY(EditInstanceOnly)
    FString default_actor_name;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> shoot_sound;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> reload_sound;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> out_of_ammo_sound;
};
