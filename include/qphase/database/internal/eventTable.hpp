#ifndef QPHASE_DATABASE_INTERNAL_EVENTTABLE_HPP
#define QPHASE_DATABASE_INTERNAL_EVENTTABLE_HPP
#include <memory>
#include <chrono>
namespace QPhase::Database
{
 namespace Connection
 {
  class IConnection;
 }
 namespace Internal
 {
  class Event;
 }
}
namespace QPhase::Database::Internal
{
/// @name EventTable "eventTable.hpp" "qphase/database/internal/eventTable.hpp"
/// @brief Defines a table of Events.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class EventTable
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    EventTable();
    /// @}

    /// @brief Sets a connection to the internal database.
    void setConnection(std::shared_ptr<QPhase::Database::Connection::IConnection> &connection);
    /// @result True indicates the database is connected.
    [[nodiscard]] bool isConnected() const noexcept;

    /// @brief Queries all stations from the database.
    /// @throws std::runtime_error if \c isConnected() is false.
    void queryAll();

    [[nodiscard]] std::vector<Event> getEvents() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~EventTable();
    /// @}

    EventTable& operator=(const EventTable &) = delete;
    EventTable& operator=(EventTable &&) noexcept = delete;
    EventTable(const EventTable &) = delete;
    EventTable(EventTable &&) noexcept = delete;
private:
    class EventTableImpl;
    std::unique_ptr<EventTableImpl> pImpl;
};
}
#endif
