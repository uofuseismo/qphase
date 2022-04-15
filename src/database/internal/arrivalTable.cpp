#include <iostream>
#include <mutex>
#include <vector>
#include <string>
#include <soci/soci.h>
#include "qphase/database/internal/arrivalTable.hpp"
#include "qphase/database/internal/arrival.hpp"
#include "qphase/database/connection/connection.hpp"

using namespace QPhase::Database::Internal;

namespace
{

Arrival::CreationMode stringToCreationMode(const std::string &status)
{
    if (status == "a" || status == "A")
    {
        return Arrival::CreationMode::AUTOMATIC;
    }
    else if (status == "h" || status == "H" ||
             status == "m" || status == "M")
    {
        return Arrival::CreationMode::MANUAL;
    }
    else
    {
        std::cerr << "Unknown creation mode: " << status << std::endl;
    }
    return Arrival::CreationMode::AUTOMATIC;
}

Arrival::FirstMotion intToFirstMotion(const int fm)
{
    if (fm == 0)
    {
        return Arrival::FirstMotion::UNKNOWN;
    }
    else if (fm == 1)
    {
        return Arrival::FirstMotion::UP;
    }
    else if (fm ==-1)
    {
        return Arrival::FirstMotion::DOWN;
    }
    else
    {
        std::cerr << "Unknown first motion: " << fm << std::endl;
    }
    return Arrival::FirstMotion::UNKNOWN;
}

}
/*
 identifier INTEGER PRIMARY KEY NOT NULL,
 origin INTEGER NOT NULL,
 network VARCHAR(32) NOT NULL,
 station VARCHAR(32) NOT NULL,
 channel VARCHAR(32) NOT NULL,
 location_code VARCHAR(32) NOT NULL DEFAULT '01',
 time DOUBLE NOT NULL,
 phase VARCHAR(8) NOT NULL,
 first_motion INTEGER DEFAULT 0,
 review_status VARCHAR(1) DEFAULT 'A',
*/

template<> struct soci::type_conversion<Arrival>
{
    [[maybe_unused]] typedef values base_type;
    [[maybe_unused]] static void
    from_base(const values &v, indicator, Arrival &data)
    {
        // Required by schema
        data.setIdentifier(v.get<int> ("arid"));
        //data.setOrigin(v.get<int> ("origin"));

        data.setNetwork(v.get<std::string> ("network"));
        data.setStation(v.get<std::string> ("station"));
        data.setChannel(v.get<std::string> ("channel"));
        data.setLocationCode(v.get<std::string> ("location_code"));
 
        data.setTime(v.get<double> ("time")*1.e-6);
        data.setPhase(v.get<std::string> ("phase"));
        data.setFirstMotion(intToFirstMotion(v.get<int> ("first_motion")));
        data.setCreationMode(
            stringToCreationMode(v.get<std::string> ("creation_mode")));
std::cout << data << std::endl;
    }
};

class ArrivalTable::ArrivalTableImpl
{
public:
    /// Get the station data
    [[nodiscard]] std::vector<Arrival> getData() const
    {
        std::scoped_lock lock(mMutex);
        return mArrivals;
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
    /// Query based on an origin
    void queryOrigin(const int64_t originIdentifier)
    {
        std::scoped_lock lock(mMutex);
        auto session = mConnection->getSession();
        soci::rowset<Arrival>
           rows(session->prepare
                << "SELECT "
                << "  identifier as arid, network, station, channel, "
                << "  location_code, time, first_motion, phase, "
                << "  review_status as creation_mode"
                << " FROM arrival "
                << "   WHERE origin = " << originIdentifier);
        std::vector<Arrival> arrivals;
        for (const auto &it : rows)
        {
            arrivals.push_back(it);
        }
        mArrivals = arrivals;
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
    std::vector<Arrival> mArrivals;
};

/// C'tor
ArrivalTable::ArrivalTable() :
    pImpl(std::make_unique<ArrivalTableImpl> ())
{
}

/// Destructor
ArrivalTable::~ArrivalTable() = default;

/// Connected?
bool ArrivalTable::isConnected() const noexcept
{
    return pImpl->isConnected();
}

/// Set the connection
void ArrivalTable::setConnection(
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

/// Query for arrivals corresponding to an event
void ArrivalTable::query(const int64_t originIdentifier)
{
    if (!isConnected()){throw std::runtime_error("No connection");}
    pImpl->queryOrigin(originIdentifier);
}
