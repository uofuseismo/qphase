#include <string>
#include <chrono>
#include "qphase/database/internal/arrival.hpp"
#include "qphase/database/internal/origin.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace QPhase::Database::Internal;

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

}
