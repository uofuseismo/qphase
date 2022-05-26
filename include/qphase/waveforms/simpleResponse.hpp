#ifndef QPHASE_WAVEFORMS_SIMPLERESPONSE_HPP
#define QPHASE_WAVEFORMS_SIMPLERESPONSE_HPP
#include "qphase/waveforms/enums.hpp"
#include <chrono>
#include <memory>
namespace QPhase::Waveforms
{
/// @class SimpleResponse "simpleResponse.hpp" "qphase/waveforms/simpleResponse.hpp"
/// @brief A simple response is a loose-approximation to the full response.
///        We model the system by the following:
///              Output = Input*Scalar
///        It is assumed that the signal of interest is contained in the
///        instrument's (flat) passband.  In this instance, the deconvolution
///        is approximately by dividing by a scalar, hence, the input is 
///        recovered by division:
///              Input = Output/Scalar 
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class SimpleResponse
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor
    SimpleResponse();
    /// @brief Copy constructor.
    /// @param[in] response  The simple response from which to initialize
    ///                      this class.
    SimpleResponse(const SimpleResponse &response);
    /// @brief Move constructor.
    /// @param[in,out] response  The simple response from which to initialize
    ///                          this class.  On exit, response's behavior is
    ///                          undefined.
    SimpleResponse(SimpleResponse &&response) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] response  The simple response to copy to this.
    /// @result A deep copy of the input response.
    SimpleResponse& operator=(const SimpleResponse &response);
    /// @brief Move assignment.
    /// @param[in,out] response  The simple response whose memory will be
    ///                          moved to this.  On exit, response's behavior
    ///                          is undefined.
    /// @result The memory from response moved to this.
    SimpleResponse& operator=(SimpleResponse &&response) noexcept;
    /// @}

    /// @name Scalar
    /// @{
 
    /// @brief The scalar that encapsulates a simple instrument response.
    /// @param[in] scalar  The scalar explained above.
    /// @throws std::invalid_argument if this is zero.
    void setScalar(double scalar);
    /// @brief The deconvolution scalar.  Dividing by this will take the 
    ///        signal from its observed units (probably counts) to 
    ///        its input units (velocity or acceleration).
    /// @throws std::runtime_error if \c haveScalar() is false.
    [[nodiscard]] double getScalar() const;
    /// @result True indicates the scalar was set.
    [[nodiscard]] bool haveScalar() const noexcept;
    /// @}

    /// @name Input Units
    /// {

    /// @brief Sets the input units (these are the deconvolved units).
    /// @param[in] units   The input units.
    void setInputUnits(Units units) noexcept;
    /// @result The input units.
    [[nodiscard]] Units getInputUnits() const;
    /// @result True indicates the input units are set.
    [[nodiscard]] bool haveInputUnits() const noexcept;
    /// @}

    /// @name Output Units
    /// {

    /// @brief Sets the output units (these are the units for the raw data).
    /// @param[in] units   The output units.
    void setOutputUnits(Units units) noexcept;
    /// @result The output units.
    [[nodiscard]] Units getOutputUnits() const;
    /// @result True indicates the output units are set.
    [[nodiscard]] bool haveOutputUnits() const noexcept;
    /// @}

    /// @name Start Time and End Time
    /// @{

    /// @brief Sets the start and end time for when the response is valid.
    /// @param[in] startEndTime  startEndTime.first is the UTC time in
    ///                          microseconds since the epoch for when the
    ///                          resopnse begins to be valid.
    ///                          startEndTime.second is the UTC time in
    ///                          microseconds since the epoch for when the
    ///                          response ceases to be valid.
    /// @throws std::invalid_argument if startEndTime.second is not
    ///         greater than startEndTime.second.
    void setStartAndEndTime(const std::pair<std::chrono::microseconds,
                                            std::chrono::microseconds> &startEndTime);
    /// @result The start time for when the response begins to be valid.
    /// @throws std::runtime_error if \c haveStartAndEndTime() is false.
    [[nodiscard]] std::chrono::microseconds getStartTime() const;
    /// @result The end time for when the response ceases to be valid.
    /// @throws std::runtime_error if \c haveStartAndEndTime() is false.
    [[nodiscard]] std::chrono::microseconds getEndTime() const; 
    /// @result True indicates the start and end time were set.
    [[nodiscard]] bool haveStartAndEndTime() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets class and releases all memory.
    void clear() noexcept;
    /// @brief Destructor
    ~SimpleResponse();
    /// @}
private:
    class SimpleResponseImpl;
    std::unique_ptr<SimpleResponseImpl> pImpl;
}; 
}
#endif
