// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "LightEnemy.generated.h"


UCLASS()
class ALightEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Root component
	USkeletalMeshComponent* skeletal_mesh_component;

	// Asset paths
	FString skeletal_mesh_asset_path = FString(TEXT("/Game/Enemies/LightEnemy/motion-dummy_female.motion-dummy_female"));
	FString animation_blueprint_class_path = FString(TEXT("/Game/Enemies/LightEnemy/ABP_Walking.ABP_Walking_C"));

	// Player tracking
	AAIController* ai_controller;
	AActor* target_actor;
	void go_to_location(const FVector &location);
};