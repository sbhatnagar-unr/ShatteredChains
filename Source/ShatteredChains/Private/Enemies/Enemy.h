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
    virtual void hit_bone(FName bone_name) override;

    
protected:
    // This is the actor that the enemy will attack
    UPROPERTY(EditAnywhere)
    TObjectPtr<AActor> target;

    // Bone collision stats modifiers
    UPROPERTY()
    TMap<FName, TObjectPtr<UStatsModifier>> stats_modifiers;
    
private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UHealthComponent> health_component;

    UPROPERTY(EditInstanceOnly)
    FString default_actor_name;
};
