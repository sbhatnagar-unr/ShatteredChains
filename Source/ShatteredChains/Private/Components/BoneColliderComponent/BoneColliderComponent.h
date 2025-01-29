// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoneColliderComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHATTEREDCHAINS_API UBoneColliderComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    // Need both for UE5
    UBoneColliderComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;


private:
    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> skeletal_mesh;

    FString owner_name;
};
