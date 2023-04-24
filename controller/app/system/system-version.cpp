#include <config/board-rev.hpp>
#include <system/system-version.hpp>

const char* system_version_get_internal(InfoType type)
{
    switch(type)
    {
        case InfoType::HASH:
            return BUILD_GIT_HASH;

        case InfoType::BRANCH:
            return BUILD_BRANCH;

        case InfoType::DEVELOPER:
            return BUILD_DEVELOPER;

        case InfoType::DATE:
            return BUILD_DATE;

        case InfoType::VERSION:
            return BUILD_VERSION;

        default:
            break;
    }
    return "";
}

void system_version_get_version()
{
    // const BoardSpecificConfig* spec_config = board_rev_get_specific_configuration();

    // TODO: ENABLE PRINT TO DEBUG INTERFACE
    // GSDebug("Board Version: %s", spec_config->board_name);
    // GSDebug("Build: %s", BUILD_VERSION);
    // GSDebug("Developer: %s", BUILD_DEVELOPER);
    // GSDebug("Date: %s", BUILD_DATE);
}
