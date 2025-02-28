#include "Components/InventoryComponent/InventoryComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(InventoryBasicTest, "ShatteredChains.ShatteredChains.Private.AutomatedTesting.FunctionalTests.InventoryComponent.InventoryBasicTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool InventoryBasicTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.

    AddExpectedMessage(TEXT("Inventory is full"), ELogVerbosity::Type::Warning, EAutomationExpectedMessageFlags::Contains, 1);
    AddExpectedMessage(TEXT("Cannot carry more than \\d+ weapons"), ELogVerbosity::Type::Warning, EAutomationExpectedMessageFlags::Contains, 1);

    UInventoryComponent* const inventory_component = NewObject<UInventoryComponent>();
    
    // Helper function
    auto check_quantities = [this, inventory_component](const FName& item, const int item_count, const bool expected) -> bool
    {
        bool passed = true;
        for (int i = 0; i <= item_count; i++)
        {
            // Should always be true
            if (inventory_component->HasItem(item, i) != expected)
            {
                AddError(FString::Printf(TEXT("Item check failed for %s count %d"), *(item.ToString()), i));
                passed = false;
            }
        }

        // Should be false
        if (inventory_component->HasItem(item, item_count+1))
        {
            AddError(FString::Printf(TEXT("Item check failed for %s count %d"), *(item.ToString()), item_count+1));
            passed = false;
        }

        // Should be false
        if (inventory_component->HasItem(item, -2))
        {
            AddError(FString::Printf(TEXT("Item check failed for %s count %d"), *(item.ToString()), -2));
            passed = false;
        }

        return passed;
    };
    
    bool passed = true;

    // Setup
    const FName weapon = FName(TEXT("Weapon"));
    const FName ammo = FName(TEXT("Ammo"));
    const FName grenade = FName(TEXT("Grenade"));
    const FName health = FName(TEXT("Health"));
    const FName other = FName(TEXT("Other"));

    // Only hold 10 items???? not a lot of space, since a single bullet counts as 1 item and no method to set max size
    constexpr int weapon_quantity = 2;
    constexpr int ammo_quantity = 2;
    constexpr int grenade_quantity = 2;
    constexpr int health_quantity = 2;
    constexpr int other_quantity = 2;

    // Add items
    AddInfo(TEXT("TESTING ITEM ADDITION"));
    if (   !inventory_component->AddItem(weapon, EItemType::Weapon, weapon_quantity, 5)
        || !inventory_component->AddItem(ammo, EItemType::Ammunition, ammo_quantity, 5)
        || !inventory_component->AddItem(grenade, EItemType::Grenade, grenade_quantity, 5)
        || !inventory_component->AddItem(health, EItemType::HealthKit, health_quantity, 5)
        || !inventory_component->AddItem(other, EItemType::Other, other_quantity, 5))
    {
        AddError(TEXT("Could not equip items despite having space"));
        passed = false;
    }

    // Check for overflow
    if (inventory_component->AddItem(TEXT("OVERFLOW"), EItemType::Other, 5, 5))
    {
        AddError(TEXT("Able to add items that overflow when this should have not been allowed"));
        passed = false;
    }

    // Try equipping the weapon
    if (!inventory_component->EquipWeapon(weapon))
    {
        AddError(TEXT("Could not equip test weapon"));
        passed = false;
    }
    
    // Check their quantities
    passed &= check_quantities(weapon, weapon_quantity, true);
    passed &= check_quantities(ammo, ammo_quantity, true);
    passed &= check_quantities(grenade, grenade_quantity, true);
    passed &= check_quantities(health, health_quantity, true);
    passed &= check_quantities(other, other_quantity, true);

    // No get item?
    AddWarning(TEXT("No get item method to test"));

    // Remove the items
    AddInfo(TEXT("TESTING ITEM REMOVAL"));
    inventory_component->RemoveItem(weapon, weapon_quantity);
    inventory_component->RemoveItem(ammo, ammo_quantity);
    inventory_component->RemoveItem(grenade, grenade_quantity);
    inventory_component->RemoveItem(health, health_quantity);
    inventory_component->RemoveItem(other, other_quantity);

    // Check quantities again
    passed &= check_quantities(weapon, weapon_quantity, false);
    passed &= check_quantities(ammo, ammo_quantity, false);
    passed &= check_quantities(grenade, grenade_quantity, false);
    passed &= check_quantities(health, health_quantity, false);
    passed &= check_quantities(other, other_quantity, false);


    // Test weapon equipping
    AddInfo(TEXT("TESTING WEAPON EQUIP"));
    const FName weapon1 = FName("Weapon1");
    const FName weapon2 = FName("Weapon2");
    const FName weapon3 = FName("Weapon3");
    const FName overflow_weapon = FName("OVERFLOW WEAPON");
    const FName not_a_weapon = FName("NOT A WEAPON");

    // Test what happens if we try to equip a weapon that doesnt exist
    if (inventory_component->EquipWeapon(TEXT("DOES NOT EXIST")))
    {
        AddError(TEXT("Non-existent weapon equip was successful, should have failed"));
        passed = false;
    }

    // Test getting equipped weapon when there is none
    if (inventory_component->GetEquippedWeapon() != FName(TEXT("")))
    {
        AddError(FString::Printf(TEXT("Equipped weapon should be empty FName when no weapon is equipped, got '%s'"), *(inventory_component->GetEquippedWeapon().ToString())));
        passed = false;
    }

    // Here inventory has been emptied from removal test
    // Add weapons to inventory
    if (   !inventory_component->AddItem(weapon1, EItemType::Weapon, 1, 1)
        || !inventory_component->AddItem(weapon2, EItemType::Weapon, 1, 1)
        || !inventory_component->AddItem(weapon3, EItemType::Weapon, 1, 1))
    {
        AddError(TEXT("Could not add weapon to inventory, despite having space for it"));
        passed = false;
    }

    // Try equipping too many weapons
    if (inventory_component->AddItem(overflow_weapon, EItemType::Weapon, 1, 1))
    {
        AddError("Able to equip more than MaxWeapons weapons");
        passed = false;
    }
    
    // Add an item that's not a weapon
    inventory_component->AddItem(not_a_weapon, EItemType::Other, 1, 1);

    // Try equipping a weapon
    if (!inventory_component->EquipWeapon(weapon2))
    {
        AddError(TEXT("Could not equip any weapon"));
        passed = false;
    }
    // Check if the equipped weapon is the one we just equipped
    if (inventory_component->GetEquippedWeapon() != weapon2)
    {
        AddError(FString::Printf(TEXT("Equipped weapon was incorrect, got '%s' and expected '%s'"), *(inventory_component->GetEquippedWeapon().ToString()), *(weapon2.ToString())));
        passed = false;
    }
    // Can we equip a weapon that's not a weapon?
    if (inventory_component->EquipWeapon(not_a_weapon))
    {
        AddError(TEXT("Able to equip item that's not a weapon as a weapon"));
        passed = false;
    }
    
    return passed;
}