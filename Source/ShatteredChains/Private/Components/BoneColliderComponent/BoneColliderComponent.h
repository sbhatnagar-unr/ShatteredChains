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
    UBoneColliderComponent();

    void hit_bone(FName bone) const;
    
protected:
    // Called when the game starts
    virtual void BeginPlay() override;


private:
    // The mesh that this bone collider is for
    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> skeletal_mesh;
};
