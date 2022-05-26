#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <limits>
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/segment.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/simpleResponse.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace QPhase::Waveforms;

TEST(SimpleResponse, SimpleResponse)
{
    SimpleResponse response;
    double scalar{10};
    std::chrono::microseconds start{0};
    std::chrono::microseconds end{10};
    Units input = Units::Velocity;
    Units output = Units::Counts;
    response.setScalar(scalar);
    response.setInputUnits(input);
    response.setOutputUnits(output);
    response.setStartAndEndTime(std::pair{start, end});
 
    auto responseCopy = response;
    EXPECT_NEAR(responseCopy.getScalar(), scalar, 1.e-14);
    EXPECT_EQ(responseCopy.getInputUnits(), input);
    EXPECT_EQ(responseCopy.getOutputUnits(), output);
    EXPECT_EQ(responseCopy.getStartTime(), start);
    EXPECT_EQ(responseCopy.getEndTime(), end);
}

//----------------------------------------------------------------------------//

template<typename T>
bool operator==(const Segment<T> &lhs, const Segment<T> &rhs)
{
    if (lhs.getStartTime() != rhs.getStartTime()){return false;}
    if (lhs.getEndTime()   != rhs.getEndTime()){return false;}
    if (lhs.getSamplingPeriodInMicroSeconds() !=
       rhs.getSamplingPeriodInMicroSeconds())
    {
       return false;
    }
    if (lhs.getNumberOfSamples() != rhs.getNumberOfSamples()){return false;}
    auto t1 = lhs.getData();
    auto t2 = rhs.getData();
    if (t1.size() != t2.size()){return false;}
    for (int i = 0; i < static_cast<int> (t1.size()); ++i)
    {
        if (static_cast<double> (std::abs(t1[i] - t2[i])) > 1.e-14)
        {
            return false;
        }
    }
    return true;
}

using MyTypes = ::testing::Types<double, float>;

template<class T>
class SegmentTest : public testing::Test
{
public:
    std::vector<double> timeSeries{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double startTime = 1628803598; 
    std::chrono::microseconds startTimeMuS{1628803598000000};
    std::chrono::microseconds endTimeMuS{1628803598000000 + 225000}; // 225000 = std::round(9./40*1000000); 
    double samplingRate = 40;
    std::chrono::microseconds samplingPeriodMuS{25000};
    double tol = std::numeric_limits<double>::epsilon();
protected:
    SegmentTest() :
        segment(std::make_unique<Segment<T>> ())
    {
    }
    ~SegmentTest() override = default;
    std::unique_ptr<Segment<T>> segment;
};

TYPED_TEST_SUITE(SegmentTest, MyTypes);

TYPED_TEST(SegmentTest, Segment)
{
    auto timeSeries = this->timeSeries;
    auto startTime = this->startTime;
    auto startTimeMuS = this->startTimeMuS;
    auto endTimeMuS   = this->endTimeMuS;
    auto samplingRate = this->samplingRate;
    auto samplingPeriodMuS = this->samplingPeriodMuS;
    auto tol = this->tol;
    this->segment->setStartTime(startTime);
    this->segment->setSamplingRate(samplingRate);
    this->segment->setData(timeSeries);
    // Test a copy (this implicitly tests the copy assignment and
    // the original class)
    auto segmentCopy = *this->segment;
    // Verify 
    EXPECT_EQ(segmentCopy.getStartTime(), startTimeMuS);
    EXPECT_NEAR(segmentCopy.getSamplingRate(), samplingRate, tol);
    EXPECT_EQ(segmentCopy.getSamplingPeriodInMicroSeconds(), samplingPeriodMuS);
    EXPECT_EQ(segmentCopy.getNumberOfSamples(),
              static_cast<int> (timeSeries.size()));
    EXPECT_EQ(segmentCopy.getEndTime(), endTimeMuS);
    auto traceBack = segmentCopy.getData();
    auto tracePtr  = segmentCopy.getDataPointer();
    EXPECT_EQ(traceBack.size(), timeSeries.size());
    for (int i = 0; i < static_cast<int> (traceBack.size()); ++i)
    {
        auto res = static_cast<double> (traceBack[i] - timeSeries[i]);
        EXPECT_NEAR(res, 0, tol);
        res = static_cast<double> (tracePtr[i] - timeSeries[i]);
        EXPECT_NEAR(res, 0, tol);
    }
    // Clear it
    segmentCopy.clear();
    EXPECT_EQ(segmentCopy.getNumberOfSamples(), 0);
}

//----------------------------------------------------------------------------//

template<class T>
class WaveformTest : public testing::Test
{
public:
    std::vector<double> timeSeries1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> timeSeries2{10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    double startTime = 1628803598;
    std::chrono::microseconds startTimeMuS1{1628803598000000};
    std::chrono::microseconds endTimeMuS1{1628803598000000 + 225000};
    std::chrono::microseconds startTimeMus2{1628803598000000 + 250000};
    std::chrono::microseconds endTimeMuS2{1628803598000000 + + 250000 + 200000};
    double samplingRate1 = 40;
    double samplingRate2 = 50;
    std::chrono::microseconds samplingPeriodMuS1{25000};
    std::chrono::microseconds samplignPeriodMuS2{20000};
    double tol = std::numeric_limits<double>::epsilon();
    std::vector<Segment<T>> segments;
    std::vector<Segment<T>> reverseSegments;
protected:
    WaveformTest() :
        waveform(std::make_unique<Waveform<T>> ()) 
    {   
        Segment<T> segment1;
        segment1.setStartTime(startTime);
        segment1.setSamplingRate(samplingRate1);
        segment1.setData(timeSeries1);
       
        Segment<T> segment2;
        segment2.setStartTime(startTime + timeSeries1.size()/samplingRate1);
        segment2.setSamplingRate(samplingRate2);
        segment2.setData(timeSeries2);

        segments.push_back(segment1);
        segments.push_back(segment2);

        reverseSegments.push_back(segment2);
        reverseSegments.push_back(segment1);
    }   
    ~WaveformTest() override = default;
    std::unique_ptr<Waveform<T>> waveform;
};

TYPED_TEST_SUITE(WaveformTest, MyTypes);

TYPED_TEST(WaveformTest, Waveform)
{
    auto segments = this->segments;
    auto reverseSegments = this->reverseSegments;
    EXPECT_NO_THROW(this->waveform->setSegments(segments.at(0)));

    auto waveformCopy = *this->waveform;
    EXPECT_EQ(waveformCopy.getNumberOfSegments(), 1);
    auto segmentsBack = waveformCopy.getSegments();
    EXPECT_TRUE(segments[0] == segmentsBack[0]);
    EXPECT_EQ(segments[0].getStartTime(), waveformCopy.getEarliestTime());
    EXPECT_EQ(segments[0].getEndTime(),   waveformCopy.getLatestTime());

    waveformCopy.clear();

    for (int job = 0; job < 2; ++job)
    {
        if (job == 0)
        {
            waveformCopy.setSegments(segments);
        }
        else
        {
            waveformCopy.setSegments(reverseSegments);
        }
        EXPECT_EQ(waveformCopy.getNumberOfSegments(), 2);
        EXPECT_EQ(waveformCopy.getCumulativeNumberOfSamples(), 21);
        EXPECT_EQ(segments[0].getStartTime(), waveformCopy.getEarliestTime());
        EXPECT_EQ(segments[1].getEndTime(),   waveformCopy.getLatestTime());
        segmentsBack = waveformCopy.getSegments();
        int is = 0;
        for (const auto &s : waveformCopy)
        {
            EXPECT_TRUE(segments[is] == s);
            is = is + 1;
        } 
    }
}


//----------------------------------------------------------------------------//

template<class T>
class ChannelTest : public testing::Test
{
public:
    SimpleResponse simpleResponse;
    Waveform<T> waveform;
    double samplingRate{100};
    std::vector<double> timeSeries{1, 2};
    std::string channelCode{"HHZ"};
    double azimuth{0};
    double dip{-90};
protected:
    ChannelTest() :
        channel(std::make_unique<Channel<T>> ()) 
    {
        Segment<T> segment;
        segment.setStartTime(std::chrono::microseconds{45});
        segment.setSamplingRate(samplingRate);
        segment.setData(timeSeries);
        waveform.setSegments(segment);

        std::chrono::microseconds start{40};
        std::chrono::microseconds end{50};
        simpleResponse.setScalar(1);
        simpleResponse.setInputUnits(Units::Velocity);
        simpleResponse.setOutputUnits(Units::Counts);
        simpleResponse.setStartAndEndTime(std::pair(start, end));
    }
    ~ChannelTest() override = default;
    std::unique_ptr<Channel<T>> channel;

};

TYPED_TEST_SUITE(ChannelTest, MyTypes);

TYPED_TEST(ChannelTest, Channel)
{
    auto channel = this->channelCode;
    auto dip = this->dip;
    auto azimuth = this->azimuth;
    auto simpleResponse = this->simpleResponse;
    auto waveform = this->waveform;
    // Set stuff
    EXPECT_NO_THROW(this->channel->setChannelCode(channel));
    EXPECT_NO_THROW(this->channel->setDip(dip));
    EXPECT_NO_THROW(this->channel->setAzimuth(azimuth));
    EXPECT_NO_THROW(this->channel->setWaveform(waveform));
    EXPECT_FALSE(this->channel->haveSimpleResponse());
    EXPECT_NO_THROW(this->channel->setSimpleResponse(simpleResponse));
    // Copy and verify
    auto channelCopy = *this->channel;
    // Verify 
    EXPECT_EQ(channelCopy.getChannelCode(),  channel);
    EXPECT_NEAR(channelCopy.getDip(),     dip,     1.e-14);
    EXPECT_NEAR(channelCopy.getAzimuth(), azimuth, 1.e-14);

    EXPECT_NEAR(channelCopy.getSimpleResponse().getScalar(),
                simpleResponse.getScalar(), 1.e-14);
    EXPECT_EQ(channelCopy.getSimpleResponse().getInputUnits(),
              simpleResponse.getInputUnits());
    EXPECT_EQ(channelCopy.getSimpleResponse().getOutputUnits(),
              simpleResponse.getOutputUnits());
    EXPECT_EQ(channelCopy.getWaveform().getNumberOfSegments(),
              waveform.getNumberOfSegments());
}

}

