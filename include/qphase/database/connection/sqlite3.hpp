#ifndef QPHASE_DATABASE_CONNECTION_SQLITE3_HPP
#define QPHASE_DATABASE_CONNECTION_SQLITE3_HPP
#include <memory>
#include "qphase/database/connection/connection.hpp"
namespace soci
{
class session;
}
namespace QPhase::Database::Connection
{
/// @class SQLite3 "sqlite3.hpp" "qphase/database/connection/sqlite3.hpp"
/// @brief Defines a SQLite3 database connection.  SQLite3 databases typically
///        exist on the application's hard drive so this usually amounts to 
///        opening a file.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class SQLite3 : public IConnection
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    SQLite3();
    /// @brief Move constructor.
    /// @param[in,out] database  The database class from which to initialize
    ///                          this class.  On exit, database's behavior is
    ///                          undefined.
    SQLite3(SQLite3 &&database) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Move assignment operator.
    /// @param[in,out] database  The database connection whose memory will be
    ///                          moved to this.  On exit, database's behavior
    ///                          is undefined.
    SQLite3& operator=(SQLite3 &&database) noexcept;
    /// @} 

    /// @name Initialization
    /// @{

    /// @brief Sets the name of the sqlite3 file.
    /// @param[in] fileName  The name of the sqlite3 file.
    /// @throws std::runtime_error if the path does not exist.
    void setFileName(const std::string &fileName);
    /// @result The path to the sqlite3 file.
    [[nodiscard]] std::string getFileName() const;
    /// @result True indicates the sqlite3 file name was not set.
    [[nodiscard]] bool haveFileName() const noexcept;

    /// @brief Sets the connection as read-only.
    void setReadOnly() noexcept;
    /// @brief Sets the connection as read/write.
    void setReadWrite() noexcept;
    /// @result True indicates this is a read-only connection.
    [[nodiscard]] bool isReadOnly() const noexcept;

    /// @brief Loads the parameters from an initialiation file.
    /// @param[in] fileName  The name of the initialization file.
    /// @param[in] section   The section of the initialization file with the
    ///                      sqlite3 information
    /// @throws std::invalid_argument if the initialization file does not exist.
    void parseInitializationFile(const std::string &fileName,
                                 const std::string &section = "SQLite3");
    /// @}

    /// @name Connect
    /// @{

    /// @brief Connects to the sqlite3 database.
    /// @throws std::runtime_error if \c haveFileName() is false.
    void connect();
    /// @brief Gets a pointer to the SOCI database session.
    /// @result A pointer to the SOCI database session.
    /// @throws std::runtime_error if \c isConnected() is false.
    [[nodiscard]] soci::session *getSession() override;
    /// @result True indicates the class is connected to the sqlite3 database.
    [[nodiscard]] bool isConnected() const noexcept override;
    /// @}

    /// @name Disconnect
    /// @{

    /// @brief Close the connection to the sqlite3 database.
    void close();
    /// @}

    /// @result The database driver.
    [[nodiscard]] std::string getDriver() const noexcept override;

    /// @name Destructors
    /// @{

    ~SQLite3() override;
    /// @}

    SQLite3(const SQLite3 &) = delete;
    SQLite3& operator=(const SQLite3 &) = delete;
private:
    class SQLite3Impl;
    std::unique_ptr<SQLite3Impl> pImpl;
};
}
#endif
