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
    PrimaryComponentTick.bCanEverTick = true;

    // const INamedActor* const owner_na = Cast<INamedActor>(GetOwner());
    // if (owner_na == nullptr)
    // {
    //     UE_LOG(BoneCollider, Error, LOG_TEXT("Bone collider owner (%s) does not inherit INamedActor"), *(GetOwner()->GetName()));
    //     return;
    // }
    //
    // owner_name = owner_na->get_actor_name();
    //
    // const TObjectPtr<ACharacter> owner = Cast<ACharacter>(GetOwner());
    // if (owner == nullptr)
    // {
    //     UE_LOG(BoneCollider, Error, LOG_TEXT("Bone collider owner (%s) is not a ACharacter"), *owner_name);
    // }
    // skeletal_mesh = owner->GetMesh();
}


// Called when the game starts
void UBoneColliderComponent::BeginPlay()
{
    Super::BeginPlay();
    
}

