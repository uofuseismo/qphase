#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <limits>
#ifndef NDEBUG
#include <cassert>
#endif
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/arrival.hpp"

using namespace QPhase::Database::Internal;

namespace
{
[[nodiscard]] double shiftLongitude(const double lonIn)
{
    double lon = lonIn;
    if (lon < -180)
    {
        for (int k = 0; k < std::numeric_limits<int>::max(); ++k)
        {
            double tempLon = lon + k*360;
            if (tempLon >= -180)
            {
                lon = tempLon;
                break;
            }
        }
    }
    if (lon >= 180)
    {
        for (int k = 0; k < std::numeric_limits<int>::max(); ++k)
        {
            double tempLon = lon - k*360;
            if (tempLon < 180)
            {
                lon = tempLon;
                break;
            }
        }
    }
#ifndef NDEBUG
    assert(lon >= -180 && lon < 180);
#endif
    return lon;
}
}

class Origin::OriginImpl
{
public:
    double mLatitude = std::numeric_limits<double>::lowest();
    double mLongitude = std::numeric_limits<double>::lowest();
    double mDepth{0};
    int64_t mIdentifier{0};
    std::chrono::microseconds mTime{0};
    Origin::ReviewStatus mReviewStatus = Origin::ReviewStatus::AUTOMATIC;
    bool mHaveIdentifier{false};
    bool mHaveDepth{false};
    bool mHaveTime{false};
};

/// C'tor
Origin::Origin() :
    pImpl(std::make_unique<OriginImpl> ())
{
}

/// Copy c'tor
Origin::Origin(const Origin &origin)
{
    *this = origin;
}

/// Move c'tor
Origin::Origin(Origin &&origin) noexcept
{
    *this = std::move(origin);
}

/// Copy assignment
Origin& Origin::operator=(const Origin &origin)
{
    if (&origin == this){return *this;}
    pImpl = std::make_unique<OriginImpl> (*origin.pImpl);
    return *this;
}

/// Move c'tor
Origin& Origin::operator=(Origin &&origin) noexcept
{
    if (&origin == this){return *this;}
    pImpl = std::move(origin.pImpl);
    return *this;
}

/// Reset class
void Origin::clear() noexcept
{
    pImpl = std::make_unique<OriginImpl> ();
}

/// Destructor
Origin::~Origin() = default;

/// Latitude
void Origin::setLatitude(const double latitude)
{
    if (latitude <-90 || latitude > 90)
    {
        throw std::invalid_argument("Latitude = " + std::to_string(latitude)
                                  + " must be in range [-90,90]");
    }
    pImpl->mLatitude = latitude;
}

double Origin::getLatitude() const
{
    if (!haveLatitude()){throw std::runtime_error("Latitude not set");}
    return pImpl->mLatitude;
}

bool Origin::haveLatitude() const noexcept
{
    return (pImpl->mLatitude > std::numeric_limits<double>::lowest());
}

/// Longitude
double Origin::getLongitude() const
{
    if (!haveLongitude()){throw std::runtime_error("Longitude not set");}
    return pImpl->mLongitude;
}

void Origin::setLongitude(const double longitude) noexcept
{
    pImpl->mLongitude = shiftLongitude(longitude);
}

bool Origin::haveLongitude() const noexcept
{
    return (pImpl->mLongitude > std::numeric_limits<double>::lowest());
}

/// Depth
void Origin::setDepth(const double depth) noexcept
{
    pImpl->mDepth = depth;
    pImpl->mHaveDepth = true;
}

double Origin::getDepth() const
{
    if (!haveDepth()){throw std::runtime_error("Depth not set");}
    return pImpl->mDepth;
}

bool Origin::haveDepth() const noexcept
{
    return pImpl->mDepth;
}

/// Time
void Origin::setTime(const double time) noexcept
{
    auto iTime = static_cast<int64_t> (std::round(time*1.e6));
    setTime(std::chrono::microseconds {iTime});
}

void Origin::setTime(const std::chrono::microseconds &time) noexcept
{
    pImpl->mTime = time;
    pImpl->mHaveTime = true;
}

std::chrono::microseconds Origin::getTime() const
{
    if (!haveTime()){throw std::runtime_error("Time not set");}
    return pImpl->mTime;
}

bool Origin::haveTime() const noexcept
{
    return pImpl->mHaveTime;
}

/// Identifier
void Origin::setIdentifier(const int64_t id) noexcept
{
    pImpl->mIdentifier = id;
    pImpl->mHaveIdentifier = true;
}

int64_t Origin::getIdentifier() const
{
    if (!haveIdentifier()){throw std::runtime_error("Identifier not set");}
    return pImpl->mIdentifier;
}

bool Origin::haveIdentifier() const noexcept
{
    return pImpl->mIdentifier;
}

/// Review status
void Origin::setReviewStatus(const Origin::ReviewStatus status) noexcept
{
    pImpl->mReviewStatus = status;
}

Origin::ReviewStatus Origin::getReviewStatus() const noexcept
{
    return pImpl->mReviewStatus;
}
