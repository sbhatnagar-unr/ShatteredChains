#include "Logging.h"


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
