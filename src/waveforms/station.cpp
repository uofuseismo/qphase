#include <string>
#include <vector>
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"

using namespace QPhase::Waveforms;

template<class T>
class Station<T>::StationImpl
{
public:
    std::vector<SingleChannelSensor<T>> mThreeChannelSensors;
    std::vector<SingleChannelSensor<T>> mSingleChannelSensors;
    std::string mName;
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

///--------------------------------------------------------------------------///
///                             Template Instantiation                       ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::Station<double>;
template class QPhase::Waveforms::Station<float>;
