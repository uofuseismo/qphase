#include <string>
#include "qphase/version.hpp"

using namespace QPhase;

int Version::getMajor() noexcept
{
    return QPHASE_MAJOR;
}

int Version::getMinor() noexcept
{
    return QPHASE_MINOR;
}

int Version::getPatch() noexcept
{
    return QPHASE_PATCH;
}

bool Version::isAtLeast(const int major, const int minor,
                        const int patch) noexcept
{
    if (QPHASE_MAJOR < major){return false;}
    if (QPHASE_MAJOR > major){return true;}
    if (QPHASE_MINOR < minor){return false;}
    if (QPHASE_MINOR > minor){return true;}
    if (QPHASE_PATCH < patch){return false;}
    return true;
}

std::string Version::getVersion() noexcept
{
    std::string version(QPHASE_VERSION);
    return version;
}
