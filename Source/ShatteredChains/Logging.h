#pragma once

// https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine

DECLARE_LOG_CATEGORY_EXTERN(Enemy, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Environment, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Health, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Menu, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Movement, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Weapon, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(GameMode, Log, All);

// Put this line in all the "Upper most" C++ file where you need each category
// Each of these lines can appear AT MOST ONCE across ALL C++ files
// If you need the category in more than one file, than you just include the header, dont put these lines again
// DEFINE_LOG_CATEGORY(Enemy);
// DEFINE_LOG_CATEGORY(Environment);
// DEFINE_LOG_CATEGORY(Health);
// DEFINE_LOG_CATEGORY(Menu);
// DEFINE_LOG_CATEGORY(Movement);
// DEFINE_LOG_CATEGORY(Weapon);
// DEFINE_LOG_CATEGORY(GameMode);