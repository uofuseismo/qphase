#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <soci/soci.h>
#include "qphase/database/internal/stationDataTable.hpp"
#include "qphase/database/internal/stationData.hpp"
#include "qphase/database/connection/connection.hpp"
#include "qphase/database/connection/sqlite3.hpp"

using namespace QPhase::Database::Internal;

/*
    std::string mNetwork;
    std::string mStation;
    std::string mDescription;
    double mLatitude  = 0;
    double mLongitude = 0;
    double mElevation = 0;
    std::chrono::microseconds mOnDate{0};
    std::chrono::microseconds mOffDate{0};
    std::chrono::microseconds mLoadDate{0};
    bool mHaveLatitude = false;
    bool mHaveLongitude = false;
    bool mHaveElevation = false;
    bool mHaveOnOffDate = false;
    bool mHaveLoadDate = false;
};
*/

template<> struct soci::type_conversion<StationData>
{
    [[maybe_unused]] typedef values base_type;
    [[maybe_unused]] static void
    from_base(const values &v, indicator , StationData &data)
    {   
        // Required by schema
        data.setNetwork(v.get<std::string> ("network"));
        data.setStation(v.get<std::string> ("station"));
        data.setLatitude(v.get<double> ("latitude"));
        data.setLongitude(v.get<double> ("longitude"));
        data.setElevation(v.get<double> ("elevation"));
        auto onDate  = static_cast<int64_t> (v.get<double> ("ondate"));
        auto offDate = static_cast<int64_t> (v.get<double> ("offdate"));
        data.setOnOffDate(std::pair(std::chrono::microseconds {onDate},
                                    std::chrono::microseconds {offDate} ));
        // Optional
        std::string description;
        try
        {
            description = v.get<std::string> ("description");
        }
        catch (...)
        {
            description = "";
        }
        data.setDescription(description);
    }
};

class StationDataTable::StationDataTableImpl
{
public:
    /// Get the station data
    [[nodiscard]] std::vector<StationData> getData() const
    {
        std::scoped_lock lock(mMutex);
        return mStationData;
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
    /// Set connection
    void setConnection(
        std::shared_ptr<QPhase::Database::Connection::IConnection> &connection)
    {
        std::scoped_lock lock(mMutex);
        mConnection = connection;
    }
    /// Query
    void queryAll()
    {
        std::scoped_lock lock(mMutex);
        auto session = mConnection->getSession();
        soci::rowset<StationData>
           rows(session->prepare
                << "SELECT network, station, latitude, longitude, elevation, ondate, offdate, description FROM station_data");
        std::vector<StationData> stations;
        for (auto &it : rows)
        {
            stations.push_back(it);
        }
        mStationData = stations;
    }
    /// Get stations
    [[nodiscard]] std::vector<StationData> getStationData() const
    {
        std::scoped_lock lock(mMutex);
        return mStationData;
    }
    mutable std::mutex mMutex;
    std::shared_ptr<QPhase::Database::Connection::IConnection>
        mConnection{nullptr};
    std::vector<StationData> mStationData;
};

/// C'tor
StationDataTable::StationDataTable() :
    pImpl(std::make_unique<StationDataTableImpl> ())
{
}

/// Destructor
StationDataTable::~StationDataTable() = default;

/// Set the connection
void StationDataTable::setConnection(
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
bool StationDataTable::isConnected() const noexcept
{
    return pImpl->isConnected();
}

/// Query all
void StationDataTable::queryAll()
{
    if (!isConnected()){throw std::runtime_error("Connection not set");}
    pImpl->queryAll();
}

/// Get the stations
std::vector<StationData> StationDataTable::getStations() const noexcept
{
    return pImpl->getStationData();
}
