// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NamedActor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UNamedActor : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SHATTEREDCHAINS_API INamedActor
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual FString get_actor_name() const;
    virtual void set_actor_name(const FString &new_name);


protected:
    FString actor_name = FString("UNNAMED");
};
