#include "Misc/AutomationTest.h"
#include "Components/HealthComponent/HealthComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(HealthBasicTest, "ShatteredChains.ShatteredChains.Private.AutomatedTesting.FunctionalTests.HealthComponent.HealthBasicTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool HealthBasicTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    UHealthComponent* const test_health_component = NewObject<UHealthComponent>();

    AddExpectedError(TEXT("Health component has no owner"), EAutomationExpectedErrorFlags::Contains, 3);
    AddExpectedMessage(TEXT("Can't set negative health"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 1);
    AddExpectedMessage(TEXT("Can't heal negative health"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 1);
    AddExpectedMessage(TEXT("Can't deal negative damage"), ELogVerbosity::Warning, EAutomationExpectedMessageFlags::Contains, 1);

    // Helper function
    auto check_health = [this, &test_health_component](const float expected_health) -> bool
    {
        if (test_health_component->get_health() != expected_health)
        {
            AddError(FString::Printf(TEXT("Expected health was %f but actual health was %f"), expected_health, test_health_component->get_health()));
            return false;
        }
        return true;
    };
    

    bool passed = true;
    

    AddInfo(TEXT("TESTING SET HEALTH"));
    // Set health
    float expected_health = 100.f;
    test_health_component->set_health(100.f);
    passed &= check_health(expected_health);

    // expected_health doesn't change, should not heal
    test_health_component->set_health(-100.f);
    passed &= check_health(expected_health);


    AddInfo(TEXT("TESTING DAMAGE"));
    // Do damage
    float damage = 10.f;
    expected_health -= damage;
    test_health_component->deal_damage(nullptr, damage);
    passed &= check_health(expected_health);

    // Do damage
    damage = -10.f;
    // expected_health doesn't change, should do no damage
    test_health_component->deal_damage(nullptr, damage);
    passed &= check_health(expected_health);


    AddInfo(TEXT("TESTING HEAL"));
    // Heal
    float heal_amount = 5.f;
    expected_health += heal_amount;
    test_health_component->heal(heal_amount);
    passed &= check_health(expected_health);

    heal_amount = -5.f;
    // expected_health doesn't change, should not heal
    test_health_component->heal(heal_amount);
    passed &= check_health(expected_health);

    

    AddInfo(TEXT("TESTING DEATH"));
    expected_health = 0;
    // Deal damage
    test_health_component->deal_damage(nullptr, 10000000);
    passed &= check_health(expected_health);


    // Deal damage to dead component again
    test_health_component->deal_damage(nullptr, 10000000);
    passed &= check_health(expected_health);



    return passed;
}
