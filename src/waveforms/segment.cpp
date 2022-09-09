#include <cmath>
#include <vector>
#include <string>
#include "qphase/waveforms/segment.hpp"

using namespace QPhase::Waveforms;

template<class T>
class Segment<T>::SegmentImpl
{
public:
    void updateEndTime()
    {
        mEndTime = mStartTime;
        if (mSamplingRate > 0)
        {
            auto nSamples = static_cast<int64_t> (mWaveform.size());
            if (nSamples > 0)
            {
                auto dtMuS = static_cast<int64_t> (std::round( 1000000
                                                              /mSamplingRate ));
                mEndTime = mStartTime
                         + std::chrono::microseconds {(nSamples - 1)*dtMuS};
            }
        } 
    }
//private:
    std::vector<T> mWaveform;
    std::chrono::microseconds mStartTime{0};
    std::chrono::microseconds mEndTime{0};
    double mSamplingRate{0};
};

/// C'tor
template<class T>
Segment<T>::Segment() :
    pImpl(std::make_unique<SegmentImpl> ())
{
}

/// Copy c'tor
template<class T>
Segment<T>::Segment(const Segment &segment)
{
    *this = segment;
}

/// Move c'tor
template<class T>
Segment<T>::Segment(Segment &&segment) noexcept
{
    *this = std::move(segment);
}

/// Copy assignment
template<class T>
Segment<T>& Segment<T>::operator=(const Segment<T> &segment)
{
    if (&segment == this){return *this;}
    pImpl = std::make_unique<SegmentImpl> (*segment.pImpl);
    return *this;
}

/// Move assignment
template<class T>
Segment<T>& Segment<T>::operator=(Segment<T> &&segment) noexcept
{
    if (&segment == this){return *this;}
    pImpl = std::move(segment.pImpl);
    return *this;
}

/// Clear
template<class T>
void Segment<T>::clear() noexcept
{
    pImpl = std::make_unique<SegmentImpl> ();
}

/// Destructor
template<class T>
Segment<T>::~Segment() = default;

/// Data
template<class T>
template<typename U>
void Segment<T>::setData(const std::vector<U> &data)
{
    setData(data.size(), data.data());
}

template<class T>
template<typename U>
void Segment<T>::setData(const int nSamples, const U *__restrict__ data)
{
    if (nSamples < 1)
    {
        pImpl->mWaveform.resize(0);
    }
    else
    {
        if (data == nullptr){throw std::invalid_argument("data is NULL");}
        // Copy data
        pImpl->mWaveform.resize(nSamples);
        T *__restrict__ waveformPtr = pImpl->mWaveform.data();
        std::copy(data, data + nSamples, waveformPtr);
    }
    pImpl->updateEndTime();
}

template<class T>
const T* Segment<T>::getDataPointer() const noexcept
{
    return pImpl->mWaveform.data();
}

template<class T>
std::vector<T> Segment<T>::getData() const noexcept
{
    return pImpl->mWaveform;
}

/// Number of samples
template<class T>
int Segment<T>::getNumberOfSamples() const noexcept
{
    return static_cast<int> (pImpl->mWaveform.size());
}

/// Sampling rate 
template<class T>
void Segment<T>::setSamplingRate(const double samplingRate)
{
    if (samplingRate <= 0)
    {
        throw std::invalid_argument("Sampling rate must be positive");
    }
    pImpl->mSamplingRate = samplingRate;
    pImpl->updateEndTime();
}

template<class T>
double Segment<T>::getSamplingPeriod() const
{
    return 1./getSamplingRate();
}

template<class T>
std::chrono::microseconds Segment<T>::getSamplingPeriodInMicroSeconds() const
{
    auto dt = getSamplingPeriod();
    std::chrono::microseconds
        dtMuS{static_cast<int64_t> (std::round(1000000*dt))};
    return dtMuS;
}

template<class T>
double Segment<T>::getSamplingRate() const
{
    if (!haveSamplingRate())
    {
        throw std::runtime_error("Sampling rate not set");
    }
    return pImpl->mSamplingRate;
}

template<class T>
bool Segment<T>::haveSamplingRate() const noexcept
{
    return (pImpl->mSamplingRate > 0);
}

/// Start time
template<class T>
void Segment<T>::setStartTime(const double startTime) noexcept
{
    auto it0 = static_cast<int64_t> (std::round(startTime*1000000));
    std::chrono::microseconds t0{it0};
    setStartTime(t0);
}

template<class T>
void Segment<T>::setStartTime(const std::chrono::microseconds &t0) noexcept
{
    pImpl->mStartTime = t0;
    pImpl->updateEndTime();
} 

template<class T>
std::chrono::microseconds Segment<T>::getStartTime() const noexcept
{
    return pImpl->mStartTime;
}

template<class T>
std::chrono::microseconds Segment<T>::getEndTime() const noexcept
{
    return pImpl->mEndTime;
}

/// Change precision
/*
template<typename T, typename U>
Segment<T> QPhase::Waveforms::convert(const Segment<U> &segment)
{
    Segment<T> result;
    auto nSamples = segment.getNumberOfSamples();
    const auto dataPtr = segment.getDataPointer();
    if (nSamples > 0 && dataPtr != nullptr)
    {
        result.setData(nSamples, dataPtr);
    }
    if (segment.haveSamplingRate())
    {
        result.setSamplingRate(segment.getSamplingRate());
    }
    result.setStartTime(segment.getStartTime()); 
    return result;
}

template<>
Segment<double> QPhase::Waveforms::convert(const Segment<double> &segment)
{
    return segment;
}

template<>
Segment<float> QPhase::Waveforms::convert(const Segment<float> &segment)
{
    return segment;
}
*/

///--------------------------------------------------------------------------///
///                          Template Instantiation                          ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::Segment<double>;
template class QPhase::Waveforms::Segment<float>;
template void QPhase::Waveforms::Segment<double>::setData(const std::vector<double> &);
template void QPhase::Waveforms::Segment<double>::setData(const std::vector<float> &);
template void QPhase::Waveforms::Segment<float>::setData(const std::vector<double> &);
template void QPhase::Waveforms::Segment<float>::setData(const std::vector<float> &);
template void QPhase::Waveforms::Segment<double>::setData(int, const double *);
template void QPhase::Waveforms::Segment<double>::setData(int, const float *);
template void QPhase::Waveforms::Segment<float>::setData(int, const double *); 
template void QPhase::Waveforms::Segment<float>::setData(int, const float *); 
/*
template QPhase::Waveforms::Segment<float>
    QPhase::Waveforms::convert(const QPhase::Waveforms::Segment<double> &);
template QPhase::Waveforms::Segment<double>
    QPhase::Waveforms::convert(const QPhase::Waveforms::Segment<float> &); 
*/
