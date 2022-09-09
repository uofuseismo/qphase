#ifndef QPHASE_WAVEFORMS_WAVEFORM_HPP
#define QPHASE_WAVEFORMS_WAVEFORM_HPP
#include <memory>
#include <chrono>
#include <vector>
namespace QPhase::Waveforms
{
template<class T> class Segment;
}
namespace QPhase::Waveforms
{
/// @brief A waveform is a collection of waveform segments on a given 
///        sensor.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T>
class Waveform
{
private:
    using SegmentsType = std::vector<Segment<T>>;
public:
    using iterator = typename SegmentsType::iterator;
    using const_iterator = typename SegmentsType::const_iterator;
public:
    /// @brief The file type.
    enum class FileType
    {   
        SAC,       /*!< This is a SAC waveform. */
        Unknown,   /*!< Unknown waveform type.  Code will try to figure it out. */
        UNKNOWN = Unknown
    };
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Waveform();
    /// @brief Copy constructor.
    /// @param[in] waveform  The waveform from which to initialize this class.
    Waveform(const Waveform &waveform);
    /// @brief Move constructor.
    /// @param[in,out] waveform  The waveform from which to initialize this
    ///                          class.  On exit, waveform's behavior is
    ///                          undefined.
    Waveform(Waveform &&waveform) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] waveform  The waveform class to copy to this.
    /// @result A deep copy of the input waveform.
    Waveform& operator=(const Waveform &waveform);
    /// @brief Move assignment operator.
    /// @param[in] waveform  The waveform class whose memory will be moved
    ///                      to this.  On exit, waveform's behavior is
    ///                      undefined.
    /// @result The memory from waveform moved to this.
    Waveform& operator=(Waveform &&waveform) noexcept;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    Segment<T>& at(size_t pos);
    const Segment<T>& at(size_t pos) const;
    Segment<T>& operator[](size_t pos);
    const Segment<T>& operator[](size_t pos) const;
    /// @}

    /// @brief Loads a waveform from file.
    void load(const std::string &fileName,
              FileType fileType = FileType::Unknown);
    /// @brief Loads a waveform from file.  This will only retain
    ///        samples between the given times.
    void load(const std::string &fileName,
              const std::chrono::microseconds &t0,
              const std::chrono::microseconds &t1,
              FileType fileType = FileType::Unknown);

    /// @result The number of waveform segments.
    [[nodiscard]] int getNumberOfSegments() const noexcept;
    /// @result The cumulative number of samples.
    [[nodiscard]] int getCumulativeNumberOfSamples() const noexcept;
    /// @result The start time (UTC) of the earliest segment.
    [[nodiscard]] std::chrono::microseconds getEarliestTime() const;
    /// @result The end time (UTC) of the latest segment.
    [[nodiscard]] std::chrono::microseconds getLatestTime() const;

    /// @brief Sets the waveform segments.
    /// @param[in] segments  The waveform segments.  Each segment must have
    ///                      a valid sampling rate.  Empty segments will not
    ///                      be copied.
    void setSegments(const std::vector<Segment<T>> &segments);
    /// @param[in,out] segments  The waveform segments.  Each segment must have
    ///                          a valid sampling rate.  On exit, the behavior 
    ///                          of segments is undefined. 
    void setSegments(std::vector<Segment<T>> &&segments);
    /// @brief Sets the waveform segments.  In this case, there is only
    ///        one segment.
    /// @param[in] segment  The waveform segment.
    /// @throws std::invalid_argument if the segment does not have a valid
    ///         sampling rate.
    void setSegments(const Segment<T> &segment);
    /// @brief Sets the waveform segments.  In this case, there is only
    ///        one segment.
    /// @param[in,out] segment  On input, this contains the waveform segment.
    ///                         On exit, segment's behavior is undefined.
    /// @throws std::invalid_argument if the segment does not have a valid
    ///         sampling rate.
    void setSegments(Segment<T> &&segment);
    /// @result The waveform segments.
    [[nodiscard]] std::vector<Segment<T>> getSegments() const noexcept;

    /// @name Waveform Name
    /// @{
    //void setNetwork(const std::string &network);
    //[[nodiscard]] std::string getNetwork() const;
    //void setStation(const std::string &station);
    //[[nodiscard]] std::string getStation() const;
    //void setChannel(const std::string &channel);
    //[[nodiscard]] std::string getChannel() const;
    //void setLocationCode(const std::string &locationCode);
    //[[nodiscard]] std::string getLocationCode() const;
    /// @}

    /// @name Destructors
    /// @{
 
    /// @brief Resets the class and releases all memory.
    void clear() noexcept;
    /// @brief Destructor.
    ~Waveform();
    /// @}

private:
    class WaveformImpl;
    std::unique_ptr<WaveformImpl> pImpl;
};
/*
template<typename U, typename T>
[[nodiscard]]
Waveform<U> convert(const Waveform<T> &waveform);
*/
/*
/// @class IWaveform "waveform.hpp" "qphase/waveforms/waveform.hpp"
/// @brief This is an abstract base class that defines the essential properties
///        of a waveform.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class IWaveform
{
public:
    /// @brief The waveform type.
    enum class Type
    {
        SAC
    };
    /// @brief The precision of the underlying waveform.
    enum class Precision
    {
        FLOAT,
        DOUBLE,
        Float = FLOAT,
        Double = DOUBLE
    };

    /// @result The number of segments of data.
    [[nodiscard]] virtual int getNumberOfSegments() const = 0;
    /// @result The total number of data points.
    [[nodiscard]] virtual int getCumulativeNumberOfSamples() const = 0;
    /// @result The time of the earliest sample in microseconds since the epoch.
    [[nodiscard]] virtual std::chrono::microseconds getEarliestTime() const = 0;
    /// @result The time of the last sample in microseconds since the epoch.
    [[nodiscard]] virtual std::chrono::microseconds getLatestTime() const = 0;
    /// @result The waveform type.
    [[nodiscard]] virtual Type getType() const noexcept = 0;
    /// @result The precision.
    [[nodiscard]] virtual Precision getPrecision() const noexcept = 0; 
    /// @brief Destructor. 
    virtual ~IWaveform() = default;
};
*/
}
#endif
