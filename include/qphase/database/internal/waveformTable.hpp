#ifndef QPHASE_DATABASE_INTERNAL_WAVEFORMTABLE_HPP
#define QPHASE_DATABASE_INTERNAL_WAVEFORMTABLE_HPP
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
  class Waveform;
 }
}
namespace QPhase::Database::Internal
{
/// @name WaveformTable "waveformTable.hpp" "qphase/database/internal/waveformTable.hpp"
/// @brief Defines a table of Waveforms.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class WaveformTable
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    WaveformTable();
    /// @}

    /// @brief Sets a connection to the internal database.
    void setConnection(std::shared_ptr<QPhase::Database::Connection::IConnection> &connection);
    /// @result True indicates the database is connected.
    [[nodiscard]] bool isConnected() const noexcept;

    /// @brief Queries the waveforms corresponding to a given event identifier.
    /// @param[in] eventIdentifier  The event identifier to which these
    ///                             waveforms are associated.
    /// @throws std::runtime_error if \c isConnected() is false.
    void query(int64_t waveformIdentifier);

    /// @result The waveforms that have been queried.
    [[nodiscard]] std::vector<Waveform> getWaveforms() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~WaveformTable();
    /// @}

    WaveformTable& operator=(const WaveformTable &) = delete;
    WaveformTable& operator=(WaveformTable &&) noexcept = delete;
    WaveformTable(const WaveformTable &) = delete;
    WaveformTable(WaveformTable &&) noexcept = delete;
private:
    class WaveformTableImpl;
    std::unique_ptr<WaveformTableImpl> pImpl;
};  
}
#endif
