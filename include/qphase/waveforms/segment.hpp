#ifndef QPHASE_WAVEFORMS_SEGMENT_HPP
#define QPHASE_WAVEFORMS_SEGMENT_HPP
#include <memory>
#include <vector>
#include <chrono>
namespace QPhase::Waveforms
{
/// @brief This defines a waveform segment.  A waveform segment is a continuous
///        segment of data as you would find in a data packet.
template<class T = double>
class Segment
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Segment();
    /// @brief Copy constructor.
    /// @param[in] segment  The segment class from which to initialize this
    ///                     class.
    Segment(const Segment &segment);
    /// @brief Move constructor.
    /// @param[in,out] segment  The segment class from which to initialize this
    ///                         class.  On exit, segment's behavior is
    ///                         undefined.
    Segment(Segment &&segment) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] segment  The segment to copy to this.
    /// @result A deep copy of the input segment.
    Segment& operator=(const Segment &segment);
    /// @brief Move assignment operator.
    /// @param[in] segment  The segment whose memory will be moved to this.
    ///                     On exit, segment's behavior is undefined.
    /// @result The memory from segment moved to this.
    Segment& operator=(Segment &&segment) noexcept;
    /// @}

    /// @name Data
    /// @{

    /// @brief Sets the waveform dat afor the segment.
    /// @param[in] data  The waveform data to set for this segment.
    template<typename U> void setData(const std::vector<U> &data);
    /// @param[in] nSamples  The number of samples in the data.
    /// @param[in] data      The waveform data for this segment to set.
    ///                      This is an array with dimension [nSamples].
    template<typename U> void setData(int nSamples, const U *data);
    /// @result A pointer to the waveform data for this segment. 
    const T *getDataPointer() const noexcept;
    /// @result The waveform data for thsi segment.
    std::vector<T> getData() const noexcept;
 
    /// @result The number of samples in the segment.
    [[nodiscard]] int getNumberOfSamples() const noexcept;
    /// @}

    /// @name Sampling Rate
    /// @{

    /// @brief Sets the sampling rate.
    /// @param[in] samplingRate  The sampling rate in Hz.
    /// @throws std::invalid_argument if the sampling rate is not positive.
    void setSamplingRate(double samplingRate);
    /// @result The sampling rate in Hz.
    /// @throws std::runtime_error if \c haveSamplingRate() is false.
    [[nodiscard]] double getSamplingRate() const;
    /// @result The sampling period in seconds.
    /// @throws std::runtime_error if \c haveSamplingRate() is false.
    [[nodiscard]] double getSamplingPeriod() const;
    /// @result The sampling period in microseconds.
    /// @throws std::runtime_error if \c haveSamplingRate() is false.
    [[nodiscard]] std::chrono::microseconds getSamplingPeriodInMicroSeconds() const;
    /// @result True indicates the sampling rate was set.
    [[nodiscard]] bool haveSamplingRate() const noexcept;
    /// @}

    /// @name Start and End Time
    /// @{

    /// @brief Sets the start time of the segment.
    /// @param[in] startTime  The start time of the segment (UTC) in
    ///                       seconds since the epoch.
    void setStartTime(double startTime) noexcept;
    /// @param[in] startTime  The start time of the segment (UTC) in
    ///                       microseconds since the epoch.
    void setStartTime(const std::chrono::microseconds &startTime) noexcept;
    
    /// @result The start time of the segment in microseconds since the epoch.
    [[nodiscard]] std::chrono::microseconds getStartTime() const noexcept;
    /// @result The end time of the segment in microseconds since the epoch.
    [[nodiscard]] std::chrono::microseconds getEndTime() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the class. 
    void clear() noexcept;
    /// @brief Destructor.
    ~Segment();
    /// @}
private:
    class SegmentImpl;
    std::unique_ptr<SegmentImpl> pImpl;
};
/*
template<typename U, typename T>
[[nodiscard]]
Segment<U> convert(const Segment<T> &segment);
*/
}
#endif
