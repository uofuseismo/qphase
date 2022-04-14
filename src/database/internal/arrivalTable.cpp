#include <mutex>
#include <vector>
#include <string>
#include <soci/soci.h>
#include "qphase/database/internal/arrivalTable.hpp"
#include "qphase/database/internal/arrival.hpp"
#include "qphase/database/connection/connection.hpp"

using namespace QPhase::Database::Internal;

template<> struct soci::type_conversion<Arrival>
{
    [[maybe_unused]] typedef values base_type;
    [[maybe_unused]] static void
    from_base(const values &v, indicator, Arrival &data)
    {
        // Required by schema
        data.setIdentifier(v.get<int> ("arid"));
        //data.setOrigin(v.get<int> ("orid"));

        data.setNetwork(v.get<std::string> ("network"));
        data.setStation(v.get<std::string> ("station"));
        data.setChannel(v.get<std::string> ("channel"));
        data.setLocationCode(v.get<std::string> ("location_code"));
/*
        data.setType(stringToEventType(v.get<std::string> ("event_type")));
        data.setReviewStatus(stringToEventReviewStatus(v.get<std::string>
                             ("event_review_status")));

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
*/
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

