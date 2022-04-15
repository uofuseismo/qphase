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
#include "private/shiftLongitude.hpp"

using namespace QPhase::Database::Internal;

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

///--------------------------------------------------------------------------///
///                                Operators                                 ///
///--------------------------------------------------------------------------///

/*
    double mLatitude = std::numeric_limits<double>::lowest();
    double mLongitude = std::numeric_limits<double>::lowest();
    double mDepth{0};
    int64_t mIdentifier{0};
    std::chrono::microseconds mTime{0};
    Origin::ReviewStatus mReviewStatus = Origin::ReviewStatus::AUTOMATIC;
*/

bool QPhase::Database::Internal::operator==(
    const Origin &lhs, const Origin &rhs)
{
    if (lhs.getReviewStatus() != rhs.getReviewStatus()){return false;}

    if (lhs.haveIdentifier() != rhs.haveIdentifier()){return false;}
    if (lhs.haveIdentifier())
    {
        if (lhs.getIdentifier() != rhs.getIdentifier()){return false;}
    }

    if (lhs.haveTime() != rhs.haveTime()){return false;}
    if (lhs.haveTime())
    {
        if (lhs.getTime() != rhs.getTime()){return false;} 
    }

    if (lhs.haveLatitude() != rhs.haveLatitude()){return false;}
    if (lhs.haveLatitude())
    {   
        if (std::abs(lhs.getLatitude() - rhs.getLatitude()) >
            std::numeric_limits<double>::epsilon()*100)
        {
            return false;
        }
    }

    if (lhs.haveLongitude() != rhs.haveLongitude()){return false;}
    if (lhs.haveLongitude())
    {
        if (std::abs(lhs.getLongitude() - rhs.getLongitude()) >
            std::numeric_limits<double>::epsilon()*1000)
        {
            return false;
        }
    }

    if (lhs.haveDepth() != rhs.haveDepth()){return false;}
    if (lhs.haveDepth())
    {
        if (std::abs(lhs.getDepth() - rhs.getDepth()) >
            std::numeric_limits<double>::epsilon()*100)
        {
            return false;
        }
    }
 
    return true;
}

bool QPhase::Database::Internal::operator!=(
    const Origin &lhs, const Origin &rhs)
 {
    return !(lhs == rhs);
}

std::ostream& QPhase::Database::Internal::operator<<(std::ostream &os,
                                                     const Origin &origin)
{
    std::string result{"Origin:"};
    if (origin.haveIdentifier())
    {
        result = result + "\n   Identifier: "
               + std::to_string(origin.getIdentifier());
    }

    if (origin.haveLatitude())
    {
        result = result + "\n   Latitude: "
               + std::to_string(origin.getLatitude());
    }

    if (origin.haveLongitude())
    {
        result = result + "\n   Longitude: "
               + std::to_string(origin.getLongitude());
    }

    if (origin.haveDepth())
    {
        result = result + "\n   Depth: "
               + std::to_string(origin.getDepth());
    }

    if (origin.haveTime())
    {
        result = result + "\n   Time: " 
               + std::to_string(origin.getTime().count());
    }

    if (origin.getReviewStatus() == Origin::ReviewStatus::AUTOMATIC)
    {
        result = result + "\n   Review Status: Automatic";
    }
    else if (origin.getReviewStatus() == Origin::ReviewStatus::INCOMPLETE)
    {
        result = result + "\n   Review Status: Incomplete";
    }
    else if (origin.getReviewStatus() == Origin::ReviewStatus::FINALIZED)
    {
        result = result + "\n   Review Status: Finalized";
    }
    else if (origin.getReviewStatus() == Origin::ReviewStatus::CANCELLED)
    {
        result = result + "\n   Review Status: Cancelled";
    }
    else
    {
        result = result + "\n   Review Status: Unknown";
    }
    return os << result;
}

