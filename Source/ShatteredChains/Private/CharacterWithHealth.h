// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterWithHealth.generated.h"

UCLASS(Abstract)
class ACharacterWithHealth : public ACharacter
{
	GENERATED_BODY()

public:
	float get_health() const;
	float get_max_health() const;
	virtual void heal(float health);
	virtual void set_health(float health);
	virtual void deal_damage(float damage);
	virtual void dead() PURE_VIRTUAL(ACharacterWithHealth::dead, return;);


private:
	UPROPERTY(EditDefaultsOnly)
	float max_health;

	float current_health;
};
