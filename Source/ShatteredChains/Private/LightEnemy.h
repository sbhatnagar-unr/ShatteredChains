// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Root component
	USkeletalMeshComponent* skeletal_mesh_component;

	// Asset paths
	const FString skeletal_mesh_asset_path = FString(TEXT("/Game/Enemies/LightEnemy/Models/motion-dummy_female.motion-dummy_female"));
	const FString animation_blueprint_class_path = FString(TEXT("/Game/Enemies/LightEnemy/Animations/ABP_Walking.ABP_Walking_C"));
	const FString ai_controller_class_path = FString(TEXT("/Game/Enemies/LightEnemy/AI/AIC_ChasePlayer.AIC_ChasePlayer_C"));

	bool issue_with_asset = false;
	template <typename T> T* load_object(const FString& object_asset_path);
	template <typename T> TSubclassOf<T> load_class(const FString& class_asset_path);
};