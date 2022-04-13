#include <iostream>
#include <string>
#include "qphase/webServices/comcat/event.hpp"
#include "private/shiftLongitude.hpp"

using namespace QPhase::WebServices::Comcat;

class Event::EventImpl
{
public:
    std::string mMagnitudeType;
    std::string mURL;
    std::string mNetwork = "UU";
    std::string mReviewStatus;
    double mDepth = 0;
    double mLatitude = 0;
    double mLongitude = 0;
    double mMagnitude = 0;
    double mOriginTime = 0;
    uint64_t mIdentifier = 0;
    bool mHaveDepth = false;
    bool mHaveLatitude = false;
    bool mHaveLongitude = false;
    bool mHaveMagnitude = false;
    bool mHaveIdentifier = false;
    bool mHaveOriginTime = false;
};

/// C'tor
Event::Event() :
    pImpl(std::make_unique<EventImpl> ())
{
}

/// Copy c'tor
Event::Event(const Event &event)
{
    *this = event;
}

/// Move c'tor
Event::Event(Event &&event) noexcept
{
    *this = std::move(event);
}

/// Copy assignment
Event& Event::operator=(const Event &event)
{
    if (&event == this){return *this;}
    pImpl = std::make_unique<EventImpl> (*event.pImpl);
    return *this;
}

/// Move assignment
Event& Event::operator=(Event &&event) noexcept
{
    if (&event == this){return *this;}
    pImpl = std::move(event.pImpl);
    return *this;
}

/// Destructor
Event::~Event() = default;

/// Reset class
void Event::clear() noexcept
{
    pImpl = std::make_unique<EventImpl> ();
}

/// Event identifier
void Event::setIdentifier(const uint64_t identifier) noexcept
{
    pImpl->mIdentifier = identifier;
    pImpl->mHaveIdentifier = true;
}

uint64_t Event::getIdentifier() const
{
    if (!haveIdentifier()){throw std::runtime_error("Identifier not set");}
    return pImpl->mIdentifier;
}

bool Event::haveIdentifier() const noexcept
{
    return pImpl->mHaveIdentifier;
}

/// Depth
void Event::setDepth(const double depth) noexcept
{
    pImpl->mDepth = depth;
    pImpl->mHaveDepth = true;
}

double Event::getDepth() const
{
    if (!haveDepth()){throw std::runtime_error("Depth not set");}
    return pImpl->mDepth;
}

bool Event::haveDepth() const noexcept
{
    return pImpl->mHaveDepth;
}

/// Latitude
void Event::setLatitude(const double latitude)
{
    if (latitude < -90 || latitude > 90)
    {
        throw std::invalid_argument("Latitude = " + std::to_string(latitude)
                                  + " must be in range [-90,90]");
    }
    pImpl->mLatitude = latitude;
    pImpl->mHaveLatitude = true;
}

double Event::getLatitude() const
{
    return pImpl->mLatitude;
}

bool Event::haveLatitude() const noexcept
{
    return pImpl->mHaveLatitude;
}

/// Longitude
void Event::setLongitude(const double longitude)
{
    // Place in range [-180, 180]
    pImpl->mLongitude = shiftLongitude(longitude);
    pImpl->mHaveLongitude = true;
}

double Event::getLongitude() const
{
    if (!haveLongitude()){throw std::runtime_error("Longitude not set");}
    return pImpl->mLongitude;
}

bool Event::haveLongitude() const noexcept
{
    return pImpl->mHaveLongitude;
}

/// Magnitude
void Event::setMagnitude(const double magnitude)
{
    if (magnitude > 10 || magnitude < -10)
    {
        throw std::invalid_argument("Magnitude must be in range [-10,10]");
    }
    pImpl->mMagnitude = magnitude;
    pImpl->mHaveMagnitude = true;
}

double Event::getMagnitude() const
{
    if (!haveMagnitude()){throw std::invalid_argument("Magnitude not set");}
    return pImpl->mMagnitude;
}

bool Event::haveMagnitude() const noexcept
{
    return pImpl->mHaveMagnitude;
}

/// The magnitude type
void Event::setMagnitudeType(const std::string &magnitudeType)
{
    if (magnitudeType.empty())
    {
        throw std::invalid_argument("Magnitude type cannot be empty");
    }
    pImpl->mMagnitudeType = magnitudeType;
}

std::string Event::getMagnitudeType() const
{
    if (!haveMagnitudeType())
    {
        throw std::runtime_error("Magnitude type not set");
    }
    return pImpl->mMagnitudeType;
}

bool Event::haveMagnitudeType() const noexcept
{
    return !pImpl->mMagnitudeType.empty();
}

/// Network identifier
void Event::setNetwork(const std::string &network)
{
    if (network.empty())
    {
        throw std::invalid_argument("Network cannot be empty");
    }
    pImpl->mNetwork = network;
    std::transform(network.begin(), network.end(),
                   pImpl->mNetwork.begin(), 
                   [](const char c)
                   {
                      return std::toupper(c);
                   });
}

std::string Event::getNetwork() const
{
    if (!haveNetwork()){throw std::runtime_error("Network not set");}
    return pImpl->mNetwork;
}

bool Event::haveNetwork() const noexcept
{
    return !pImpl->mNetwork.empty();
}

/// Sets the USGS URL
void Event::setURL(const std::string &url)
{
    if (url.empty())
    {
        throw std::invalid_argument("URL is NULL");
    }
    pImpl->mURL = url;
}

std::string Event::getURL() const
{
    if (!haveURL()){throw std::runtime_error("URL not set");}
    return pImpl->mURL;
}

bool Event::haveURL() const noexcept
{
    return !pImpl->mURL.empty();
}

/// Sets the origin time
void Event::setOriginTime(const double originTime) noexcept
{
    pImpl->mOriginTime = originTime;
    pImpl->mHaveOriginTime = true;
}

double Event::getOriginTime() const
{
    if (!haveOriginTime()){throw std::runtime_error("Origin time not set");}
    return pImpl->mOriginTime;
}

bool Event::haveOriginTime() const noexcept
{
    return pImpl->mHaveOriginTime;
}

/// Review flag
void Event::setReviewStatus(const std::string &reviewStatus)
{
    if (reviewStatus.empty())
    {
        throw std::invalid_argument("Review status cannot be empty");
    }
    pImpl->mReviewStatus = reviewStatus;     
}

std::string Event::getReviewStatus() const
{
    if (!haveReviewStatus())
    {
        std::runtime_error("Review flag not set");
    }
    return pImpl->mReviewStatus;
}

bool Event::haveReviewStatus() const noexcept
{
    return !pImpl->mReviewStatus.empty();
}
