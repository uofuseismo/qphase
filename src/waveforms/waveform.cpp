#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <sff/sac/waveform.hpp>
#include <sff/utilities/time.hpp>
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/segment.hpp"

using namespace QPhase::Waveforms;

template<class T>
class Waveform<T>::WaveformImpl
{
public:
    void sortTemporally()
    {
        if (mSegments.empty()){return;}
        if (static_cast<int> (mSegments.size()) == 1){return;}
        std::sort(mSegments.begin(), mSegments.end(),
                  [](const Segment<T> &a, const Segment<T> &b)
                  {
                      return a.getStartTime() < b.getStartTime();
                  });
    }
    void updateCumulativeNumberOfSamples()
    {
        mCumulativeNumberOfSamples = 0;
        for (const auto &segment : mSegments)
        {
            mCumulativeNumberOfSamples = mCumulativeNumberOfSamples
                                       + segment.getNumberOfSamples();
        }
    }
    void updateEarliestLatestTime()
    {
        if (mSegments.empty())
        {
            mEarliestTime = std::chrono::microseconds {0};
            mLatestTime   = std::chrono::microseconds {0};
            return;
        }
        std::chrono::microseconds latestTime{-2208988800*1000000};   // Year 1900 
        std::chrono::microseconds earliestTime{ 4102444800*1000000}; // Year 2100
        for (const auto &segment : mSegments)
        {
            earliestTime = std::min(earliestTime, segment.getStartTime());
            latestTime   = std::max(latestTime,   segment.getEndTime());
        }
        mEarliestTime = earliestTime;
        mLatestTime = latestTime;
    }
    void update()
    {
        updateEarliestLatestTime();
        updateCumulativeNumberOfSamples();
        sortTemporally();
    }
    void clear() noexcept
    {
        mSegments.clear();
    }
    std::vector<Segment<T>> mSegments;
    std::string mNetwork;
    std::string mStation;
    std::string mChannel;
    std::string mLocationCode;
    std::chrono::microseconds mEarliestTime{0};
    std::chrono::microseconds mLatestTime{0};
    int mCumulativeNumberOfSamples{0};
};

/// C'tor
template<class T>
Waveform<T>::Waveform() :
    pImpl(std::make_unique<WaveformImpl> ())
{
}

/// Copy c'tor
template<class T>
Waveform<T>::Waveform(const Waveform &waveform)
{
    *this = waveform;
}

/// Move c'tor
template<class T>
Waveform<T>::Waveform(Waveform &&waveform) noexcept
{
    *this = std::move(waveform);
}

/// Copy operator
template<class T>
Waveform<T>& Waveform<T>::operator=(const Waveform<T> &waveform)
{
    if (&waveform == this){return *this;}
    pImpl = std::make_unique<WaveformImpl> (*waveform.pImpl);
    return *this;
}

/// Move operator
template<class T>
Waveform<T>& Waveform<T>::operator=(Waveform<T> &&waveform) noexcept
{
    if (&waveform == this){return *this;}
    pImpl = std::move(waveform.pImpl);
    return *this;
}

/// Destructor
template<class T>
Waveform<T>::~Waveform() = default;

/// Reset class and release memory
template<class T>
void Waveform<T>::clear() noexcept
{
    pImpl->clear();
}

/// Segments
template<class T>
std::vector<Segment<T>> Waveform<T>::getSegments() const noexcept
{
    return pImpl->mSegments;
}

/// Load wavefofrm
template<class T>
void Waveform<T>::load(const std::string &fileName,
                       const std::chrono::microseconds &t0,
                       const std::chrono::microseconds &t1,
                       const Waveform::FileType fileType)
{
    // Does the file exist?
    if (!std::filesystem::exists(fileName))
    {
        throw std::invalid_argument("File " + fileName + " does not exist");
    }
    auto fileTypeTry = fileType;
    if (fileType == Waveform::FileType::Unknown)
    {
        // Load waveform
        auto fileTypeLower = fileName;
        std::transform(fileTypeLower.begin(), fileTypeLower.end(),
                       fileTypeLower.begin(),
                       [](const unsigned char c)
        {
            return std::tolower(c);
        });
        if (fileTypeLower.find(".sac") != std::string::npos)
        {
            fileTypeTry = Waveform::FileType::SAC;
        }
    }
    // Attempt to load the file
    if (fileTypeTry == Waveform::FileType::SAC)
    {
        Waveform waveform;
        SFF::SAC::Waveform sacWaveform; 
        SFF::Utilities::Time startTime(t0.count()*1.e-6);
        SFF::Utilities::Time endTime(t1.count()*1.e-6);
        try
        {
            sacWaveform.read(fileName, startTime, endTime);
        }
        catch (const std::exception &e)
        {
            auto errorMessage = "Failed to load: " + fileName
                              + ".  Failed with: "
                              + std::string(e.what());
            throw std::runtime_error(errorMessage);
        }
        // Figure out some header information
        auto network = sacWaveform.getHeader(SFF::SAC::Character::KNETWK);
        auto station = sacWaveform.getHeader(SFF::SAC::Character::KSTNM);
        auto channel = sacWaveform.getHeader(SFF::SAC::Character::KCMPNM);
        auto locationCode = sacWaveform.getHeader(SFF::SAC::Character::KHOLE);
        if (network != "-12345"){waveform.setNetwork(network);}
        if (station != "-12345"){waveform.setStation(station);}
        if (channel != "-12345"){waveform.setChannel(channel);}
        if (locationCode != "-12345"){waveform.setLocationCode(locationCode);}
        // Copy some information 
        Segment<T> segment; 
        auto traceStartTime = sacWaveform.getStartTime().getEpoch();
        auto nSamples = sacWaveform.getNumberOfSamples();
        auto dataPtr  = sacWaveform.getDataPointer();
        segment.setSamplingRate(sacWaveform.getSamplingRate());
        segment.setStartTime(traceStartTime);
        segment.setData(nSamples, dataPtr);
        // Add the segment
        waveform.setSegments(std::move(segment));
        // This thing checks out
        *this = std::move(waveform);
    }
    else
    {
        throw std::runtime_error("Unhandled file type");
    }
}

template<class T>
void Waveform<T>::load(const std::string &fileName,
                       const Waveform::FileType fileType)
{
    constexpr std::chrono::microseconds t0{-2208988800*1000000}; // Year 1900 
    constexpr std::chrono::microseconds t1{ 4102444800*1000000}; // Year 2100
    load(fileName, t0, t1, fileType);
}

/// Sets the segments
template<class T>
void Waveform<T>::setSegments(Segment<T> &&segment)
{
    if (!segment.haveSamplingRate())
    {   
        throw std::invalid_argument("Segment does not have sampling rate");
    }   
    pImpl->mSegments.clear();
    pImpl->mSegments.push_back(std::move(segment));
    pImpl->update();
}

template<class T>
void Waveform<T>::setSegments(const Segment<T> &segment)
{
    if (!segment.haveSamplingRate())
    {
        throw std::invalid_argument("Segment does not have sampling rate");
    }
    pImpl->mSegments.clear();
    pImpl->mSegments.push_back(segment);
    pImpl->update();  
}

/// Get the number of segments
template<class T>
int Waveform<T>::getNumberOfSegments() const noexcept
{
    return static_cast<int> (pImpl->mSegments.size());
}

/// Get the cumulative number ofsamples
template<class T>
int Waveform<T>::getCumulativeNumberOfSamples() const noexcept
{
    return pImpl->mCumulativeNumberOfSamples;
}

/// Get the earliest time
template<class T>
std::chrono::microseconds Waveform<T>::getEarliestTime() const
{
    if (getNumberOfSegments() < 1){throw std::runtime_error("No segments");}
    return pImpl->mEarliestTime;
}

template<class T>
std::chrono::microseconds Waveform<T>::getLatestTime() const
{
    if (getNumberOfSegments() < 1){throw std::runtime_error("No segments");}
    return pImpl->mLatestTime;
}

/// Network
template<class T>
void Waveform<T>::setNetwork(const std::string &network)
{
    pImpl->mNetwork = network;
}

/// Station
template<class T>
void Waveform<T>::setStation(const std::string &station)
{
    pImpl->mStation = station;
}

/// Channel
template<class T>
void Waveform<T>::setChannel(const std::string &channel)
{
    pImpl->mChannel = channel;
}

/// Location code
template<class T>
void Waveform<T>::setLocationCode(const std::string &locationCode)
{
    pImpl->mLocationCode = locationCode;
}

/// Access
template<class T>
typename Waveform<T>::SegmentsType::iterator Waveform<T>::begin()
{
    return pImpl->mSegments.begin();
}

template<class T>
typename Waveform<T>::SegmentsType::iterator Waveform<T>::end()
{
    return pImpl->mSegments.end();
}

template<class T>
typename Waveform<T>::SegmentsType::const_iterator Waveform<T>::begin() const
{
    return pImpl->mSegments.begin();
}

template<class T>
typename Waveform<T>::SegmentsType::const_iterator Waveform<T>::end() const
{
    return pImpl->mSegments.end();
}

template<class T>
typename Waveform<T>::SegmentsType::const_iterator Waveform<T>::cbegin() const
{
    return pImpl->mSegments.cbegin();
}

template<class T>
typename Waveform<T>::SegmentsType::const_iterator Waveform<T>::cend() const
{
    return pImpl->mSegments.cend();
}

template<class T>
Segment<T>& Waveform<T>::at(const size_t i)
{
    return pImpl->mSegments.at(i);
}

template<class T>
const Segment<T>& Waveform<T>::at(const size_t i) const
{
    return pImpl->mSegments.at(i);
}

template<class T>
Segment<T>& Waveform<T>::operator[](const size_t i)
{
    return pImpl->mSegments[i];
}

template<class T>
const Segment<T>& Waveform<T>::operator[](const size_t i) const
{
    return pImpl->mSegments[i];
}

///--------------------------------------------------------------------------///
///                            Template Instantiation                        ///
///--------------------------------------------------------------------------///
template class QPhase::Waveforms::Waveform<double>;
template class QPhase::Waveforms::Waveform<float>;
