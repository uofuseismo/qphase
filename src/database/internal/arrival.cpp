#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>
#ifndef NDEBUG
#include <cassert>
#endif
#include "qphase/database/internal/arrival.hpp"
#include "private/isEmpty.hpp"

using namespace QPhase::Database::Internal;

namespace
{
[[nodiscard]] std::string convertString(const std::string &s) 
{
    auto temp = s;
    temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
    std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    return temp;
}
}

class Arrival::ArrivalImpl
{
public:
    std::string mNetwork;
    std::string mStation;
    std::string mChannel;
    std::string mLocationCode{"--"}; // SEED unknown
    std::string mPhase;
    std::chrono::microseconds mTime{0};
    int64_t mIdentifier{0};
    int64_t mOriginIdentifier{0};
    Arrival::CreationMode mCreationMode{Arrival::CreationMode::Automatic};
    Arrival::FirstMotion mFirstMotion{Arrival::FirstMotion::Unknown};
    bool mHaveIdentifier = false;
    bool mHaveOriginIdentifier = false;
    bool mHaveTime = false;
};

/// C'tor
Arrival::Arrival() :
    pImpl(std::make_unique<ArrivalImpl> ())
{
}

/// Copy c'tor
Arrival::Arrival(const Arrival &arrival)
{
    *this = arrival;
}

/// Move assignment
Arrival::Arrival(Arrival &&arrival) noexcept
{
    *this = std::move(arrival);
}

/// Copy assignment
Arrival& Arrival::operator=(const Arrival &arrival)
{
    if (&arrival == this){return *this;}
    pImpl = std::make_unique<ArrivalImpl> (*arrival.pImpl);
    return *this;
}

/// Move assignment
Arrival& Arrival::operator=(Arrival &&arrival) noexcept
{
    if (&arrival == this){return *this;}
    pImpl = std::move(arrival.pImpl);
    return *this;
}

/// Reset class
void Arrival::clear() noexcept
{
    pImpl = std::make_unique<ArrivalImpl> ();
}

/// Destructor
Arrival::~Arrival() = default;

/// Origin identifier
void Arrival::setOriginIdentifier(const int64_t id) noexcept
{
    pImpl->mOriginIdentifier = id;
    pImpl->mHaveOriginIdentifier = true;
}

int64_t Arrival::getOriginIdentifier() const
{
    if (!haveOriginIdentifier())
    {
        throw std::runtime_error("Origin identifier not set");
    }
    return pImpl->mOriginIdentifier;
}

[[nodiscard]] bool Arrival::haveOriginIdentifier() const noexcept
{
    return pImpl->mHaveOriginIdentifier;
}

/// Identifier
void Arrival::setIdentifier(const int64_t id) noexcept
{
    pImpl->mIdentifier = id;
    pImpl->mHaveIdentifier = true;
}

int64_t Arrival::getIdentifier() const
{
    if (!haveIdentifier())
    {
        throw std::runtime_error("Arrival identifier not set");
    }
    return pImpl->mIdentifier;
}

bool Arrival::haveIdentifier() const noexcept
{
    return pImpl->mHaveIdentifier;
}

/// Creation mode
void Arrival::setCreationMode(const CreationMode mode) noexcept
{
    pImpl->mCreationMode = mode;
}

Arrival::CreationMode Arrival::getCreationMode() const noexcept
{
    return pImpl->mCreationMode;
}

/// Time
void Arrival::setTime(const double time) noexcept
{
    auto iTime = static_cast<int64_t> (std::round(time*1.e6));
    setTime(std::chrono::microseconds {iTime});
}

void Arrival::setTime(const std::chrono::microseconds &time) noexcept
{
    pImpl->mTime = time;
    pImpl->mHaveTime = true;
}

std::chrono::microseconds Arrival::getTime() const
{
    if (!haveTime()){throw std::runtime_error("Time not set");}
    return pImpl->mTime;
}

bool Arrival::haveTime() const noexcept
{
    return pImpl->mHaveTime;
}

/// Network
void Arrival::setNetwork(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Network is empty");}
    pImpl->mNetwork = s;  
}

std::string Arrival::getNetwork() const
{
    if (!haveNetwork()){throw std::runtime_error("Network not set");}
    return pImpl->mNetwork;
}

bool Arrival::haveNetwork() const noexcept
{
    return !pImpl->mNetwork.empty();
}

/// Station
void Arrival::setStation(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Station is empty");}
    pImpl->mStation = s;  
}

std::string Arrival::getStation() const
{
    if (!haveStation()){throw std::runtime_error("Station not set");}
    return pImpl->mStation;
}

bool Arrival::haveStation() const noexcept
{
    return !pImpl->mStation.empty();
}

/// Channel
void Arrival::setChannel(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Channel is empty");}
    pImpl->mChannel = s;
}

std::string Arrival::getChannel() const
{
    if (!haveChannel()){throw std::runtime_error("Channel not set");}
    return pImpl->mChannel;
}

bool Arrival::haveChannel() const noexcept
{
    return !pImpl->mChannel.empty();
}

/// Location code
void Arrival::setLocationCode(const std::string &locationCode) noexcept
{
    pImpl->mLocationCode = locationCode;
}

std::string Arrival::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// First motion
void Arrival::setFirstMotion(const Arrival::FirstMotion firstMotion) noexcept
{
    pImpl->mFirstMotion = firstMotion;
}

Arrival::FirstMotion Arrival::getFirstMotion() const noexcept
{ 
    return pImpl->mFirstMotion;
}

/// Phase
void Arrival::setPhase(const Arrival::Phase phase) noexcept
{
    if (phase == Arrival::Phase::P)
    {
        setPhase("P");
    }
    else if (phase == Arrival::Phase::S)
    {
        setPhase("S");
    }
#ifndef NDEBUG
    else
    {
        assert(false);
    }
#endif
}

void Arrival::setPhase(const std::string &phase)
{
    if (isEmpty(phase)){throw std::invalid_argument("Phase is empty");}
    pImpl->mPhase = phase;
}

std::string Arrival::getPhase() const
{
    if (!havePhase()){throw std::runtime_error("Phase not set");}
    return pImpl->mPhase;
}

bool Arrival::havePhase() const noexcept
{
    return !pImpl->mPhase.empty();
}

/// Equality?
bool QPhase::Database::Internal::operator==(const Arrival &lhs,
                                            const Arrival &rhs)
{
    // First motion (b/c its easy)
    if (lhs.getFirstMotion() != rhs.getFirstMotion()){return false;}
    // Creation mode
    if (lhs.getCreationMode() != rhs.getCreationMode()){return false;}
    // Identifier
    if (lhs.haveIdentifier() && rhs.haveIdentifier())
    {
        if (lhs.getIdentifier() != rhs.getIdentifier())
        {
            return false;
        }
    }
    else
    {
        if (lhs.haveIdentifier() != rhs.haveIdentifier())
        {
            return false;
        }
    }
    // Time
    if (lhs.haveTime() && rhs.haveTime())
    {
        if (lhs.getTime() != rhs.getTime()){return false;}
    }
    else
    {
        if (lhs.haveTime() != rhs.haveTime()){return false;}
    }
    // Phase
    if (lhs.havePhase() && rhs.havePhase())
    {
        if (lhs.getPhase() != rhs.getPhase()){return false;}
    }
    else
    {
        if (lhs.havePhase() != rhs.havePhase()){return false;}
    }
    // Network
    if (lhs.haveNetwork() && rhs.haveNetwork())
    {
        if (lhs.getNetwork() != rhs.getNetwork()){return false;}
    }
    else
    {
        if (lhs.haveNetwork() != rhs.haveNetwork()){return false;}
    }
    // Station
    if (lhs.haveStation() && rhs.haveStation())
    {
        if (lhs.getStation() != rhs.getStation()){return false;}
    }
    else
    {
        if (lhs.haveStation() != rhs.haveStation()){return false;}
    }
    // Channel
    if (lhs.haveChannel() && rhs.haveChannel())
    {
        if (lhs.getChannel() != rhs.getChannel()){return false;}
    }
    else
    {
        if (lhs.haveChannel() != rhs.haveChannel()){return false;}
    }
    // Location code
    if (lhs.getLocationCode() != rhs.getLocationCode()){return false;}
    return true;
}

bool QPhase::Database::Internal::operator!=(const Arrival &lhs,
                                            const Arrival &rhs)
{
    return !(lhs == rhs);
}

std::ostream& QPhase::Database::Internal::operator<<(std::ostream &os,
                                                     const Arrival &arrival)
{
    std::string result{"Arrival:"};
    if (arrival.haveIdentifier())
    {   
        result = result + "\n   Identifier: "
               + std::to_string(arrival.getIdentifier());
    }
    if (arrival.haveOriginIdentifier())
    {
        result = result + "\n   Origin Identifier: "
               + std::to_string(arrival.getOriginIdentifier());
    }
    if (arrival.haveNetwork())
    {
        result = result + "\n   Network: " + arrival.getNetwork();
    }

    if (arrival.haveStation())
    {
        result = result + "\n   Station: " + arrival.getStation();
    }

    if (arrival.haveChannel())
    {
        result = result + "\n   Channel: " + arrival.getChannel();
    }

    //if (arrival.haveLocationCode())
    //{
        result = result + "\n   Location Code: " + arrival.getLocationCode();
    //}

    if (arrival.haveTime())
    {
        result = result + "\n   Time: "
               + std::to_string(arrival.getTime().count());
    }

    if (arrival.havePhase())
    {
        result = result + "\n   Phase: " + arrival.getPhase();
    }

    if (arrival.getFirstMotion() == Arrival::FirstMotion::Unknown)
    {
        result = result + "\n   First Motion: Unknown";
    }
    else if (arrival.getFirstMotion() == Arrival::FirstMotion::Up)
    {
        result = result + "\n   First Motion: Up";
    }
    else if (arrival.getFirstMotion() == Arrival::FirstMotion::Down)
    {
        result = result + "\n   First Motion: Down";
    }

    if (arrival.getCreationMode() == Arrival::CreationMode::Automatic)
    {
        result = result + "\n   Review Status: Automatic";
    }
    else if (arrival.getCreationMode() == Arrival::CreationMode::Manual)
    {
        result = result + "\n   Review Status: Manual";
    } 

    return os << result;
}

