// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/InventoryComponent/InventoryComponent.h"
#include "HasInventory.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHasInventory : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SHATTEREDCHAINS_API IHasInventory
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual TObjectPtr<UInventoryComponent> get_inventory_component() const PURE_VIRTUAL(IHasInventory::get_inventory_component, return nullptr;);

};
