#ifndef QPHASE_DATABASE_INTERNAL_STATIONDATATABLE_HPP
#define QPHASE_DATABASE_INTERNAL_STATIONDATATABLE_HPP
#include <memory>
namespace QPhase::Database
{
 namespace Connection
 {
  class IConnection;
 }
 namespace Internal
 {
  class StationData;
 }
}
namespace QPhase::Database::Internal
{
/// @name StationDataTable "stationDataTable.hpp" "qphase/database/internal/stationDataTable.hpp"
/// @brief Defines a table of StationData.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class StationDataTable
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    StationDataTable();
    /// @}

    /// @brief Sets a connection to the internal database.
    void setConnection(std::shared_ptr<QPhase::Database::Connection::IConnection> &connection);
    /// @result True indicates the database is connected.
    [[nodiscard]] bool isConnected() const noexcept;
 
    /// @brief Queries all stations from the database.
    /// @throws std::runtime_error if \c isConnected() is false.
    void queryAll();
    /// @result The queried stations.
    [[nodiscard]] std::vector<StationData> getStations() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~StationDataTable();
    /// @}

    StationDataTable& operator=(const StationDataTable &) = delete;
    StationDataTable& operator=(StationDataTable &&) noexcept = delete;
    StationDataTable(const StationDataTable &) = delete;
    StationDataTable(StationDataTable &&) noexcept = delete;
private:
    class StationDataTableImpl;
    std::unique_ptr<StationDataTableImpl> pImpl;
};
}
#endif
