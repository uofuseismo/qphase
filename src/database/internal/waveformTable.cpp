#include <iostream>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>
#include <soci/soci.h>
#include "qphase/database/internal/waveformTable.hpp"
#include "qphase/database/internal/waveform.hpp"
#include "qphase/database/connection/connection.hpp"

using namespace QPhase::Database::Internal;

template<> struct soci::type_conversion<Waveform>
{
    [[maybe_unused]] typedef values base_type;
    [[maybe_unused]] static void
    from_base(const values &v, indicator, Waveform &data)
    {
        data.clear();
        // Required by schema
        data.setIdentifier(v.get<int> ("identifier"));

        data.setNetwork(v.get<std::string> ("network"));
        data.setStation(v.get<std::string> ("station"));
        data.setChannel(v.get<std::string> ("channel"));
        data.setLocationCode(v.get<std::string> ("location_code"));
 
        auto onTime  = (v.get<double> ("ontime"))*1.e-6;
        auto offTime = (v.get<double> ("offtime"))*1.e-6;
        data.setStartAndEndTime(std::pair<double, double> {onTime, offTime});
        try
        {
            data.setEventIdentifier(v.get<int64_t> ("event_identifier"));
        }
        catch (...)
        {
        }
        try
        {
            data.setFileName(v.get<std::string> ("filename"));
        }
        catch (...)
        {
        }
/*
        data.setTime(v.get<double> ("time")*1.e-6);
        data.setPhase(v.get<std::string> ("phase"));
        data.setFirstMotion(intToFirstMotion(v.get<int> ("first_motion")));
        data.setCreationMode(
            stringToCreationMode(v.get<std::string> ("creation_mode")));
*/
    }   
};

namespace
{

}

class WaveformTable::WaveformTableImpl
{
public:
    /// Get the station data
    [[nodiscard]] std::vector<Waveform> getData() const
    {   
        std::scoped_lock lock(mMutex);
        return mWaveforms;
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
    void queryEvent(const int64_t eventIdentifier)
    {
        std::scoped_lock lock(mMutex);
        auto session = mConnection->getSession();
        soci::rowset<Waveform>
           rows(session->prepare
                << "SELECT "
                << "  identifier, network, station, channel, "
                << "  location_code, event_identifier, ontime, offtime, filename "
                << " FROM waveform "
                << "   WHERE event_identifier = " << eventIdentifier);
        std::vector<Waveform> waveforms;
        for (const auto &it : rows)
        {
            if (it.haveFileName()){waveforms.push_back(it);}
        }
        mWaveforms = waveforms;
    }
    /// Set connection
    void setConnection(
        std::shared_ptr<QPhase::Database::Connection::IConnection> &connection)
    {
        std::scoped_lock lock(mMutex);
        mConnection = connection;
    }
    /// Get arrivals
    [[nodiscard]] std::vector<Waveform> getWaveforms() const
    {
        std::scoped_lock lock(mMutex);
        return mWaveforms;
    }
    mutable std::mutex mMutex;
    std::shared_ptr<QPhase::Database::Connection::IConnection>
        mConnection{nullptr};
    std::vector<Waveform> mWaveforms;

};

/// C'tor
WaveformTable::WaveformTable() :
    pImpl(std::make_unique<WaveformTableImpl> ())
{
}

/// Destructor
WaveformTable::~WaveformTable() = default;

/// Connected?
bool WaveformTable::isConnected() const noexcept
{
    return pImpl->isConnected();
}

/// Set the connection
void WaveformTable::setConnection(
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
void WaveformTable::query(const int64_t eventIdentifier)
{
    if (!isConnected()){throw std::runtime_error("No connection");}
    pImpl->queryEvent(eventIdentifier);
}

/// Gets the arrivals
std::vector<Waveform> WaveformTable::getWaveforms() const noexcept
{
    return pImpl->getWaveforms();
}

