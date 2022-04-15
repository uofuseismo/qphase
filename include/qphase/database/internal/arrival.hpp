#ifndef QPHASE_DATABASE_INTERNAL_ARRIVAL_HPP
#define QPHASE_DATABASE_INTERNAL_ARRIVAL_HPP
#include <memory>
#include <chrono>
#include <ostream>
namespace QPhase::Database::Internal
{
/// @name Arrival "arrival.hpp" "qphase/database/internal/arrival.hpp"
/// @brief Defines a phase arrival (an associated pick).
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Arrival
{
public:
    /// @brief A simple way to specify the phase.
    enum class Phase : int8_t
    {
        P = 0, /*!< A P phase.  For regional network seismology this is likely Pg. */
        S = 1  /*!< An S phase.  For regional network seismology this is likely Sg. */
    }; 
    /// @brief The first motion.
    enum class FirstMotion : int8_t
    {
        UNKNOWN = 0, /*!< The first motion of the arrival is unknown. */
        UP =+1,      /*!< The first motion of the arrival is up. */
        DOWN =-1     /*!< The first motion of the arrival is down. */
    };
    /// @brief Defines whether this pick was created automatically or manually.
    enum class CreationMode : int8_t
    {
        AUTOMATIC = 0, /*!< This is an automatically picked arrival. */
        MANUAL = 1     /*!< This is a manually/reviewed picked arrival. */
    };
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Arrival();
    /// @brief Copy constructor.
    /// @param[in] arrival  The arrival class from which to initialize
    ///                     this class.
    Arrival(const Arrival &arrival);
    /// @brief Move constructor.
    /// @param[in,out] arrival  The arrival class from which to initialize
    ///                         this class.  On exit, arrival's behavior is
    ///                         undefined.
    Arrival(Arrival &&arrival) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] arrival  The arrival to copy to this.
    /// @result A deep copy of the input arrival.
    Arrival& operator=(const Arrival &arrival);
    /// @brief Move assignment operator.
    /// @param[in,out] arrival  The arrival whose memory will be moved to this.
    ///                         On exit, arrival's behavior is undefined.
    /// @result The memory moved from the arrival to this.
    Arrival& operator=(Arrival &&arrival) noexcept;
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

    /// @brief Sets the arrival time (time of the phase onset).
    /// @param[in] time  The arrival time (UTC) in seconds since the epoch.
    void setTime(double time) noexcept;
    /// @brief Sets the arrival time (time of the phase onset).
    /// @param[in] time  The arrival time (UTC) in microseconds since the epoch.
    void setTime(const std::chrono::microseconds &time) noexcept;  
    /// @result UTC time of pick in microseconds since epoch.
    [[nodiscard]] std::chrono::microseconds getTime() const;
    /// @result True indicates the arrival time was set.
    [[nodiscard]] bool haveTime() const noexcept;

    /// @brief Sets the phase name.
    void setPhase(const Phase phase) noexcept;
    /// @brief Sets the phase name. 
    /// @throws std::invalid_argument if the phase is empty.
    void setPhase(const std::string &phase);
    /// @result The name of the phase.
    [[nodiscard]] std::string getPhase() const;
    /// @result True indicates the phase was set.
    [[nodiscard]] bool havePhase() const noexcept;

    /// @}    

    /// @name Optional Parameters
    /// @{

    /// @brief Sets the first motion of the arrival.
    /// @param[in] firstMotion  The first motion of the arrival.
    void setFirstMotion(FirstMotion firstMotion) noexcept;
    /// @result The first motion of the arrival.
    [[nodiscard]] FirstMotion getFirstMotion() const noexcept;

    /// @brief Sets the creation mode.
    /// @param[in] mode   The creation mode.
    void setCreationMode(const CreationMode mode) noexcept;
    /// @result The creation mode.
    [[nodiscard]] CreationMode getCreationMode() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor.
    ~Arrival();
    /// @}
private:
    class ArrivalImpl;
    std::unique_ptr<ArrivalImpl> pImpl;
};
/// @result True indicates the arrivals are equal.
[[nodiscard]] bool operator==(const Arrival &lhs, const Arrival &rhs);
/// @result True indicates the arrivals are not equal.
[[nodiscard]] bool operator!=(const Arrival &lhs, const Arrival &rhs);
[[nodiscard]] std::ostream& operator<<(std::ostream &os, const Arrival &arrival);
}
#endif
