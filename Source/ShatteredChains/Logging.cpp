#include "Logging.h"


DEFINE_LOG_CATEGORY(Enemy)
DEFINE_LOG_CATEGORY(Health)
DEFINE_LOG_CATEGORY(Player)
DEFINE_LOG_CATEGORY(Weapon)
DEFINE_LOG_CATEGORY(UtilityActor)
DEFINE_LOG_CATEGORY(BoneCollision)


std::string basename(const char* file_path)
{
    std::string path(file_path);
    const size_t pos = path.rfind('\\');
    if (pos == std::string::npos)
    {
        return path;
    }

    return path.substr(pos + 1);
}
