#ifndef QNODE_TOPICS_HPP
#define QNODE_TOPICS_HPP
#include <memory>
#include "qphase/database/connection/connection.hpp"
namespace QPhase::Database::Connection
{
class IConnection;
class SQLite3;
}
namespace QPhase::QNode
{
class Topics
{
public:
    std::shared_ptr<QPhase::Database::Connection::IConnection> mInternalDatabaseConnection;
    std::shared_ptr<QPhase::Database::Connection::SQLite3> mScratchDatabaseConnection;
    std::shared_ptr<int64_t> mEventIdentifier{nullptr};
};
}
#endif
