#ifndef QPHASE_DATABASE_CONNECTION_CONNECTION_HPP
#define QPHASE_DATABASE_CONNECTION_CONNECTION_HPP
namespace soci
{
class session;
}
namespace QPhase::Database::Connection
{
/// @class IConnection "sqlite3.hpp" "qphase/database/connection/sqlite3.hpp"
/// @brief Defines an abstract base class soci connection.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class IConnection
{
public:
    /// @brief Destructor.
    virtual ~IConnection() = default;
    /// @result True indicates the database is connected.
    [[nodiscard]] virtual bool isConnected() const noexcept = 0;
    /// @brief Gets a pointer to the SOCI database session.
    /// @result A pointer to the SOCI database session.
    [[nodiscard]] virtual soci::session *getSession() = 0;
    /// @result The database driver.
    virtual std::string getDriver() const noexcept = 0;
};
}
#endif
