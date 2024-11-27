#pragma once
#include <string>

#define LOG_TEXT(text) TEXT("[%hs:%d] -> ") TEXT(text), basename(__FILE__).c_str(), __LINE__
std::string basename(const char* file_path);


// https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine?application_version=5.4

DECLARE_LOG_CATEGORY_EXTERN(Enemy, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Health, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Player, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Weapons, Log, All);

// Put this line in all the "Upper most" C++ file where you need each category
// Each of these lines can appear AT MOST ONCE across ALL C++ files
// If you need the category in more than one file, than you just include the header, dont put these lines again
// DEFINE_LOG_CATEGORY(Enemy);
// DEFINE_LOG_CATEGORY(Health);
