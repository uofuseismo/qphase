#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <time/utc.hpp>
#include "qphase/database/internal/waveform.hpp"
#include "private/isEmpty.hpp"

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

using namespace QPhase::Database::Internal;

class Waveform::WaveformImpl
{
public:
    std::string mFileName;
    std::string mNetwork;
    std::string mStation;
    std::string mChannel;
    std::string mLocationCode{"--"}; // SEED default
    std::chrono::microseconds mStartTime{0};
    std::chrono::microseconds mEndTime{0};
    int64_t mIdentifier = 0;
    int64_t mEventIdentifier = 0;
    bool mHaveStartAndEndTime = false;
    bool mHaveIdentifier = false;
    bool mHaveEventIdentifier = false;
};

/// C'tor
Waveform::Waveform() :
    pImpl(std::make_unique<WaveformImpl> ())
{
}

/// Copy c'tor
Waveform::Waveform(const Waveform &waveform)
{
    *this = waveform;
}

/// Move c'tor
Waveform::Waveform(Waveform &&waveform) noexcept
{
    *this = std::move(waveform);
}

/// Copy assignment
Waveform& Waveform::operator=(const Waveform &waveform)
{
    if (&waveform == this){return *this;}
    pImpl = std::make_unique<WaveformImpl> (*waveform.pImpl);
    return *this;
}

/// Move assignment
Waveform& Waveform::operator=(Waveform &&waveform) noexcept
{
    if (&waveform == this){return *this;}
    pImpl = std::move(waveform.pImpl);
    return *this;
}

/// Reset class
void Waveform::clear() noexcept
{
    pImpl = std::make_unique<WaveformImpl> ();
}

/// Destructor
Waveform::~Waveform() = default;

/// Network
void Waveform::setNetwork(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Network is empty");}
    pImpl->mNetwork = s;
}

std::string Waveform::getNetwork() const
{
    if (!haveNetwork()){throw std::runtime_error("Network not set");}
    return pImpl->mNetwork;
}

bool Waveform::haveNetwork() const noexcept
{
    return !pImpl->mNetwork.empty();
}

/// Station
void Waveform::setStation(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Station is empty");}
    pImpl->mStation = s;
}

std::string Waveform::getStation() const
{
    if (!haveStation()){throw std::runtime_error("Station not set");}
    return pImpl->mStation;
}

bool Waveform::haveStation() const noexcept
{
    return !pImpl->mStation.empty();
}

/// Channel
void Waveform::setChannel(const std::string &sIn)
{
    auto s = convertString(sIn);
    if (isEmpty(s)){throw std::invalid_argument("Channel is empty");}
    pImpl->mChannel = s;
}

std::string Waveform::getChannel() const
{
    if (!haveChannel()){throw std::runtime_error("Channel not set");}
    return pImpl->mChannel;
}

bool Waveform::haveChannel() const noexcept
{
    return !pImpl->mChannel.empty();
}

/// Location code
void Waveform::setLocationCode(const std::string &locationCode) noexcept
{
    pImpl->mLocationCode = locationCode;
}

std::string Waveform::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// Filename
void Waveform::setFileName(const std::string &fileName)
{
    if (isEmpty(fileName)){throw std::invalid_argument("File name is empty");}
    if (!std::filesystem::exists(fileName))
    {
        throw std::invalid_argument("File: " + fileName + " does not exist");
    }
    pImpl->mFileName = fileName;
}

std::string Waveform::getFileName() const
{
    if (!haveFileName()){throw std::runtime_error("File name not set");}
    return pImpl->mFileName;
}

bool Waveform::haveFileName() const noexcept
{
    return !pImpl->mFileName.empty();
}

/// Identifier
void Waveform::setIdentifier(const int64_t id) noexcept
{
    pImpl->mIdentifier = id; 
    pImpl->mHaveIdentifier = true;
}

int64_t Waveform::getIdentifier() const
{
    if (!haveIdentifier())
    {   
        throw std::runtime_error("Waveform identifier not set");
    }   
    return pImpl->mIdentifier;
}

bool Waveform::haveIdentifier() const noexcept
{
    return pImpl->mHaveIdentifier;
}

/// Event identifier
void Waveform::setEventIdentifier(const int64_t id) noexcept
{
    pImpl->mEventIdentifier = id; 
    pImpl->mHaveEventIdentifier = true;
}

int64_t Waveform::getEventIdentifier() const
{
    if (!haveEventIdentifier())
    {
        throw std::runtime_error("Event identifier not set");
    }
    return pImpl->mEventIdentifier;
}

bool Waveform::haveEventIdentifier() const noexcept
{
    return pImpl->mHaveEventIdentifier;
}

/// Start/end time
void Waveform::setStartAndEndTime(
    const std::pair<double, double> &times)
{
    auto it0 = static_cast<int64_t> (std::round(times.first*1000000));
    auto it1 = static_cast<int64_t> (std::round(times.second*1000000));
    setStartAndEndTime(std::pair( std::chrono::microseconds {it0},
                                  std::chrono::microseconds {it1} ));
}

void Waveform::setStartAndEndTime(
    const std::pair<std::chrono::microseconds,
                    std::chrono::microseconds> &times)
{
    if (times.first >= times.second)
    {
        throw std::invalid_argument("times.first >= times.second");
    }
    pImpl->mStartTime = times.first;
    pImpl->mEndTime = times.second;
    pImpl->mHaveStartAndEndTime = true;
}

std::chrono::microseconds Waveform::getStartTime() const
{
    if (!haveStartAndEndTime())
    {
        throw std::runtime_error("Start time not set");
    }
    return pImpl->mStartTime;
}

std::chrono::microseconds Waveform::getEndTime() const
{
    if (!haveStartAndEndTime())
    {
        throw std::runtime_error("End time not set");
    }
    return pImpl->mEndTime;
}

bool Waveform::haveStartAndEndTime() const noexcept
{
    return pImpl->mHaveStartAndEndTime;
}

/// Equality?
bool QPhase::Database::Internal::operator==(const Waveform &lhs,
                                            const Waveform &rhs)
{
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
    // Event identifier
    if (lhs.haveEventIdentifier() && rhs.haveEventIdentifier())
    {
        if (lhs.getEventIdentifier() != rhs.getEventIdentifier())
        {
            return false;
        }
    }
    else
    {
        if (lhs.haveEventIdentifier() != rhs.haveEventIdentifier())
        {
            return false;
        }
    }
    // Time
    if (lhs.haveStartAndEndTime() && rhs.haveStartAndEndTime())
    {   
        if (lhs.getStartTime() != rhs.getStartTime()){return false;}
        if (lhs.getEndTime() != rhs.getEndTime()){return false;}
    }   
    else
    {
        if (lhs.haveStartAndEndTime() != rhs.haveStartAndEndTime())
        {
            return false;
        }
    } 
    // Filename
    if (lhs.haveFileName() && rhs.haveFileName())
    {
        if (lhs.getFileName() != rhs.getFileName()){return false;}
    }
    else
    {
        if (lhs.haveFileName() != rhs.haveFileName()){return false;}
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

bool QPhase::Database::Internal::operator!=(const Waveform &lhs,
                                            const Waveform &rhs)
{
    return !(lhs == rhs);
}

std::ostream& QPhase::Database::Internal::operator<<(std::ostream &os,
                                                     const Waveform &waveform)
{
    std::string result{"Waveform:"};
    if (waveform.haveIdentifier())
    {
        result = result + "\n   Identifier: "
               + std::to_string(waveform.getIdentifier());
    }

    if (waveform.haveNetwork())
    {
        result = result + "\n   Network: " + waveform.getNetwork();
    }

    if (waveform.haveStation())
    {
        result = result + "\n   Station: " + waveform.getStation();
    }

    if (waveform.haveChannel())
    {
        result = result + "\n   Channel: " + waveform.getChannel();
    }

    //if (waveform.haveLocationCode())
    //{
        result = result + "\n   Location Code: " + waveform.getLocationCode();
    //}

    if (waveform.haveStartAndEndTime())
    {
        Time::UTC t0{waveform.getStartTime().count()*1.e-6};
        Time::UTC t1{waveform.getEndTime().count()*1.e-6};
        std::stringstream st0;
        std::stringstream st1;
        st0 << t0;
        st1 << t1;
        result = result + "\n   Start Time: " + st0.str();
        result = result + "\n   End Time: " + st1.str();
    }

    if (waveform.haveEventIdentifier())
    {
        result = result + "\n   Event Identifier: "
               + std::to_string(waveform.getEventIdentifier());
    }

    if (waveform.haveFileName())
    {
        result = result + "\n   File: " + waveform.getFileName();
    }
    return os << result;
}
