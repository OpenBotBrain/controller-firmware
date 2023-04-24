#pragma once

enum class InfoType
{
    HASH = 0,
    BRANCH,
    DEVELOPER,
    DATE,
    VERSION,
};

const char* system_version_get_internal(InfoType type);
void system_version_get_version();
