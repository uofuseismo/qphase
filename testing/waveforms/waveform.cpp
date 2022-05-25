#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <limits>
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/segment.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace QPhase::Waveforms;

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

}

