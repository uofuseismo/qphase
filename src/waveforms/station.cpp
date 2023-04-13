#include <string>
#include <vector>
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"
#include "qphase/waveforms/channel.hpp"
#include "private/removeBlanksAndCapitalize.hpp"

using namespace QPhase::Waveforms;

template<class T>
class Station<T>::StationImpl
{
public:
    void checkSensor(const SingleChannelSensor<T> &sensor) const
    {
        if (!sensor.haveVerticalChannel())
        {
            throw std::invalid_argument("Vertical channel not set");
        }
        // Get channel codes and check for existence
        auto locationCode = sensor.getLocationCode();
        auto zChannelCode
            = sensor.getVerticalChannelReference().getChannelCode();
        if (channelExists(zChannelCode, locationCode))
        {
            throw std::invalid_argument(zChannelCode + "." + locationCode
                                      + " already exists");
        }
    }
    void checkSensor(const ThreeChannelSensor<T> &sensor) const
    {
        if (!sensor.haveVerticalChannel())
        {   
            throw std::invalid_argument("Vertical channel not set");
        }   
        if (!sensor.haveNorthChannel())
        {   
            throw std::invalid_argument("North channel not set");
        }   
        if (!sensor.haveEastChannel())
        {   
            throw std::invalid_argument("East channel not set");
        }   
        // Get channel codes and check for existence
        auto locationCode = sensor.getLocationCode();
        auto zChannelCode
            = sensor.getVerticalChannelReference().getChannelCode(); 
        auto nChannelCode
            = sensor.getNorthChannelReference().getChannelCode();
        auto eChannelCode
            = sensor.getEastChannelReference().getChannelCode();
        if (channelExists(zChannelCode, locationCode))
        {
            throw std::invalid_argument(zChannelCode + "." + locationCode
                                      + " already exists");
        }
        if (channelExists(nChannelCode, locationCode))
        {
            throw std::invalid_argument(nChannelCode + "." + locationCode
                                  + " already exists");
        }
        if (channelExists(eChannelCode, locationCode))
        {   
            throw std::invalid_argument(eChannelCode + "." + locationCode
                                      + " already exists");
        }
    }
    // Check if channel exists
    bool channelExists(const std::string &channel,
                       const std::string &locationCode) const
    {
        for (const auto &sensor : mSingleChannelVerticalSensors)
        {
            auto sensorLocationCode = sensor.getLocationCode();
            if (sensorLocationCode == locationCode)
            {   
                auto zChannelCode
                    = sensor.getVerticalChannelReference().getChannelCode();
                if (channel == zChannelCode)
                {
                    return true;
                }
            }
        }
        for (const auto &sensor : mThreeChannelSensors)
        {
            auto sensorLocationCode = sensor.getLocationCode();
            if (sensorLocationCode == locationCode)
            {
                auto zChannelCode
                    = sensor.getVerticalChannelReference().getChannelCode(); 
                if (channel == zChannelCode)
                {
                    return true;
                }
                auto nChannelCode
                    = sensor.getNorthChannelReference().getChannelCode();
                if (channel == nChannelCode)
                {
                    return true;
                }
                auto eChannelCode
                    = sensor.getEastChannelReference().getChannelCode();
                if (channel == eChannelCode)
                {
                    return true;
                }
            }
        }
        return false;
    }
    std::vector<ThreeChannelSensor<T>> mThreeChannelSensors;
    std::vector<SingleChannelSensor<T>> mSingleChannelVerticalSensors;
    std::vector<Channel<T>> mSingleChannelSensors;
    std::string mName;
    std::string mNetworkCode;
};

/// C'tor
template<class T>
Station<T>::Station() :
    pImpl(std::make_unique<StationImpl> ())
{
}

/// Copy c'tor
template<class T>
Station<T>::Station(const Station<T> &station)
{
    *this = station;
}

/// Move c'tor
template<class T>
Station<T>::Station(Station<T> &&station) noexcept
{
    *this = std::move(station);
}

/// Copy assignment
template<class T>
Station<T>& Station<T>::operator=(const Station<T> &station)
{
    if (&station == this){return *this;}
    pImpl = std::make_unique<StationImpl> (*station.pImpl);
    return *this;
}

/// Move assignment
template<class T>
Station<T>& Station<T>::operator=(Station<T> &&station) noexcept
{
    if (&station == this){return *this;}
    pImpl = std::move(station.pImpl);
    return *this;
}

/// Reset class
template<class T>
void Station<T>::clear() noexcept
{
    pImpl = std::make_unique<StationImpl> ();
}

/// Destructor
template<class T>
Station<T>::~Station() = default;

/// Station's name
template<class T>
void Station<T>::setName(const std::string &name)
{
    auto s = removeBlanksAndCapitalize(name);
    if (s.empty()){throw std::invalid_argument("Station name is empty");}
    pImpl->mName = s;
}

template<class T>
std::string Station<T>::getName() const
{
    if (!haveName()){throw std::runtime_error("Station name not set");}
    return pImpl->mName;
}

template<class T>
bool Station<T>::haveName() const noexcept
{
    return !pImpl->mName.empty();
}

/// Station's name
template<class T>
void Station<T>::setNetworkCode(const std::string &network)
{
    auto s = removeBlanksAndCapitalize(network);
    if (s.empty()){throw std::invalid_argument("Network code is empty");}
    pImpl->mNetworkCode = s;
}

template<class T>
std::string Station<T>::getNetworkCode() const
{
    if (!haveNetworkCode()){throw std::runtime_error("Network code not set");}
    return pImpl->mNetworkCode;
}

template<class T>
bool Station<T>::haveNetworkCode() const noexcept
{
    return !pImpl->mNetworkCode.empty();
}

/// Adds a sensor
template<class T>
void Station<T>::add(const ThreeChannelSensor<T> &sensor)
{
    pImpl->checkSensor(sensor);
    pImpl->mThreeChannelSensors.push_back(sensor);
} 

template<class T>
void Station<T>::add(ThreeChannelSensor<T> &&sensor)
{
    pImpl->checkSensor(sensor);
    pImpl->mThreeChannelSensors.push_back(std::move(sensor));
}

/// Adds a sensor
template<class T>
void Station<T>::add(const SingleChannelSensor<T> &sensor)
{
    pImpl->checkSensor(sensor);
    pImpl->mSingleChannelVerticalSensors.push_back(sensor);
}

template<class T>
void Station<T>::add(SingleChannelSensor<T> &&sensor)
{
    pImpl->checkSensor(sensor);
    pImpl->mSingleChannelVerticalSensors.push_back(std::move(sensor));
}

/// Get total number of channels
template<class T>
int Station<T>::getNumberOfChannels() const noexcept
{
    auto nChannels
        = static_cast<int> (pImpl->mThreeChannelSensors.size())*3
        + static_cast<int> (pImpl->mSingleChannelVerticalSensors.size());
    return nChannels;
}

/// Get total numbe rof vertical channels
template<class T>
int Station<T>::getNumberOfVerticalChannels() const noexcept
{
    auto nVerticalChannels
        = static_cast<int> (pImpl->mThreeChannelSensors.size())
        + static_cast<int> (pImpl->mSingleChannelVerticalSensors.size());
    return nVerticalChannels;
}

/// Gets the three channel sensors
template<class T>
const std::vector<ThreeChannelSensor<T>>&
    Station<T>::getThreeChannelSensorsReference() const noexcept
{
    return pImpl->mThreeChannelSensors;
}

/// Gets the three channel sensors
template<class T>
const std::vector<SingleChannelSensor<T>>&
    Station<T>::getSingleChannelSensorsReference() const noexcept
{
    return pImpl->mSingleChannelVerticalSensors;
}

/// Channel exists?
template<class T>
bool Station<T>::channelExists(const std::string &channel,
                               const std::string &locationCode) const
{
    return pImpl->channelExists(channel, locationCode);
}

///--------------------------------------------------------------------------///
///                             Template Instantiation                       ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::Station<double>;
template class QPhase::Waveforms::Station<float>;
