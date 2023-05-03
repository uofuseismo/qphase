#include <QColor>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <limits>
#include "qphase/widgets/colorMaps/parula.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace QPhase::Widgets::ColorMaps;

TEST(WidgetsColorMaps, Parula)
{
    Parula parula;
    const std::pair<double, double> range{-2, 2};
    const QColor c0{ 62,  38, 168, 255};
    const QColor c1{249, 251,  21, 255};
    EXPECT_NO_THROW(parula.initialize(range, InterpolationMethod::Linear));
    EXPECT_TRUE(parula.isInitialized());
    // Edge cases
    EXPECT_EQ(c0, parula.evaluate(range.first));
    EXPECT_EQ(c0, parula.evaluate(range.first - 1));
    EXPECT_EQ(c1, parula.evaluate(range.second));
    EXPECT_EQ(c1, parula.evaluate(range.second + 1));
    EXPECT_EQ(c1, parula.evaluate(range.second - 1.e-10));
    // 0.4 is exactly defined
    auto x40 = range.first + (range.second - range.first)*0.4;
    const QColor c40{28, 170, 223};
    EXPECT_EQ(c40, parula.evaluate(x40));
    // Between [0.9960784, 1)
    auto x999l = range.first + (range.second - range.first)
               *(0.9960784 + 0.499*(1 - 0.9960784));
    auto x999u = range.first + (range.second - range.first)
               *(0.9960784 + 0.501*(1 - 0.9960784));
    QColor c999l{249, 250,  22};
    QColor c999u{249, 250,  21}; 
    EXPECT_EQ(c999l, parula.evaluate(x999l));
    EXPECT_EQ(c999u, parula.evaluate(x999u));
 
    // Do a nearest neighbor interpolation
    EXPECT_NO_THROW(parula.initialize(range, InterpolationMethod::Nearest));
    EXPECT_TRUE(parula.isInitialized());
    // Edge cases
    EXPECT_EQ(c0, parula.evaluate(range.first));
    EXPECT_EQ(c0, parula.evaluate(range.first - 1));
    EXPECT_EQ(c1, parula.evaluate(range.second));
    EXPECT_EQ(c1, parula.evaluate(range.second + 1));
    EXPECT_EQ(c1, parula.evaluate(range.second - 1.e-10));
    EXPECT_EQ(c40, parula.evaluate(x40));
    QColor c999lNearest{249, 249,  22};
    QColor c999uNearest{249, 251,  21};
    EXPECT_EQ(c999lNearest, parula.evaluate(x999l));
    EXPECT_EQ(c999uNearest, parula.evaluate(x999u));
}

}
