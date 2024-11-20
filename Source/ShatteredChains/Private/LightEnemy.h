// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CharacterWithHealth.h"
#include "Animation/AnimMontage.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public ACharacterWithHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightEnemy();

	AActor* get_target();
	float get_attack_range();
	UAnimMontage* get_attack_animation_montage();
	float get_attack_animation_exit_blend();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// This is the actor that the enemy whill chase and attack
	TObjectPtr<AActor> target;

	// Distance enemy can be from player to hit an attack
	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	float attack_range;

	// Animation montage for attacking player
	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	TObjectPtr<UAnimMontage> attack_animation_montage;
	
	UPROPERTY(EditDefaultsOnly, Category="Attacking")
	float attack_animation_exit_blend;
	
	void dead() override;
};
