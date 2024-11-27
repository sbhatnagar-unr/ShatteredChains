// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponents/HealthComponent/HealthComponent.h"
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
    float get_attack_range() const;
    UAnimMontage* get_attack_animation_montage() const;
    float get_attack_animation_exit_blend() const;

    virtual UHealthComponent* get_health_component() const override final;

    virtual void on_death(AActor* killed_by) override;
    
protected:
    // This is the actor that the enemy will chase and attack
    UPROPERTY()
    TObjectPtr<AActor> target;

    // Distance enemy can be from player to hit an attack
    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="Distance enemy can attack player from"))
    float attack_range;

    // Animation montage for attacking player
    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="Animation montage to play when enemy is attacking"))
    TObjectPtr<UAnimMontage> attack_animation_montage;

private:

    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="How long it takes to transition out of the attack montage"))
    float attack_animation_exit_blend;

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* health_component;
};
