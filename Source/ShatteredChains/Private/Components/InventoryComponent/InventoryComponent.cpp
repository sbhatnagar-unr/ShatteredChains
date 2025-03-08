// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ShatteredChains/Logging.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    WeaponSlots.SetNum(3); // Initialize inventory with 3 slots
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UInventoryComponent::AddWeapon(FName WeaponID)
{
    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        if (WeaponSlots[i].IsNone()) // Find first empty slot
        {
            WeaponSlots[i] = WeaponID;
            UE_LOG(LogTemp, Log, TEXT("Added %s to Weapon Slot %d"), *WeaponID.ToString(), i + 1);
            return true;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Weapon inventory full!"));
    return false;
}

const TArray<FName>& UInventoryComponent::GetWeaponSlots() const
{
    return WeaponSlots;
}

void UInventoryComponent::LogInventory() const
{
    UE_LOG(LogTemp, Log, TEXT("Inventory:"));
    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        FString WeaponName = WeaponSlots[i].IsNone() ? TEXT("Empty") : WeaponSlots[i].ToString();
        UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d: %s"), i + 1, *WeaponName);
    }
}

void UInventoryComponent::ToggleInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("ToggleInventory function triggered!"));

    UE_LOG(LogTemp, Log, TEXT("========Inventory Opened========="));

    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        FString WeaponName = WeaponSlots[i].IsNone() ? TEXT("Empty") : WeaponSlots[i].ToString();
        UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d: %s"), i + 1, *WeaponName);
    }
}

// Add an item to the inventory
bool UInventoryComponent::AddItem(const FName &ItemID, const EItemType ItemType, const int32 Quantity, const int32 MaxQuantity)
{
    if (Quantity <= 0) return false;

    int32 ItemCount = 0;
    for (const auto &Item : Items)
    {
        ItemCount += Item.Value.Quantity;
    }

    if (ItemCount + Quantity > MaxInventorySize)
    {
        UE_LOG(Inventory, Warning, TEXT("Inventory is full!"));
        return false;
    }

    // Special case for weapons (max 3)
    if (ItemType == EItemType::Weapon)
    {
        if (EquippedWeapons.Num() >= MaxWeapons)
        {
            UE_LOG(Inventory, Warning, LOG_TEXT("Cannot carry more than %d weapons!"), MaxWeapons);
            return false;
        }
    }

    // Check if item exists
    if (Items.Contains(ItemID))
    {
        FInventoryItem* InventoryItem = &Items[ItemID];
        InventoryItem->Quantity = FMath::Min(InventoryItem->Quantity + Quantity, InventoryItem->MaxQuantity);
    }
    else
    {
        Items.Add(ItemID, FInventoryItem(ItemID, ItemType, Quantity, MaxQuantity));
    }

    return true;
}

// Remove an item from the inventory
bool UInventoryComponent::RemoveItem(const FName &ItemID, const int32 Quantity)
{
    if (Quantity <= 0) return false;

    // If we have the item
    if (Items.Contains(ItemID))
    {
        // Remove the quantity
        FInventoryItem* InventoryItem = &Items[ItemID];
        UE_LOG(Inventory, Log, LOG_TEXT("Removing %d items from %s"), Quantity, *(ItemID.ToString()));
        InventoryItem->Quantity -= Quantity;

        // If we run out, remove the item
        if (InventoryItem->Quantity <= 0)
        {
            UE_LOG(Inventory, Log, LOG_TEXT("Item %s quantity <= 0, removing it from inventory"), *(ItemID.ToString()));
            Items.Remove(ItemID);
        }
        return true;
    }

    // If we dont have the item
    UE_LOG(Inventory, Warning, LOG_TEXT("Inventory does not have %s item, cant remove it."), *(ItemID.ToString()));
    return false;
}

// Check if an item exists in the inventory
bool UInventoryComponent::HasItem(const FName &ItemID, const int32 Quantity) const
{
    // If we dont have the item
    if (!Items.Contains(ItemID)) return false;

    // If we do have it
    return Items[ItemID].Quantity >= Quantity;
}

// Get all inventory items
TMap<FName, FInventoryItem> UInventoryComponent::GetInventory() const
{
    return Items;
}

// Equip a weapon
bool UInventoryComponent::EquipWeapon(const FName &WeaponID)
{
    if (!HasItem(WeaponID, 1)) return false;

    if (EquippedWeapons.Num() >= MaxWeapons)
    {
        UE_LOG(Inventory, Warning, LOG_TEXT("Cannot equip more than %d weapons!"), MaxWeapons);
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
