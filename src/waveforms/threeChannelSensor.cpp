#include "qphase/waveforms/threeChannelSensor.hpp"
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
}

template<typename T>
void checkVerticalChannel(const Channel<T> &channel)
{
    checkChannel(channel);
    auto channelCode = channel.getChannelCode();
    if (channelCode[2] == 'N' ||
        channelCode[2] == 'R' ||
        channelCode[2] == 'Q' ||
        channelCode[2] == 'E' ||
        channelCode[2] == 'T')
    {   
        throw std::invalid_argument("Channel code = " + channelCode
                                  + " appears to not be vertical");
    }
}

template<typename T>
void checkNorthChannel(const Channel<T> &channel)
{
    checkChannel(channel);
    auto channelCode = channel.getChannelCode();
    if (channelCode[2] == 'Z' ||
        channelCode[2] == 'L' ||
        channelCode[2] == 'E' ||
        channelCode[2] == 'T')
    {   
        throw std::invalid_argument("Channel code = " + channelCode
                                  + " appears to not be north");
    }
}

template<typename T>
void checkEastChannel(const Channel<T> &channel)
{
    checkChannel(channel);
    auto channelCode = channel.getChannelCode();
    if (channelCode[2] == 'Z' ||
        channelCode[2] == 'L' ||
        channelCode[2] == 'N' ||
        channelCode[2] == 'R' ||
        channelCode[2] == 'Q')
    {   
        throw std::invalid_argument("Channel code = " + channelCode
                                  + " appears to not be east");
    }
}

}

template<class T>
class ThreeChannelSensor<T>::ThreeChannelSensorImpl
{
public:
    Channel<T> mVerticalChannel;
    Channel<T> mNorthChannel;
    Channel<T> mEastChannel;
    std::string mLocationCode;
    double mLatitude;
    double mLongitude;
    double mElevation;
    bool mHaveVerticalChannel{false};
    bool mHaveNorthChannel{false};
    bool mHaveEastChannel{false};
    bool mHaveLatitude{false};
    bool mHaveLongitude{false};
    bool mHaveElevation{false};
};

/// C'tor
template<class T>
ThreeChannelSensor<T>::ThreeChannelSensor() :
    pImpl(std::make_unique<ThreeChannelSensorImpl> ()) 
{
}

/// Copy c'tor
template<class T>
ThreeChannelSensor<T>::ThreeChannelSensor(
    const ThreeChannelSensor<T> &sensor)
{
    *this = sensor;
}

/// Move c'tor
template<class T>
ThreeChannelSensor<T>::ThreeChannelSensor(
    ThreeChannelSensor<T> &&sensor) noexcept
{
    *this = std::move(sensor);
}

/// Copy assignment
template<class T>
ThreeChannelSensor<T>&
    ThreeChannelSensor<T>::operator=(const ThreeChannelSensor<T> &sensor)
{
    if (&sensor == this){return *this;}
    pImpl = std::make_unique<ThreeChannelSensorImpl> (*sensor.pImpl);
    return *this;
}

/// Move assignment
template<class T>
ThreeChannelSensor<T>&
    ThreeChannelSensor<T>::operator=(ThreeChannelSensor<T> &&sensor) noexcept
{
    if (&sensor == this){return *this;}
    pImpl = std::move(sensor.pImpl);
    return *this;
}

/// Vertical channel
template<class T>
void ThreeChannelSensor<T>::setVerticalChannel(Channel<T> &&channel)
{
    checkVerticalChannel(channel);
    pImpl->mVerticalChannel = std::move(channel);
    if (!pImpl->mVerticalChannel.haveAzimuth())
    {
        pImpl->mVerticalChannel.setAzimuth(0);
    }   
    pImpl->mHaveVerticalChannel = true;
}

template<class T>
void ThreeChannelSensor<T>::setVerticalChannel(const Channel<T> &channel)
{
    checkVerticalChannel(channel);
    pImpl->mVerticalChannel = channel;
    if (!pImpl->mVerticalChannel.haveAzimuth())
    {
        pImpl->mVerticalChannel.setAzimuth(0);
    }
    pImpl->mHaveVerticalChannel = true;
}

template<class T>
const Channel<T>& ThreeChannelSensor<T>::getVerticalChannelReference() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
Channel<T> ThreeChannelSensor<T>::getVerticalChannel() const
{
    if (!haveVerticalChannel())
    {
        throw std::runtime_error("Vertical channel not set");
    }
    return pImpl->mVerticalChannel;
}

template<class T>
bool ThreeChannelSensor<T>::haveVerticalChannel() const noexcept
{
    return pImpl->mHaveVerticalChannel;
}

/// North channel
template<class T>
void ThreeChannelSensor<T>::setNorthChannel(Channel<T> &&channel)
{
    checkNorthChannel(channel);
    pImpl->mNorthChannel = std::move(channel);
    if (!pImpl->mNorthChannel.haveAzimuth())
    {
        pImpl->mNorthChannel.setAzimuth(0);
    }   
    pImpl->mHaveNorthChannel = true;
}

template<class T>
void ThreeChannelSensor<T>::setNorthChannel(const Channel<T> &channel)
{
    checkNorthChannel(channel);
    pImpl->mNorthChannel = channel;
    if (!pImpl->mNorthChannel.haveAzimuth())
    {
        pImpl->mNorthChannel.setAzimuth(0);
    }
    pImpl->mHaveNorthChannel = true;
}

template<class T>
const Channel<T>& ThreeChannelSensor<T>::getNorthChannelReference() const
{
    if (!haveNorthChannel())
    {
        throw std::runtime_error("North channel not set");
    }
    return pImpl->mNorthChannel;
}

template<class T>
Channel<T> ThreeChannelSensor<T>::getNorthChannel() const
{
    if (!haveNorthChannel())
    {
        throw std::runtime_error("North channel not set");
    }
    return pImpl->mNorthChannel;
}

template<class T>
bool ThreeChannelSensor<T>::haveNorthChannel() const noexcept
{
    return pImpl->mHaveNorthChannel;
}

/// East channel
template<class T>
void ThreeChannelSensor<T>::setEastChannel(Channel<T> &&channel)
{
    checkEastChannel(channel);
    pImpl->mEastChannel = std::move(channel);
    if (!pImpl->mEastChannel.haveAzimuth())
    {
        pImpl->mEastChannel.setAzimuth(90);
    }   
    pImpl->mHaveEastChannel = true;
}

template<class T>
void ThreeChannelSensor<T>::setEastChannel(const Channel<T> &channel)
{
    checkEastChannel(channel);
    pImpl->mEastChannel = channel;
    if (!pImpl->mEastChannel.haveAzimuth())
    {
        pImpl->mEastChannel.setAzimuth(0);
    }
    pImpl->mHaveEastChannel = true;
}

template<class T>
const Channel<T>& ThreeChannelSensor<T>::getEastChannelReference() const
{
    if (!haveEastChannel())
    {
        throw std::runtime_error("East channel not set");
    }
    return pImpl->mEastChannel;
}

template<class T>
Channel<T> ThreeChannelSensor<T>::getEastChannel() const
{
    if (!haveEastChannel())
    {
        throw std::runtime_error("East channel not set");
    }
    return pImpl->mEastChannel;
}

template<class T>
bool ThreeChannelSensor<T>::haveEastChannel() const noexcept
{
    return pImpl->mHaveEastChannel;
}

/// Latitude
template<class T>
void ThreeChannelSensor<T>::setLatitude(const double latitude)
{
    if (latitude < -90 || latitude > 90)
    {
        throw std::invalid_argument("Latitude must be in range [-90,90]");
    }
    pImpl->mLatitude = latitude;
    pImpl->mHaveLatitude = true;
}

template<class T>
double ThreeChannelSensor<T>::getLatitude() const
{
    if (!haveLatitude()){throw std::runtime_error("Latitude not set");}
    return pImpl->mLatitude;
}

template<class T>
bool ThreeChannelSensor<T>::haveLatitude() const noexcept
{
    return pImpl->mHaveLatitude;
}

/// Longitude
template<class T>
void ThreeChannelSensor<T>::setLongitude(const double longitude) noexcept
{
    pImpl->mLongitude = shiftLongitude(longitude);
    pImpl->mHaveLongitude = true;
}

template<class T>
double ThreeChannelSensor<T>::getLongitude() const
{
    if (!haveLongitude()){throw std::runtime_error("Longitude not set");}
    return pImpl->mLongitude;
}

template<class T>
bool ThreeChannelSensor<T>::haveLongitude() const noexcept
{
    return pImpl->mHaveLongitude;
}

/// Elevation 
template<class T>
void ThreeChannelSensor<T>::setElevation(const double elevation)
{
    if (elevation < -15*1.e3 || elevation > 15*1.e3)
    {
        throw std::invalid_argument("Elevation must be in range +/- 15 km");
    }
    pImpl->mElevation = elevation;
    pImpl->mHaveElevation = true;
}

template<class T>
double ThreeChannelSensor<T>::getElevation() const
{
    if (!haveElevation()){throw std::runtime_error("Elevation not set");}
    return pImpl->mElevation;
}

template<class T>
bool ThreeChannelSensor<T>::haveElevation() const noexcept
{
    return pImpl->mHaveElevation;
}

/// Location code
template<class T>
void ThreeChannelSensor<T>::setLocationCode(const std::string &locationCode)
{
    pImpl->mLocationCode = locationCode;
}

template<class T>
std::string ThreeChannelSensor<T>::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// Reset class
template<class T>
void ThreeChannelSensor<T>::clear() noexcept
{
    pImpl = std::make_unique<ThreeChannelSensorImpl> (); 
}

/// Destructor
template<class T>
ThreeChannelSensor<T>::~ThreeChannelSensor() = default;

///--------------------------------------------------------------------------///
///                          Template Instantiation                          ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::ThreeChannelSensor<double>;
template class QPhase::Waveforms::ThreeChannelSensor<float>;
