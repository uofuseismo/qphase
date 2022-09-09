#include <iostream>
#include <string>
#include <algorithm>
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/simpleResponse.hpp"

using namespace QPhase::Waveforms;

namespace
{
[[nodiscard]] std::string removeBlanksAndCapitalize(const std::string &s)
{
    auto result = s;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace),
                 result.end());
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
}

template<class T>
class Channel<T>::ChannelImpl
{
public:
    Waveform<T> mWaveform;
    SimpleResponse mSimpleResponse;
    std::string mChannelCode;
    double mDip{0}; 
    double mAzimuth{0};
    bool mHaveWaveform{false};
    bool mHaveDip{false};
    bool mHaveAzimuth{false};
};

/// C'tor
template<class T>
Channel<T>::Channel() :
    pImpl(std::make_unique<ChannelImpl> ())
{
}

/// Copy c'tor
template<class T>
Channel<T>::Channel(const Channel &channel)
{
    *this = channel;
}

/// Move c'tor
template<class T>
Channel<T>::Channel(Channel &&channel) noexcept
{
    *this = std::move(channel);
}

/// Copy assignment
template<class T>
Channel<T>& Channel<T>::operator=(const Channel<T> &channel)
{
    if (&channel == this){return *this;}
    pImpl = std::make_unique<ChannelImpl> (*channel.pImpl);
    return *this;
}

/// Move assignment
template<class T>
Channel<T>& Channel<T>::operator=(Channel<T> &&channel) noexcept
{
    if (&channel == this){return *this;}
    pImpl = std::move(channel.pImpl);
    return *this;
}

/// Destructor
template<class T>
Channel<T>::~Channel() = default;

/*
/// Network code
template<class T>
void Channel<T>::setNetworkCode(const std::string &networkCode)
{
    auto s = removeBlanksAndCapitalize(networkCode);
    if (s.empty()){throw std::invalid_argument("Network code is empty");}
    pImpl->mNetworkCode = s;
}

template<class T>
std::string Channel<T>::getNetworkCode() const
{
    if (!haveNetworkCode()){throw std::runtime_error("Network code not set");}
    return pImpl->mNetworkCode;
}  

template<class T>
bool Channel<T>::haveNetworkCode() const noexcept
{
    return !pImpl->mNetworkCode.empty();
}

/// Station name
template<class T>
void Channel<T>::setStationName(const std::string &stationName)
{
    auto s = removeBlanksAndCapitalize(stationName);
    if (s.empty()){throw std::invalid_argument("Station name is empty");}
    pImpl->mStationName = s;
}

template<class T>
std::string Channel<T>::getStationName() const
{
    if (!haveStationName()){throw std::runtime_error("Station name not set");}
    return pImpl->mStationName;
}

template<class T>
bool Channel<T>::haveStationName() const noexcept
{
    return !pImpl->mStationName.empty();
}
*/

/// Channel code
template<class T>
void Channel<T>::setChannelCode(const std::string &channelCode)
{
    auto s = removeBlanksAndCapitalize(channelCode);
    if (static_cast<int> (s.size()) != 3)
    {
        throw std::invalid_argument("Channel code must be length 3");
    }
    pImpl->mChannelCode = s;
}

template<class T>
std::string Channel<T>::getChannelCode() const
{
    if (!haveChannelCode()){throw std::runtime_error("Channel code not set");}
    return pImpl->mChannelCode;
}

template<class T>
bool Channel<T>::haveChannelCode() const noexcept
{
    return !pImpl->mChannelCode.empty();
}

/// Location code
/*
template<class T>
void Channel<T>::setLocationCode(const std::string &locationCode)
{
    pImpl->mLocationCode = locationCode;
}

template<class T>
std::string Channel<T>::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}
*/

/// Dip
template<class T>
void Channel<T>::setDip(const double dip)
{
    if (dip < -90 || dip > 90)
    {
        throw std::invalid_argument("dip = " + std::to_string(dip)
                                  + " must be in range [-90,90]");
    }
    pImpl->mDip = dip;
    pImpl->mHaveDip = true;
}

template<class T>
double Channel<T>::getDip() const
{
    if (!haveDip()){throw std::runtime_error("Dip not set");}
    return pImpl->mDip;
}

template<class T>
bool Channel<T>::haveDip() const noexcept
{
    return pImpl->mHaveDip;
}

/// Azimuth 
template<class T>
void Channel<T>::setAzimuth(const double azimuth)
{
    if (azimuth < 0 || azimuth > 360)
    {
        throw std::invalid_argument("azimuth = " + std::to_string(azimuth)
                                  + " must be in range [0,360]");
    }
    pImpl->mAzimuth = azimuth;
    pImpl->mHaveAzimuth = true;
}

template<class T>
double Channel<T>::getAzimuth() const
{
    if (!haveAzimuth()){throw std::runtime_error("Azimuth not set");}
    return pImpl->mAzimuth;
}

template<class T>
bool Channel<T>::haveAzimuth() const noexcept
{
    return pImpl->mHaveAzimuth;
}

/// Waveform
template<class T>
void Channel<T>::setWaveform(Waveform<T> &&waveform)
{
    pImpl->mWaveform = std::move(waveform);
    pImpl->mHaveWaveform = true;
}

template<class T>
void Channel<T>::setWaveform(const Waveform<T> &waveform)
{
    pImpl->mWaveform = waveform;
    pImpl->mHaveWaveform = true;
}

template<class T>
const Waveform<T>& Channel<T>::getWaveformReference() const
{
    if (!haveWaveform()){throw std::runtime_error("Waveform not set");}
    return pImpl->mWaveform;
}

template<class T>
Waveform<T> Channel<T>::getWaveform() const
{
    if (!haveWaveform()){throw std::runtime_error("Waveform not set");}
    return pImpl->mWaveform;
}

template<class T>
bool Channel<T>::haveWaveform() const noexcept
{
    return pImpl->mHaveWaveform;
}

/// Simple response
template<class T>
void Channel<T>::setSimpleResponse(const SimpleResponse &response)
{
    if (!response.haveInputUnits())
    {
        throw std::invalid_argument("Input units not set");
    }
    if (!response.haveOutputUnits())
    {
        throw std::invalid_argument("Output units not set");
    }
    if (!response.haveScalar())
    {
        throw std::invalid_argument("Scalar not set");
    }
    if (!response.haveStartAndEndTime())
    {
        throw std::invalid_argument("Start and end times not set");
    }  
    pImpl->mSimpleResponse = response;
}

template<class T>
SimpleResponse Channel<T>::getSimpleResponse() const
{
    if (!haveSimpleResponse())
    {
        throw std::runtime_error("Simple response not set");
    }
    return pImpl->mSimpleResponse;
}

template<class T>
bool Channel<T>::haveSimpleResponse() const noexcept
{
    return pImpl->mSimpleResponse.haveScalar();
}

template<class T>
void Channel<T>::clear() noexcept
{
     pImpl = std::make_unique<ChannelImpl> ();
}

/// Convert
/*
template<typename T, typename U>
Channel<T> QPhase::Waveforms::convert(const Channel<U> &channel)
{
    Channel<T> result;

    if (channel.haveWaveform())
    {
        auto waveform = channel.getWaveformReference();
        result.setWaveform(std::move(convert<T>(waveform)));
    }
    if (channel.haveSimpleResponse())
    {
        result.setSimpleResponse(channel.getSimpleResponse());
    }
    if (channel.haveChannelCode())
    {
        result.setChannelCode(channel.getChannelCode());
    }
    if (channel.haveDip())
    {
        result.setDip(channel.getDip());
    }
    if (channel.haveAzimuth())
    {
        result.setAzimuth(channel.getAzimuth());
    }
    return result;
}

template<>
Channel<double> QPhase::Waveforms::convert(const Channel<double> &channel)
{
    return channel;
}

template<>
Channel<float> QPhase::Waveforms::convert(const Channel<float> &channel)
{
    return channel;
}
*/

///--------------------------------------------------------------------------///
///                            Template Instantiation                        ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::Channel<double>;
template class QPhase::Waveforms::Channel<float>;
//template Channel<double> QPhase::Waveforms::convert(const Channel<float> &);
//template Channel<float> QPhase::Waveforms::convert(const Channel<double> &);
