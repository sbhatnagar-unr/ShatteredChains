// Fill out your copyright notice in the Description page of Project Settings.


#include "BoneColliderComponent.h"
#include "Interfaces/NamedActor/NamedActor.h"
#include "GameFramework/Character.h"

#include "ShatteredChains/Logging.h"

DEFINE_LOG_CATEGORY(BoneCollider);


// Sets default values for this component's properties
UBoneColliderComponent::UBoneColliderComponent()
{
    // Dont tick for performance reasons
    PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UBoneColliderComponent::BeginPlay()
{
    Super::BeginPlay();

    const INamedActor* const owner_na = Cast<INamedActor>(GetOwner());
    if (owner_na == nullptr)
    {
        UE_LOG(BoneCollider, Error, LOG_TEXT("Bone collider owner (%s) does not inherit INamedActor"), *(GetOwner()->GetName()));
        return;
    }
    
    const TObjectPtr<ACharacter> owner = Cast<ACharacter>(GetOwner());
    if (owner == nullptr)
    {
        UE_LOG(BoneCollider, Error, LOG_TEXT("Bone collider owner (%s) is not a ACharacter"), *(owner_na->get_default_actor_name()));
        return;
    }
    skeletal_mesh = owner->GetMesh();
    UE_LOG(BoneCollider, Log, LOG_TEXT("Bone collider added to '%s'"), *(owner_na->get_default_actor_name()));
}


void UBoneColliderComponent::hit_bone(const FName bone) const
{
    UE_LOG(BoneCollider, Log, LOG_TEXT("Bone '%s' on actor '%s' was hit"), *(bone.ToString()), *(GetOwner<INamedActor>()->get_actor_name()));
}

