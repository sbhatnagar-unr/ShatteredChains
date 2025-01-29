// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"
#include "Components/ActorComponents/BoneColliderComponent/BoneColliderComponent.h"
#include "Interfaces/HasHealth/HasHealth.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy : public ACharacter, public IHasHealth
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    AActor* get_target() const;

    virtual UHealthComponent* get_health_component() const override final;

    virtual void BeginPlay() override;
    
    virtual void on_death(AActor* killed_by) override;

    FString get_name() const;
    
protected:
    // This is the actor that the enemy will attack
    UPROPERTY(EditAnywhere)
    TObjectPtr<AActor> target;

    FString actor_name;

private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UHealthComponent> health_component;

    UPROPERTY()
    TObjectPtr<UBoneColliderComponent> bone_collider_component;

};
