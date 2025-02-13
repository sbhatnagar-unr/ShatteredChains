// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public AEnemy
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALightEnemy();

    float get_attack_damage() const;

    float get_attack_range() const;
    UAnimMontage* get_attack_animation_montage() const;
    float get_attack_animation_exit_blend() const;
    
    virtual void on_death(AActor* killed_by) override final;

    bool get_should_chase_target() const;
    void set_should_chase_target(const bool chase_target);

private:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Distance enemy can be from player to hit an attack
    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="Distance enemy can attack player from"))
    float attack_range;

    // Animation montage for attacking player
    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="Animation montage to play when enemy is attacking"))
    TObjectPtr<UAnimMontage> attack_animation_montage;
    
    UPROPERTY(EditDefaultsOnly, Category="Attacking", meta=(ToolTip="How long it takes to transition out of the attack montage"))
    float attack_animation_exit_blend;
    
    UPROPERTY(EditAnywhere, Category="Attacking")
    float attack_damage;

    UFUNCTION()
    void on_see_pawn(APawn* pawn);

    bool should_chase_target;
};
