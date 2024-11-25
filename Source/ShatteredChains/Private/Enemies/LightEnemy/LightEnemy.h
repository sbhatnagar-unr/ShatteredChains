// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightEnemy();

	AActor* get_target() const;
	float get_attack_range() const;
	float get_attack_damage() const;
	UAnimMontage* get_attack_animation_montage() const;
	float get_attack_animation_exit_blend() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// This is the actor that the enemy will chase and attack
	UPROPERTY()
	TObjectPtr<AActor> target;

	// Distance enemy can be from player to hit an attack
	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	float attack_range;

	// Damage enemy does on each attack
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float attack_damage;

	// Animation montage for attacking player
	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	TObjectPtr<UAnimMontage> attack_animation_montage;

	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	float attack_animation_exit_blend;
};
