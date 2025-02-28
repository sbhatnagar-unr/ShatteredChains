#include "Misc/AutomationTest.h"
#include "Weapons/Weapon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(WeaponBaseBasicTest, "ShatteredChains.ShatteredChains.Private.AutomatedTesting.FunctionalTests.Weapons.WeaponBaseBasicTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool WeaponBaseBasicTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.

    AddExpectedMessage(TEXT("Weapon mag ammo decrement triggered but mag ammo count was 0"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 2);
    bool passed = true;

    AWeapon* weapon = NewObject<AWeapon>();

    // Setup weapon for tests
    weapon->set_max_ammo_stock_pile_count(256);
    weapon->set_current_ammo_stock_pile_count(300);
    weapon->set_magazine_size(8);

    if (weapon->get_current_ammo_stock_pile_count() != 256)
    {
        AddError(TEXT("Able to set weapon ammo stock pile larger than max stock pile size.  Aborting rest of test."));
        return false;
    }

    // Here its equal to whatever the max is (it can only get here if the above check passes)
    

    int expected_stock_pile_ammo = weapon->get_current_ammo_stock_pile_count();
    int expected_mag_ammo = weapon->get_current_magazine_ammo_count();

    auto check_values = [this, &weapon, &expected_stock_pile_ammo, &expected_mag_ammo]() -> bool
    {
        if (weapon->get_current_ammo_stock_pile_count() != expected_stock_pile_ammo)
        {
            AddError(FString::Printf(TEXT("Stock pile ammo was %d when expected %d"), weapon->get_current_ammo_stock_pile_count(), expected_stock_pile_ammo));
            return false;
        }

        if (weapon->get_current_magazine_ammo_count() != expected_mag_ammo)
        {
            AddError(FString::Printf(TEXT("Magazine ammo was %d when expected %d"), weapon->get_current_magazine_ammo_count(), expected_mag_ammo));
            return false;
        }

        return true;
    };
    
    
    // Simulate fire without ammo
    AddInfo(TEXT("Testing fire without ammo 1"));
    weapon->decrement_mag_ammo_count();
    expected_mag_ammo = 0;
    passed &= check_values();
    
    // Simulate reload
    AddInfo(TEXT("Testing reload 1"));
    weapon->refill_magazine();
    expected_stock_pile_ammo -= weapon->get_magazine_size() - expected_mag_ammo;
    expected_mag_ammo = weapon->get_magazine_size();
    passed &= check_values();
    
    // Simulate fire
    AddInfo(TEXT("Testing firing weapon 1"));
    weapon->decrement_mag_ammo_count();
    expected_mag_ammo -= 1;
    passed &= check_values();

    // Simulate fire
    AddInfo(TEXT("Testing firing weapon 2"));
    weapon->decrement_mag_ammo_count();
    expected_mag_ammo -= 1;
    passed &= check_values();

    // Simulate fire
    AddInfo(TEXT("Testing firing weapon 3"));
    weapon->decrement_mag_ammo_count();
    expected_mag_ammo -= 1;
    passed &= check_values();


    // Simulate reload
    AddInfo(TEXT("Testing reload 2"));
    expected_stock_pile_ammo -= weapon->get_magazine_size() - expected_mag_ammo;
    expected_mag_ammo = weapon->get_magazine_size();
    weapon->refill_magazine();
    passed &= check_values();


    // Empty out gun
    weapon->set_current_ammo_stock_pile_count(0);
    expected_stock_pile_ammo = 0;
    for (int i = weapon->get_current_magazine_ammo_count(); i > 0; i--)
    {
        AddInfo(FString::Printf(TEXT("Emptying gun (%d) ammo remaining"), weapon->get_current_magazine_ammo_count()));
        weapon->decrement_mag_ammo_count();
        expected_mag_ammo -= 1;
        passed &= check_values();
    }
    
    // Simulate fire without ammo
    AddInfo(TEXT("Testing fire without ammo 2"));
    weapon->decrement_mag_ammo_count();
    expected_mag_ammo = 0;
    passed &= check_values();
    
    // Simulate reloading without ammo
    AddInfo(TEXT("Testing reload without ammo"));
    weapon->refill_magazine();
    expected_mag_ammo = 0;
    passed &= check_values();

    return passed;
}
