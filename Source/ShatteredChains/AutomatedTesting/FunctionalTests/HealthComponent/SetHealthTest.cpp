#include "Misc/AutomationTest.h"
#include "ShatteredChains/Logging.h"
#include "Components/HealthComponent/HealthComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SetHealthTest, "ShatteredChains.ShatteredChains.AutomatedTesting.FunctionalTests.HealthComponent.SetHealthTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SetHealthTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    UHealthComponent* const test_health_component = NewObject<UHealthComponent>();
    test_health_component->set_health(100.f);
    
    constexpr float expected_health = 100.f;
    const float health = test_health_component->get_health();
    if (health != expected_health)
    {
        AddError(FString::Printf(LOG_TEXT("Expected health was %f but actual health was %f"), expected_health, health));
        return false;
    }
    return true;
}
