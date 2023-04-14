#include <string>
#include "qphase/waveforms/singleChannelVerticalSensor.hpp"
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
class SingleChannelVerticalSensor<T>::SingleChannelVerticalSensorImpl
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
SingleChannelVerticalSensor<T>::SingleChannelVerticalSensor() :
    pImpl(std::make_unique<SingleChannelVerticalSensorImpl> ())
{
}

/// Copy c'tor
template<class T>
SingleChannelVerticalSensor<T>::SingleChannelVerticalSensor(
    const SingleChannelVerticalSensor<T> &sensor)
{
    *this = sensor;
}

/// Move c'tor
template<class T>
SingleChannelVerticalSensor<T>::SingleChannelVerticalSensor(
    SingleChannelVerticalSensor<T> &&sensor) noexcept
{
    *this = std::move(sensor);
}

template<class T>
SingleChannelVerticalSensor<T>::SingleChannelVerticalSensor(
    const SingleChannelSensor<T> &sensor) :
    pImpl(std::make_unique<SingleChannelVerticalSensorImpl> ()) 
{
    if (sensor.haveChannel())
    {
        auto channel = sensor.getChannelReference();
        checkChannel(channel);
        setVerticalChannel(channel);
    }
    //if (sensor.haveLocationCode())
    //{
    //    setLocationCode(sensor.getLocationCode());
    //}
    if (sensor.haveLatitude())
    {
        setLatitude(sensor.getLatitude());
    }
    if (sensor.haveLongitude())
    {
        setLongitude(sensor.getLongitude());
    }
    if (sensor.haveLatitude())
    {
        setElevation(sensor.getElevation());
    }
}


/// Copy assignment
template<class T>
SingleChannelVerticalSensor<T>& 
    SingleChannelVerticalSensor<T>::operator=(const SingleChannelVerticalSensor<T> &sensor)
{
    if (&sensor == this){return *this;}
    pImpl = std::make_unique<SingleChannelVerticalSensorImpl> (*sensor.pImpl);
    return *this;
}

/// Move assignment
template<class T>
SingleChannelVerticalSensor<T>& 
    SingleChannelVerticalSensor<T>::operator=(SingleChannelVerticalSensor<T> &&sensor) noexcept
{
    if (&sensor == this){return *this;}
    pImpl = std::move(sensor.pImpl);
    return *this;
}

/// Vertical channel
template<class T>
void SingleChannelVerticalSensor<T>::setVerticalChannel(Channel<T> &&channel)
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
void SingleChannelVerticalSensor<T>::setVerticalChannel(const Channel<T> &channel)
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
const Channel<T>& SingleChannelVerticalSensor<T>::getVerticalChannelReference() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
Channel<T> SingleChannelVerticalSensor<T>::getVerticalChannel() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
bool SingleChannelVerticalSensor<T>::haveVerticalChannel() const noexcept
{
    return pImpl->mHaveVerticalChannel;
}

/// Latitude
template<class T>
void SingleChannelVerticalSensor<T>::setLatitude(const double latitude)
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
double SingleChannelVerticalSensor<T>::getLatitude() const
{
    if (!haveLatitude()){throw std::runtime_error("Latitude not set");}
    return pImpl->mLatitude;
}

template<class T>
bool SingleChannelVerticalSensor<T>::haveLatitude() const noexcept
{
    return pImpl->mHaveLatitude;
}

/// Longitude
template<class T>
void SingleChannelVerticalSensor<T>::setLongitude(
    const double longitude) noexcept
{
    pImpl->mLongitude = shiftLongitude(longitude);
    pImpl->mHaveLongitude = true;
}

template<class T>
double SingleChannelVerticalSensor<T>::getLongitude() const
{
    if (!haveLongitude()){throw std::runtime_error("Longitude not set");}
    return pImpl->mLongitude;
}

template<class T>
bool SingleChannelVerticalSensor<T>::haveLongitude() const noexcept
{
    return pImpl->mHaveLongitude;
}

/// Elevation 
template<class T>
void SingleChannelVerticalSensor<T>::setElevation(const double elevation)
{
    if (elevation < -15*1.e3 || elevation > 15*1.e3)
    {
        throw std::invalid_argument("Elevation must be in range +/- 15 km");
    }
    pImpl->mElevation = elevation;
    pImpl->mHaveElevation = true;
}

template<class T>
double SingleChannelVerticalSensor<T>::getElevation() const
{
    if (!haveElevation()){throw std::runtime_error("Elevation not set");}
    return pImpl->mElevation;
}

template<class T>
bool SingleChannelVerticalSensor<T>::haveElevation() const noexcept
{
    return pImpl->mHaveElevation;
}

/// Location code
template<class T>
void SingleChannelVerticalSensor<T>::setLocationCode(
    const std::string &locationCode)
{
    pImpl->mLocationCode = locationCode;
}

template<class T>
std::string SingleChannelVerticalSensor<T>::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// Reset class
template<class T>
void SingleChannelVerticalSensor<T>::clear() noexcept
{
    pImpl = std::make_unique<SingleChannelVerticalSensorImpl> ();
}

/// Destructor
template<class T>
SingleChannelVerticalSensor<T>::~SingleChannelVerticalSensor() = default;

///--------------------------------------------------------------------------///
///                          Template Instantiation                          ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::SingleChannelVerticalSensor<double>;
template class QPhase::Waveforms::SingleChannelVerticalSensor<float>;
