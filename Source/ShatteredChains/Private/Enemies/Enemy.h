// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent/HealthComponent.h"
#include "Components/BoneColliderComponent/BoneColliderComponent.h"
#include "Components/BoneColliderComponent/StatsModifier/StatsModifier.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "Interfaces/NamedActor/NamedActor.h"
#include "Interfaces/HasBoneCollider/HasBoneCollider.h"
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
    virtual TObjectPtr<UBoneColliderComponent> get_bone_collider_component() const override final;
    
protected:
    // This is the actor that the enemy will attack
    UPROPERTY(EditAnywhere)
    TObjectPtr<AActor> target;

private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UHealthComponent> health_component;

    UPROPERTY()
    TObjectPtr<UBoneColliderComponent> bone_collider_component;

    UPROPERTY(EditInstanceOnly)
    FString default_actor_name;

    // Bone collision stats modifiers
    UPROPERTY()
    TMap<FName, TObjectPtr<UStatsModifier>> stats_modifiers;
};
