#ifndef QPHASE_DATABASE_INTERNAL_WAVEFORM_HPP
#define QPHASE_DATABASE_INTERNAL_WAVEFORM_HPP
#include <memory>
#include <chrono>
#include <ostream>
namespace QPhase::Database::Internal
{
/// @name Waveform "waveform.hpp" "qphase/database/internal/waveform.hpp"
/// @brief Defines a wavform in the wveform table.  A waveform
///        defines the start/end time for waveform file, the file path,
///        as well as, potentially, the event identifier for which the
///        waveform will contain.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Waveform
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Waveform();
    /// @brief Copy constructor.
    /// @param[in] waveform  The waveform class from which to initialize
    ///                      this class.
    Waveform(const Waveform &waveform);
    /// @brief Move constructor.
    /// @param[in,out] waveform  The waveform class from which to initialize
    ///                          this class.  On exit, arrival's behavior is
    ///                          undefined.
    Waveform(Waveform &&waveform) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] waveform  The waveform to copy to this.
    /// @result A deep copy of the input waveform.
    Waveform& operator=(const Waveform &waveform);
    /// @brief Move assignment operator.
    /// @param[in,out] waveform  The waveform whose memory will be moved to
    ///                          this.  On exit, arrival's behavior is
    ///                          undefined.
    /// @result The memory moved from the waveform to this.
    Waveform& operator=(Waveform &&waveform) noexcept;
    /// @}

    /// @name Required Parameters
    /// @{

    /// @brief Sets the unique arrival identifier.
    /// @param[in] identifier  The arrival identifier.
    void setIdentifier(int64_t identifier) noexcept;
    /// @result The arrival identifier.
    /// @throws std::runtime_error if \c haveIdentifier() is false.
    [[nodiscard]] int64_t getIdentifier() const;
    /// @result True indicates the arrival identifier was set.
    [[nodiscard]] bool haveIdentifier() const noexcept;

    /// @brief Sets the network code - e.g., UU.
    /// @param[in] network  The network code.
    /// @throws std::invalid_argument if the network is empty.
    void setNetwork(const std::string &network);
    /// @result The network code.
    /// @throws std::invalid_argument if \c haveNetwork() is false.
    [[nodiscard]] std::string getNetwork() const;
    /// @Result True indicates the network was set.
    [[nodiscard]] bool haveNetwork() const noexcept;

    /// @brief Sets the station name - e.g., NLU.
    /// @param[in] station  The station name.
    /// @throws std::invalid_argument if the station is empty.
    void setStation(const std::string &station);
    /// @result The station name.
    /// @throws std::invalid_argument if \c haveStation() is false.
    [[nodiscard]] std::string getStation() const;
    /// @Result True indicates the station was set.
    [[nodiscard]] bool haveStation() const noexcept;

    /// @brief Sets the channel - e.g., for a P pick EHZ.
    /// @param[in] channel   The channel code.
    /// @throws std::invalid_argument if the channel is empty.
    void setChannel(const std::string &channel);
    /// @result The channel code.
    /// @throws std::invalid_argument if \c haveChannel() is false.
    [[nodiscard]] std::string getChannel() const;
    /// @Result True indicates the channel was set.
    [[nodiscard]] bool haveChannel() const noexcept;

    /// @brief Sets the location code - e.g., 01.
    /// @param[in] locationCode   The location code.
    void setLocationCode(const std::string &locationCode) noexcept;
    /// @result The location code.
    [[nodiscard]] std::string getLocationCode() const noexcept;

    /// @brief Sets the start time and end time of the file.
    /// @param[in] times  times.first is the start time (UTC) seconds
    ///                   since the epoch when the file begins and
    ///                   times.second is the end time (UTC) seconds
    ///                   since the epoch when the file ends.
    /// @throws std::invalid_argument if times.first >= times.second.
    void setStartAndEndTime(const std::pair<double, double> &times);
    /// @brief Sets the start time of the file.
    /// @param[in] times  times.first is the start time (UTC) microseconds
    ///                   since the epoch when the file begins and
    ///                   times.second is the end time (UTC) microseconds
    ///                   since the epoch when the file ends.
    /// @throws std::invalid_argument if times.first >= times.second.
    void setStartAndEndTime(const std::pair<std::chrono::microseconds, std::chrono::microseconds> &times);
    /// @result UTC time when file starts in microseconds since epoch.
    /// @throws std::runtime_error \c haveStartAndEndTime() is false.
    [[nodiscard]] std::chrono::microseconds getStartTime() const;
    /// @result UTC time when file ends in microseconds since epoch.
    /// @throws std::runtime_error \c haveStartAndEndTime() is false.
    [[nodiscard]] std::chrono::microseconds getEndTime() const;
    /// @result True indicates the arrival time was set.
    [[nodiscard]] bool haveStartAndEndTime() const noexcept;

    /// @brief Sets the file name.
    /// @param[in] fileName  The name of the file.
    void setFileName(const std::string &fileName);
    /// @result The file name.
    /// @throws std::runtime_error if \c haveFileName() is false.
    [[nodiscard]] std::string getFileName() const;
    /// @result True indicates the file name was set.
    [[nodiscard]] bool haveFileName() const noexcept;
    /// @}    

    /// @name Optional Parameters
    /// @{

    /// @brief Sets the event identifier.
    void setEventIdentifier(int64_t identifier) noexcept;
    /// @result The event identifier.
    /// @throws std::runtime_error if \c haveEventIdentifier() is false.
    [[nodiscard]] int64_t getEventIdentifier() const;
    /// @result True indicates that there is an event identifier.
    [[nodiscard]] bool haveEventIdentifier() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor.
    ~Waveform();
    /// @}
private:
    class WaveformImpl;
    std::unique_ptr<WaveformImpl> pImpl;
};
/// @result True indicates the waveforms are equal.
[[nodiscard]] bool operator==(const Waveform &lhs, const Waveform &rhs);
/// @result True indicates the waveforms are not equal.
[[nodiscard]] bool operator!=(const Waveform &lhs, const Waveform &rhs);
[[nodiscard]] std::ostream& operator<<(std::ostream &os, const Waveform &waveform);
}
#endif
