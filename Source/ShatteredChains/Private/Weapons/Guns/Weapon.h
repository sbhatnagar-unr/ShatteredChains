// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	void fire() const;
	void reload() const;

	unsigned int get_magazine_size() const;
	unsigned int get_current_magazine_ammo_count() const;

	unsigned int get_max_ammo_stock_pile_count() const;
	unsigned int get_current_ammo_stock_pile_count() const;

	uint32 get_scan_distance() const;

	virtual void refill_magazine();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> weapon_skeletal_mesh_component;

	UPROPERTY(EditDefaultsOnly, Category="Hit-Scan")
	uint32 scan_distance;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	unsigned int magazine_size;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	unsigned int max_ammo_stock_pile_count;

	unsigned int current_magazine_ammo_count;
	unsigned int current_ammo_stock_pile_count;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> fire_animation_montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> reload_animation_montage;
};
