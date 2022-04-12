#include <string>
#include <chrono>
#include "qphase/database/internal/arrival.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/magnitude.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/stationData.hpp"
#include "qphase/database/connection/sqlite3.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace QPhase::Database::Internal;

TEST(DatabaseConnection, SQLite3)
{
    QPhase::Database::Connection::SQLite3 sqlite3;
    EXPECT_NO_THROW(sqlite3.setFileName("test.sqlite3"));
    EXPECT_NO_THROW(sqlite3.connect());
    EXPECT_TRUE(sqlite3.isConnected());
}

TEST(DatabaseInternal, Arrival)
{
    const std::string networkLower{" uu"};
    const std::string network{"UU"};
    const std::string station{"PSUT"};
    const std::string channel{"HHZ"};
    const std::string locationCode{"01"};
    const std::string phase{"Pg"};
    const double time{110.12};
    const std::chrono::microseconds referenceTime{110120000};
    const int64_t arrivalID = 103;
    const auto creationMode = Arrival::CreationMode::MANUAL;
    const auto firstMotion = Arrival::FirstMotion::DOWN;
    Arrival arrival;
    arrival.setNetwork(network);
    arrival.setStation(station);
    arrival.setChannel(channel);
    arrival.setLocationCode(locationCode);
    arrival.setTime(time);
    arrival.setIdentifier(arrivalID);
    arrival.setCreationMode(creationMode);
    arrival.setFirstMotion(firstMotion);
    arrival.setPhase(phase);
 
    Arrival aCopy(arrival);
    EXPECT_EQ(aCopy.getNetwork(), network);
    EXPECT_EQ(aCopy.getStation(), station); 
    EXPECT_EQ(aCopy.getChannel(), channel);
    EXPECT_EQ(aCopy.getLocationCode(), locationCode);
    EXPECT_EQ(aCopy.getIdentifier(), arrivalID);
    EXPECT_EQ(aCopy.getCreationMode(), creationMode);
    EXPECT_EQ(aCopy.getFirstMotion(), firstMotion);
    EXPECT_EQ(aCopy.getTime(), referenceTime);
    EXPECT_EQ(aCopy.getPhase(), phase);

    EXPECT_TRUE(aCopy == arrival);

    arrival.clear();
    EXPECT_TRUE(aCopy != arrival);
    EXPECT_EQ(arrival.getFirstMotion(), Arrival::FirstMotion::UNKNOWN);
    EXPECT_EQ(arrival.getCreationMode(), Arrival::CreationMode::AUTOMATIC); 

    // Test macros
    arrival.setPhase(Arrival::Phase::P);
    EXPECT_EQ(arrival.getPhase(), "P");
    arrival.setPhase(Arrival::Phase::S);
    EXPECT_EQ(arrival.getPhase(), "S");
}

TEST(DatabaseInternal, Magnitude)
{
    const int64_t identifier = 423;
    const double value = 5.6;
    const std::string type{"l"};

    Magnitude magnitude;
    magnitude.setIdentifier(identifier);
    magnitude.setValue(value);
    magnitude.setType(type);
   
    Magnitude magCopy(magnitude);
    EXPECT_EQ(magCopy.getIdentifier(), identifier);
    EXPECT_EQ(magCopy.getType(), type);
    EXPECT_NEAR(magCopy.getValue(), value, 1.e-14);

    EXPECT_TRUE(magnitude == magCopy);
    magnitude.clear();
    EXPECT_TRUE(magnitude != magCopy);
}

TEST(DatabaseInternal, Origin)
{
    const double time{110.12};
    const double depth{10};
    const double latitude{41};
    const double longitude{-112 + 360};
    const std::chrono::microseconds referenceTime{110120000};
    auto reviewStatus = Origin::ReviewStatus::FINALIZED;
    const int64_t id = 103;

    Origin origin;
    origin.setIdentifier(id);
    origin.setLatitude(latitude);
    origin.setLongitude(longitude);
    origin.setDepth(depth);
    origin.setTime(time);
    origin.setReviewStatus(reviewStatus);

    Origin oCopy(origin);
    EXPECT_EQ(oCopy.getIdentifier(), id);
    EXPECT_EQ(oCopy.getTime(), referenceTime);
    EXPECT_NEAR(oCopy.getLongitude(), longitude - 360, 1.e-14);
    EXPECT_NEAR(oCopy.getLatitude(), latitude, 1.e-14);
    EXPECT_NEAR(oCopy.getDepth(), depth, 1.e-14);
    EXPECT_EQ(oCopy.getReviewStatus(), reviewStatus);

    origin.clear();
    EXPECT_EQ(origin.getReviewStatus(), Origin::ReviewStatus::AUTOMATIC);
}

TEST(DatabaseInternal, Event)
{
    const int64_t magIdentifier = 123;
    const double value = 3.6;
    const std::string magnitudeType{"d"};

    Magnitude magnitude;
    magnitude.setIdentifier(magIdentifier);
    magnitude.setValue(value);
    magnitude.setType(magnitudeType);

    const double time{112.12};
    const double depth{11};
    const double latitude{41.1};
    const double longitude{-112.5};
    auto originReviewStatus = Origin::ReviewStatus::FINALIZED;
    const int64_t originIdentifier = 103;

    Origin origin;
    origin.setIdentifier(originIdentifier);
    origin.setLatitude(latitude);
    origin.setLongitude(longitude);
    origin.setDepth(depth);
    origin.setTime(time);
    origin.setReviewStatus(originReviewStatus);

    Event event;
    const auto reviewStatus = Event::ReviewStatus::INCOMPLETE;
    const auto eventType = Event::Type::QUARRY_BLAST;
    EXPECT_NO_THROW(event.setOrigin(origin));
    EXPECT_NO_THROW(event.setMagnitude(magnitude));
    event.setType(eventType);
    event.setReviewStatus(reviewStatus);

    Event eCopy(event);
    EXPECT_EQ(eCopy.getOrigin(), origin);
    EXPECT_EQ(eCopy.getMagnitude(), magnitude);
    EXPECT_EQ(eCopy.getType(), eventType);
    EXPECT_EQ(eCopy.getReviewStatus(), reviewStatus);

    EXPECT_TRUE(eCopy == event);

    eCopy.clear();
    EXPECT_TRUE(eCopy != event);
    EXPECT_EQ(eCopy.getReviewStatus(), Event::ReviewStatus::AUTOMATIC);
    EXPECT_EQ(eCopy.getType(), Event::Type::UNKNOWN);
}

TEST(DatabaseInternal, StationData)
{
    const double latitude{40.77};
    const double longitude{-111.85 + 360};
    const double elevation{1449};
    const std::string description{"University of Utah EMCB Bldg, SLC, UT, USA"};
    const std::string network{"UU"};
    const std::string station{"UUE"};
    const std::chrono::microseconds onDate{1588291200000000};
    const std::chrono::microseconds offDate{1651363200000000};
    const std::chrono::microseconds loadDate{1651313200000000};
    
    StationData stationData;
    stationData.setNetwork(network);
    stationData.setStation(station);
    stationData.setLatitude(latitude);
    stationData.setLongitude(longitude);
    stationData.setElevation(elevation);
    stationData.setDescription(description);
    stationData.setOnOffDate(std::pair {onDate, offDate});
    stationData.setLoadDate(loadDate);

    StationData sCopy(stationData);
    EXPECT_EQ(sCopy.getNetwork(), network);
    EXPECT_EQ(sCopy.getStation(), station);
    EXPECT_EQ(sCopy.getDescription(), description);
    EXPECT_EQ(sCopy.getOnDate(), onDate);
    EXPECT_EQ(sCopy.getOffDate(), offDate);
    EXPECT_EQ(sCopy.getLoadDate(), loadDate);
    EXPECT_NEAR(sCopy.getLatitude(), latitude, 1.e-10);
    EXPECT_NEAR(sCopy.getLongitude(), longitude - 360, 1.e-10);
    EXPECT_NEAR(sCopy.getElevation(), elevation, 1.e-10);

    EXPECT_TRUE(sCopy == stationData);
    stationData.clear();
    EXPECT_TRUE(sCopy != stationData);
}

TEST(DatabaseInternal, StationDataTable)
{
 
}

}
