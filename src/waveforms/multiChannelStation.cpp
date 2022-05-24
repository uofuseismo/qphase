#include <string>
#include <vector>
#include "qphase/waveforms/multiChannelStation.hpp"
#include "qphase/waveforms/waveform.hpp"

using namespace QPhase::Waveforms;

/// 
template<class T>
class MultiChannelStation<T>::MultiChannelStationImpl
{
public:
    std::map<Waveform<T>> mChannels;
    std::vector<int> mOrdering;
};

/// C'tor
template<class T>
MultiChannelStation<T>::MultiChannelStation() :
    pImpl(std::make_unique<MultiChannelStationImpl> ()) 
{
}

/// Copy c'tor
template<class T>
MultiChannelStation<T>::MultiChannelStation(
    const MultiChannelStation<T> &station)
{
    *this = station;
}

/// Move c'tor
template<class T>
MultiChannelStation<T>::MultiChannelStation(
    MultiChannelStation<T> &&station) noexcept
{
    *this = std::move(station);
}

/// Copy assignment
template<class T>
MultiChannelStation<T>
&MultiChannelStation<T>::operator=(const MultiChannelStation<T> &station)
{
    if (&station == this){return *this;}
    pImpl = std::make_unique<MultiChannelStationImpl> (*station.pImpl);
    return *this;
}

/// Move assignment
template<class T>
MultiChannelStation<T>
&MultiChannelStation<T>::operator=(MultiChannelStation<T> &&station) noexcept
{
    if (&station == this){return *this;}
    pImpl = std::move(station.pImpl);
    return *this;
}

/// Reset class
template<class T>
void MultiChannelStation<T>::clear() noexcept
{
    pImpl = std::make_unique<MultiChannelStationImpl> ();
}

/// Destructor
template<class T>
MultiChannelStation<T>::~MultiChannelStation() = default;

/// Gets the number of channels
template<class T>
int MultiChannelStation<T>::getNumberOfChannels() const noexcept
{
    return static_cast<int> (pImpl->mChannels.size());
}

///--------------------------------------------------------------------------///
///                             Template Instantiation                       ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::MultiChannelStation<double>;
template class QPhase::Waveforms::MultiChannelStation<float>;
