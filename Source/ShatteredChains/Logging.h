#pragma once
#include <string>

#define LOG_TEXT(text) TEXT("[%hs:%hs:%d] -> ") TEXT(text), basename(__FILE__).c_str(), __func__, __LINE__
std::string basename(const char* file_path);


// https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine?application_version=5.4

DECLARE_LOG_CATEGORY_EXTERN(Enemy, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Health, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Player, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Weapon, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(UtilityActor, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(BoneCollision, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Inventory, Log, All);
