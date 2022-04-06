#ifndef QPHASE_DATABASE_INTERNAL_EVENT_HPP
#define QPHASE_DATABASE_INTERNAL_EVENT_HPP
#include <memory>
namespace QPhase::Database::Internal
{
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
}
#endif
