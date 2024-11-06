// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightEnemy.generated.h"

UCLASS()
class ALightEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* skeletal_mesh_component;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FString skeletal_mesh_asset_path = FString(TEXT("/Game/Enemies/LightEnemy/motion-dummy_female.motion-dummy_female"));
	FString animation_blueprint_class_path = FString(TEXT("/Game/Enemies/LightEnemy/ABP_Walking.ABP_Walking_C"));
	
};