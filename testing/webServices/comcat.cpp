#include <string>
#include "qphase/webServices/comcat/event.hpp"
#include <gtest/gtest.h>
namespace
{

using namespace QPhase::WebServices::Comcat;

TEST(WebServicesComcat, Event)
{
    const double latitude  = 41;
    const double longitude =-112.5;
    const double originTime = 5;
    const double depth = 3;
    const double magnitude = 3.5;
    const std::string magnitudeType{"Ml"};
    const std::string network{"uu"}; // Will be capitalized
    const std::string networkRef{"UU"};
    const std::string reviewStatus{"r"};
    const uint64_t id{60039503};
    const std::string url{"https://quake.utah.edu"};
    Event event;

    event.setLatitude(latitude);
    event.setLongitude(longitude + 360);
    event.setDepth(depth);
    event.setOriginTime(originTime);
    event.setMagnitude(magnitude);
    event.setMagnitudeType(magnitudeType);
    event.setIdentifier(id);
    event.setNetwork(network);
    event.setReviewStatus(reviewStatus);
    event.setURL(url);

    Event eventCopy(event);
    EXPECT_NEAR(eventCopy.getLatitude(),   latitude,   1.e-10);
    EXPECT_NEAR(eventCopy.getLongitude(),  longitude,  1.e-10);
    EXPECT_NEAR(eventCopy.getDepth(),      depth,      1.e-10);
    EXPECT_NEAR(eventCopy.getOriginTime(), originTime, 1.e-10);
    EXPECT_NEAR(eventCopy.getMagnitude(),  magnitude,  1.e-10);
    EXPECT_EQ(eventCopy.getIdentifier(), id);
    EXPECT_EQ(eventCopy.getMagnitudeType(), magnitudeType);
    EXPECT_EQ(eventCopy.getReviewStatus(),  reviewStatus);
    EXPECT_EQ(eventCopy.getURL(),           url);
    EXPECT_EQ(eventCopy.getNetwork(), networkRef);

    event.clear();
    EXPECT_FALSE(event.haveLatitude());
    EXPECT_FALSE(event.haveLongitude());
}

}
