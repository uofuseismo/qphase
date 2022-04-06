#include <string>
#include <chrono>
#include "qphase/database/internal/arrival.hpp"
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

}

}
