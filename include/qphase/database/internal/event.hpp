#ifndef QPHASE_DATABASE_INTERNAL_EVENT_HPP
#define QPHASE_DATABASE_INTERNAL_EVENT_HPP
#include <memory>
namespace QPhase::Database::Internal
{
class Origin;
class Magnitude;
/// @name Event "event.hpp" "qphase/database/internal/event.hpp"
/// @brief Defines an event.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Event
{
public:
    /// @brief Defines the event's review status.
    enum class ReviewStatus : int8_t
    {   
        AUTOMATIC = 0,  /*!< This is an automatically created event and yes to
                             be reviewed. */
        INCOMPLETE = 1, /*!< This event was manually reviewed but has to be
                             finalized. */
        FINALIZED = 2,  /*!< This is a reviewed event. */
        CANCELLED = 3   /*!< This event has been cancelled. */
    };  
    /// @brief Defines the event type.
    enum class Type : int8_t
    {
        UNKNOWN = 0,          /*!< The event type is unknown. */
        LOCAL_EARTHQUAKE = 1, /*!< This is a local earthquake. */
        QUARRY_BLAST = 2,     /*!< This event is a quarry blast. */
    }; 
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Event();
    /// @brief Copy constructor.
    /// @param[in] event  The event class from which to initialize this class.
    Event(const Event &event);
    /// @brief Move constructor.
    /// @param[in,out] event  The event class from which to initialize this
    ///                       class.  On exit, event's behavior is undefined.
    Event(Event &&event) noexcept;
    /// @}

    /// @name Properties
    /// @{

    /// @brief Sets the identifier.
    /// @param[in] identifier  The event identifier.
    void setIdentifier(int64_t identifier) noexcept;
    /// @result The event identifier.
    /// @throws std::invalid_argument if \c haveIdentifier() is false.
    [[nodiscard]] int64_t getIdentifier() const;
    /// @result True indicates the identifier was set.
    [[nodiscard]] bool haveIdentifier() const noexcept;

    /// @brief Sets the event's origin.
    /// @param[in] origin  The event's origin.
    /// @throws std::invalid_argument if the origin's latitude, longitude,
    ///         depth, and time are not set.
    void setOrigin(const Origin &origin);
    /// @result The origin.
    /// @throws std::runtime_error if \c haveOrigin() is false.
    [[nodiscard]] Origin getOrigin() const;
    /// @result True indicates the origin was set.
    [[nodiscard]] bool haveOrigin() const noexcept;

    /// @brief Sets the event's magnitude.
    /// @param[in] magnitude  The event's magnitude.
    /// @throws std::invalid_argument if the magnitude's value and type
    ///         are not set.
    void setMagnitude(const Magnitude &magnitude);
    /// @result The magnitude.
    /// @throws std::runtime_error if \c haveMagnitude() is false.
    [[nodiscard]] Magnitude getMagnitude() const;
    /// @result True indicates the magnitude was set.
    [[nodiscard]] bool haveMagnitude() const noexcept;
    /// @}

    /// @name Optional Properties
    /// @{

    /// @brief Sets the event type.
    /// @param[in] type  The event type.
    void setType(Event::Type type) noexcept;
    /// @result The event type.
    [[nodiscard]] Event::Type getType() const noexcept;

    /// @brief Sets the review status.
    /// @param[in] status  The review status.
    void setReviewStatus(Event::ReviewStatus status) noexcept;
    /// @result The review status.
    [[nodiscard]] Event::ReviewStatus getReviewStatus() const noexcept; 
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] event  The event to copy to this.
    /// @result A deep copy of the input event.
    Event& operator=(const Event &event);
    /// @brief Move assignment operator.
    /// @param[in,out] event  The event whose memory will be moved to this.
    ///                       On exit, event's behavior is undefined.
    /// @result The memory from event moved to this.
    Event& operator=(Event &&event) noexcept;
    /// @} 

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor
    ~Event();
    /// @}
private:
    class EventImpl;
    std::unique_ptr<EventImpl> pImpl;
};
[[nodiscard]] bool operator==(const Event &lhs, const Event &rhs);
[[nodiscard]] bool operator!=(const Event &lhs, const Event &rhs);
}
#endif
