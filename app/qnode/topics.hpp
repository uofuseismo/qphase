#ifndef QNODE_TOPICS_HPP
#define QNODE_TOPICS_HPP
#include <memory>
#include "qphase/database/connection/connection.hpp"
namespace QPhase::Database::Connection
{
class IConnection;
}
namespace QPhase::QNode
{
class Topics
{
public:
    std::shared_ptr<QPhase::Database::Connection::IConnection> mInternalDatabaseConnection;
};
}
#endif
