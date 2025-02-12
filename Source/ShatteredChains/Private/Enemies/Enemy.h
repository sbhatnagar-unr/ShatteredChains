// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent/HealthComponent.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Interfaces/NamedActor/NamedActor.h"
#include "Interfaces/HasBoneCollider/HasBoneCollider.h"
#include "Interfaces/HasBoneCollider/StatsModifier/StatsModifier.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy : public ACharacter, public IHasHealth, public INamedActor, public IHasBoneCollider
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();
    
    AActor* get_target() const;


    virtual void BeginPlay() override;

    // IHasHealth functions
    virtual UHealthComponent* get_health_component() const override final;
    virtual void on_death(AActor* killed_by) override;

    // INamedActor functions
    virtual FString get_default_actor_name() const override final;

    // IHasBoneCollider functions
    virtual const TMap<FName, TObjectPtr<UStatsModifier>>* get_bone_collider_stats_modifiers() const override final;
    virtual void hit_bone(const FName bone_name) override;

    virtual USoundBase* get_damage_sound() const override final;

    
protected:
    // This is the actor that the enemy will attack
    UPROPERTY(EditAnywhere)
    TObjectPtr<AActor> target;

    // Bone collision stats modifiers
    UPROPERTY()
    TMap<FName, TObjectPtr<UStatsModifier>> stats_modifiers;

    // These are general values, the subclass should apply these to the proper UStatsModifiers as needed
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a leg shot is hit (+ increases and - decreases)"))
    float leg_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a leg shot is hit"))
    float leg_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a leg shot is hit (+ increases and - decreases)"))
    float leg_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a leg shot is hit"))
    float leg_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a arm shot is hit (+ increases and - decreases)"))
    float arm_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a arm shot is hit"))
    float arm_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a arm shot is hit (+ increases and - decreases)"))
    float arm_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a arm shot is hit"))
    float arm_shot_speed_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a hand shot is hit (+ increases and - decreases)"))
    float hand_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a hand shot is hit"))
    float hand_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a hand shot is hit (+ increases and - decreases)"))
    float hand_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a hand shot is hit"))
    float hand_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a torso shot is hit (+ increases and - decreases)"))
    float torso_shot_damage_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a torso shot is hit"))
    float torso_shot_damage_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a torso shot is hit (+ increases and - decreases)"))
    float torso_shot_speed_adder;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a torso shot is hit"))
    float torso_shot_speed_multiplier;
    
    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional damage added whenever a head shot is hit (+ increases and - decreases)"))
    float head_shot_damage_adder;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for damage whenever a head shot is hit"))
    float head_shot_damage_multiplier;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Additional speed added whenever a head shot is hit (+ increases and - decreases)"))
    float head_shot_speed_adder;

    UPROPERTY(EditAnywhere, Category="StatsModifiers", meta=(Tooltip = "Multiplier for speed whenever a head shot is hit"))
    float head_shot_speed_multiplier;

    
private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UHealthComponent> health_component;

    UPROPERTY(EditInstanceOnly)
    FString default_actor_name;

    UPROPERTY(EditDefaultsOnly, Category="Sound Effects")
    TObjectPtr<USoundBase> take_damage_sound;
};
