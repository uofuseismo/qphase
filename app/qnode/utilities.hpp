#ifndef QNODE_UTILITIES_HPP
#include <string>
#include <qphase/database/connection/sqlite3.hpp>
namespace
{

[[nodiscard]] [[maybe_unused]]
std::shared_ptr<QPhase::Database::Connection::IConnection>
    createSQLite3Connection(const std::string &sqlite3File,
                            const bool readOnly = false)
{
    auto sqlite3 = std::make_shared<QPhase::Database::Connection::SQLite3> ();
    sqlite3->setFileName(sqlite3File);
    if (readOnly)
    {
        sqlite3->setReadOnly();
    }
    else
    {
        sqlite3->setReadWrite();
    }
    sqlite3->connect();
    auto result
        = std::shared_ptr<QPhase::Database::Connection::IConnection> (sqlite3);
    return result;
}

}
#endif
