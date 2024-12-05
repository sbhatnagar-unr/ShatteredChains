// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Make on dead delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, AActor*, killed_by);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UHealthComponent();

    float get_health() const;
    float get_max_health() const;
    virtual void heal(float health);
    virtual void set_health(float health);
    virtual void deal_damage(AActor* dealt_by, float damage);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;


private:
    UPROPERTY()
    FOnDeathDelegate on_death_delegate;
    
    UPROPERTY(EditAnywhere, Category="Health", meta=(ToolTip="Max health the actor can have"))
    float max_health;

    UPROPERTY(EditAnywhere, Category="Health", meta=(ToolTip="Damage received will be multiplied by this value before its applied"))
    float damage_multiplier;
    
    float current_health;
    bool is_dead;

    FString owner_name;
        
};
