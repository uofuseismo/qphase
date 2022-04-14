#ifndef QPHASE_DATABASE_INTERNAL_ARRIVALTABLE_HPP
#define QPHASE_DATABASE_INTERNAL_ARRIVALTABLE_HPP
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
  class Arrival;
 }
}
namespace QPhase::Database::Internal
{
/// @name ArrivalTable "arrivalTable.hpp" "qphase/database/internal/arrivalTable.hpp"
/// @brief Defines a table of Arrivals.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class ArrivalTable
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    ArrivalTable();
    /// @}

    /// @brief Sets a connection to the internal database.
    void setConnection(std::shared_ptr<QPhase::Database::Connection::IConnection> &connection);
    /// @result True indicates the database is connected.
    [[nodiscard]] bool isConnected() const noexcept;

    /// @brief Queries the arrivals corresponding to a given origin.
    /// @param[in] originIdentifier  The origin identifier to which these
    ///                              arrivals are associated.
    /// @throws std::runtime_error if \c isConnected() is false.
    void query(int originIdentifier);

    [[nodiscard]] std::vector<Arrival> getArrivals() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~ArrivalTable();
    /// @}

    ArrivalTable& operator=(const ArrivalTable &) = delete;
    ArrivalTable& operator=(ArrivalTable &&) noexcept = delete;
    ArrivalTable(const ArrivalTable &) = delete;
    ArrivalTable(ArrivalTable &&) noexcept = delete;
private:
    class ArrivalTableImpl;
    std::unique_ptr<ArrivalTableImpl> pImpl;
};
}
#endif
