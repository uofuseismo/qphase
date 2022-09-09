#include <string>
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/waveforms/channel.hpp"
#include "private/shiftLongitude.hpp"

using namespace QPhase::Waveforms;

namespace
{

template<typename T>
void checkChannel(const Channel<T> &channel)
{
    if (!channel.haveWaveform())
    {
        throw std::runtime_error("Waveform not set on channel");
    }
    if (!channel.haveChannelCode())
    {
        throw std::runtime_error("Channel code not set");
    }
    auto channelCode = channel.getChannelCode();
    if (static_cast<int> (channelCode.size()) != 3)
    {
        throw std::runtime_error("Channel code size must be 3");
    }
    if (channelCode[2] == 'N' ||
        channelCode[2] == 'E' ||
        channelCode[2] == 'R' || 
        channelCode[2] == 'T')
    {
        throw std::invalid_argument("Channel code = " + channelCode
                                  + " appears to not be vertical");
    }
}

}

template<class T>
class SingleChannelSensor<T>::SingleChannelSensorImpl
{
public:
    Channel<T> mVerticalChannel;
    std::string mLocationCode;
    double mLatitude{0};
    double mLongitude{0};
    double mElevation{0};
    bool mHaveVerticalChannel{false};
    bool mHaveLatitude{false};
    bool mHaveLongitude{false};
    bool mHaveElevation{false};
};

/// C'tor
template<class T>
SingleChannelSensor<T>::SingleChannelSensor() :
    pImpl(std::make_unique<SingleChannelSensorImpl> ())
{
}

/// Copy c'tor
template<class T>
SingleChannelSensor<T>::SingleChannelSensor(
    const SingleChannelSensor<T> &sensor)
{
    *this = sensor;
}

/// Move c'tor
template<class T>
SingleChannelSensor<T>::SingleChannelSensor(
    SingleChannelSensor<T> &&sensor) noexcept
{
    *this = std::move(sensor);
}

/// Copy assignment
template<class T>
SingleChannelSensor<T>& 
    SingleChannelSensor<T>::operator=(const SingleChannelSensor<T> &sensor)
{
    if (&sensor == this){return *this;}
    pImpl = std::make_unique<SingleChannelSensorImpl> (*sensor.pImpl);
    return *this;
}

/// Move assignment
template<class T>
SingleChannelSensor<T>& 
    SingleChannelSensor<T>::operator=(SingleChannelSensor<T> &&sensor) noexcept
{
    if (&sensor == this){return *this;}
    pImpl = std::move(sensor.pImpl);
    return *this;
}

/// Vertical channel
template<class T>
void SingleChannelSensor<T>::setVerticalChannel(Channel<T> &&channel)
{
    checkChannel(channel);
    pImpl->mVerticalChannel = std::move(channel);
    if (!pImpl->mVerticalChannel.haveAzimuth())
    {
        pImpl->mVerticalChannel.setAzimuth(0);
    }   
    pImpl->mHaveVerticalChannel = true;
}

template<class T>
void SingleChannelSensor<T>::setVerticalChannel(const Channel<T> &channel)
{
    checkChannel(channel);
    pImpl->mVerticalChannel = channel;
    if (!pImpl->mVerticalChannel.haveAzimuth())
    {
        pImpl->mVerticalChannel.setAzimuth(0);
    }
    pImpl->mHaveVerticalChannel = true;
}

template<class T>
const Channel<T>& SingleChannelSensor<T>::getVerticalChannelReference() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
Channel<T> SingleChannelSensor<T>::getVerticalChannel() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
bool SingleChannelSensor<T>::haveVerticalChannel() const noexcept
{
    return pImpl->mHaveVerticalChannel;
}

/// Latitude
template<class T>
void SingleChannelSensor<T>::setLatitude(const double latitude)
{
    if (latitude < -90 || latitude > 90)
    {
        throw std::invalid_argument("Latitude = " + std::to_string(latitude)
                                  + " must be in range [-90,90]");
    }
    pImpl->mLatitude = latitude;
    pImpl->mHaveLatitude = true;
}

template<class T>
double SingleChannelSensor<T>::getLatitude() const
{
    if (!haveLatitude()){throw std::runtime_error("Latitude not set");}
    return pImpl->mLatitude;
}

template<class T>
bool SingleChannelSensor<T>::haveLatitude() const noexcept
{
    return pImpl->mHaveLatitude;
}

/// Longitude
template<class T>
void SingleChannelSensor<T>::setLongitude(const double longitude) noexcept
{
    pImpl->mLongitude = shiftLongitude(longitude);
    pImpl->mHaveLongitude = true;
}

template<class T>
double SingleChannelSensor<T>::getLongitude() const
{
    if (!haveLongitude()){throw std::runtime_error("Longitude not set");}
    return pImpl->mLongitude;
}

template<class T>
bool SingleChannelSensor<T>::haveLongitude() const noexcept
{
    return pImpl->mHaveLongitude;
}

/// Elevation 
template<class T>
void SingleChannelSensor<T>::setElevation(const double elevation)
{
    if (elevation < -15*1.e3 || elevation > 15*1.e3)
    {
        throw std::invalid_argument("Elevation must be in range +/- 15 km");
    }
    pImpl->mElevation = elevation;
    pImpl->mHaveElevation = true;
}

template<class T>
double SingleChannelSensor<T>::getElevation() const
{
    if (!haveElevation()){throw std::runtime_error("Elevation not set");}
    return pImpl->mElevation;
}

template<class T>
bool SingleChannelSensor<T>::haveElevation() const noexcept
{
    return pImpl->mHaveElevation;
}

/// Location code
template<class T>
void SingleChannelSensor<T>::setLocationCode(const std::string &locationCode)
{
    pImpl->mLocationCode = locationCode;
}

template<class T>
std::string SingleChannelSensor<T>::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// Reset class
template<class T>
void SingleChannelSensor<T>::clear() noexcept
{
    pImpl = std::make_unique<SingleChannelSensorImpl> ();
}

/// Destructor
template<class T>
SingleChannelSensor<T>::~SingleChannelSensor() = default;

///--------------------------------------------------------------------------///
///                          Template Instantiation                          ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::SingleChannelSensor<double>;
template class QPhase::Waveforms::SingleChannelSensor<float>;
