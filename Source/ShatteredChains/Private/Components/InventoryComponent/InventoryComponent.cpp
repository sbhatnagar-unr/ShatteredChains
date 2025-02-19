// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Add an item to the inventory
bool UInventoryComponent::AddItem(FName ItemID, EItemType ItemType, int32 Quantity, int32 MaxQuantity)
{
    if (Quantity <= 0) return false;

    int32 ItemCount = 0;
    for (const FInventoryItem& Item : Items)
    {
        ItemCount += Item.Quantity;
    }

    if (ItemCount + Quantity > MaxInventorySize)
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
        return false;
    }

    // Special case for weapons (max 3)
    if (ItemType == EItemType::Weapon)
    {
        if (EquippedWeapons.Num() >= MaxWeapons)
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot carry more than %d weapons!"), MaxWeapons);
            return false;
        }
    }

    // Check if item exists
    for (FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            int32 NewQuantity = Item.Quantity + Quantity;
            Item.Quantity = FMath::Min(NewQuantity, Item.MaxQuantity);
            return true;
        }
    }

    Items.Add(FInventoryItem(ItemID, ItemType, Quantity, MaxQuantity));
    return true;
}

// Remove an item from the inventory
bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    if (Quantity <= 0) return false;

    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].ItemID == ItemID)
        {
            if (Items[i].Quantity >= Quantity)
            {
                Items[i].Quantity -= Quantity;
                if (Items[i].Quantity == 0)
                {
                    Items.RemoveAt(i);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

// Check if an item exists in the inventory
bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID && Item.Quantity >= Quantity)
        {
            return true;
        }
    }
    return false;
}

// Get all inventory items
TArray<FInventoryItem> UInventoryComponent::GetInventory() const
{
    return Items;
}

// Equip a weapon
bool UInventoryComponent::EquipWeapon(FName WeaponID)
{
    if (!HasItem(WeaponID, 1)) return false;

    if (EquippedWeapons.Num() >= MaxWeapons)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot equip more than %d weapons!"), MaxWeapons);
        return false;
    }

    EquippedWeapons.Add(WeaponID);
    return true;
}

// Get the currently equipped weapon (first equipped weapon)
FName UInventoryComponent::GetEquippedWeapon() const
{
    return EquippedWeapons.Num() > 0 ? EquippedWeapons[0] : FName("");
}
