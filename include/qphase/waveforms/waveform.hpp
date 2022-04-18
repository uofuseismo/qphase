#ifndef QPHASE_WAVEFORMS_WAVEFORM_HPP
#define QPHASE_WAVEFORMS_WAVEFORM_HPP
#include <memory>
#include <chrono>
namespace QPhase::Waveforms
{
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
        SAC /*!< This is a SAC waveform. */
    };
    /// @brief The precision of the underlying waveform.
    enum class Precision
    {
        FLOAT,   /*!< The data is stored as a 32 bit floating point. */
        DOUBLE,  /*!< The data is stored as a 64 bit floating point. */
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
}
#endif
