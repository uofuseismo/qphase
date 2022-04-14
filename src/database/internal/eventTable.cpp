#include <iostream>
#include <cmath>
#include <mutex>
#include <vector>
#include <soci/soci.h>
#include "qphase/database/internal/eventTable.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/magnitude.hpp"
#include "qphase/database/connection/connection.hpp"

using namespace QPhase::Database::Internal;

template<> struct soci::type_conversion<Event>
{
    [[maybe_unused]] typedef values base_type;
    [[maybe_unused]] static void
    from_base(const values &v, indicator , Event &data)
    {
        // Required by schema
        data.setIdentifier(v.get<int> ("evid"));

        Origin origin;
        origin.setIdentifier(v.get<int> ("orid"));
        origin.setLatitude(v.get<double> ("latitude"));
        origin.setLongitude(v.get<double> ("longitude"));
        origin.setDepth(v.get<double> ("depth"));
        double originTime = v.get<double> ("origin_time");
        origin.setTime(originTime*1.e-6);

        Magnitude magnitude;
        magnitude.setIdentifier(v.get<int> ("magid"));
        magnitude.setValue(v.get<double> ("magnitude"));
        magnitude.setType("M" + v.get<std::string> ("magnitude_type")); 

        data.setOrigin(origin);
        data.setMagnitude(magnitude);
    }
};


class EventTable::EventTableImpl
{
public:
    /// Get the station data
    [[nodiscard]] std::vector<Event> getData() const
    {
        std::scoped_lock lock(mMutex);
        return mEvents;
    }
    /// Connected?
    [[nodiscard]] bool isConnected() const noexcept
    {
        std::scoped_lock lock(mMutex);
        if (mConnection != nullptr)
        {
            return mConnection->isConnected();
        }   
        return false;
    }   
    /// Query
    void queryAll()
    {
        std::scoped_lock lock(mMutex);
        auto session = mConnection->getSession();
        soci::rowset<Event>
           rows(session->prepare
                << "SELECT event.identifier as evid, "
                << " origin.identifier as orid, origin.latitude as latitude, origin.longitude as longitude, origin.depth, origin.time as origin_time, "
                << " magnitude.identifier as magid, magnitude.magnitude as magnitude, magnitude.magnitude_type as magnitude_type "
                << "FROM "
                << "   event "
                << "   INNER JOIN origin ON event.preferred_origin = origin.identifier "
                << "   INNER JOIN magnitude ON event.preferred_magnitude =  magnitude.identifier");
        std::vector<Event> events;
        for (auto &it : rows)
        {
            events.push_back(it);
        }
        mEvents = std::move(events);
    }
    /// Set connection
    void setConnection(
        std::shared_ptr<QPhase::Database::Connection::IConnection> &connection)
    {
        std::scoped_lock lock(mMutex);
        mConnection = connection;
    }
    mutable std::mutex mMutex;
    std::shared_ptr<QPhase::Database::Connection::IConnection>
        mConnection{nullptr};
    std::vector<Event> mEvents;
};

/// C'tor
EventTable::EventTable() :
    pImpl(std::make_unique<EventTableImpl> ()) 
{
}

/// Query
void EventTable::queryAll()
{
    if (!isConnected()){throw std::runtime_error("No connection");}
    pImpl->queryAll();
}

/// Get events
std::vector<Event> EventTable::getEvents() const noexcept
{
    return pImpl->mEvents;
}

/// Destructor
EventTable::~EventTable() = default;

/// Set the connection
void EventTable::setConnection(
    std::shared_ptr<QPhase::Database::Connection::IConnection> &connection)
{
    if (connection == nullptr)
    {
        throw std::invalid_argument("Connection is NULL");
    }
    if (!connection->isConnected())
    {
        throw std::invalid_argument("Database connection not set");
    }
    pImpl->setConnection(connection);
}

/// Connected?
bool EventTable::isConnected() const noexcept
{
    return pImpl->isConnected();
}
