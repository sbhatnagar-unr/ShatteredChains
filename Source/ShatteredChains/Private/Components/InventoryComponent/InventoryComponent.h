// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon      UMETA(DisplayName = "Weapon"),
    Ammunition  UMETA(DisplayName = "Ammunition"),
    Grenade     UMETA(DisplayName = "Grenade"),
    HealthKit   UMETA(DisplayName = "HealthKit"),
    Other       UMETA(DisplayName = "Other")
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxQuantity;

    FInventoryItem()
    {
        ItemID = "";
        ItemType = EItemType::Other;
        Quantity = 0;
        MaxQuantity = 1;
    }

    FInventoryItem(FName NewItemID, EItemType NewItemType, int32 NewQuantity, int32 NewMaxQuantity)
    {
        ItemID = NewItemID;
        ItemType = NewItemType;
        Quantity = NewQuantity;
        MaxQuantity = NewMaxQuantity;
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHATTEREDCHAINS_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Tooltip = "Maximum number of items allowed in inventory"))
    int32 MaxInventorySize = 10;

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Tooltip = "Maximum number of weapons that can be carried"))
    int32 MaxWeapons = 3;

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Tooltip = "Maximum ammunition capacity"))
    int32 MaxAmmunition = 120;

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Tooltip = "Maximum grenades a player can carry"))
    int32 MaxGrenades = 5;

    TArray<FInventoryItem> Items;  // The player's inventory
    TArray<FName> EquippedWeapons; // Equipped weapons

public:
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FName ItemID, EItemType ItemType, int32 Quantity, int32 MaxQuantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool HasItem(FName ItemID, int32 Quantity) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    TArray<FInventoryItem> GetInventory() const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipWeapon(FName WeaponID);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FName GetEquippedWeapon() const;
};
