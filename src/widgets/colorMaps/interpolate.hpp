#ifndef QPHASE_COLOR_MAPS_INTERPOLATE_HPP
#define QPHASE_COLOR_MAPS_INTERPOLATE_HPP
#include <QColor>
#include <cmath>
#ifndef NDEBUG
#include <cassert>
#endif
namespace
{
[[nodiscard]]
int interpolate(const double x0, const double x1,
                const int y0, const int y1,
                const double x)
{
    auto dx = x1 - x0;
    auto yi = static_cast<int> (std::round(y0 + ((x - x0)/dx)*(y1 - y0)));
    yi = std::min(255, std::max(0, yi));
    return yi;
}

[[nodiscard]]
QColor interpolate(const double value,
                   const std::pair<double, QColor> &c0,
                   const std::pair<double, QColor> &c1)
{
    if (value <= c0.first){return c0.second;}
    if (value >= c1.first){return c1.second;}
    auto ri = ::interpolate(c0.first, c1.first,
                            c0.second.red(),   c1.second.red(),
                            value);
    auto gi = ::interpolate(c0.first, c1.first,
                            c0.second.green(), c1.second.green(),
                            value);
    auto bi = ::interpolate(c0.first, c1.first,
                            c0.second.blue(),  c1.second.blue(),
                            value);
    return QColor{ri, gi, bi, 255};
}

[[nodiscard]]
QColor interpolate(const double value,
                   const int n,
                   const std::pair<double, QColor> *colorMap,
                   const QPhase::Widgets::ColorMaps::InterpolationMethod method
                      = QPhase::Widgets::ColorMaps::InterpolationMethod::Linear)
{
#ifndef NDEBUG
    assert(n > 1);
#endif
    auto v0 = colorMap[0].first; 
    if (value <= v0 || n == 1){return colorMap[0].second;}
    auto v1 = colorMap[n - 1].first;
    if (value >= v1){return colorMap[n-1].second;}

    auto upperBound = std::upper_bound(colorMap, colorMap + n, value,
                                       [](const double lhs,
                                          const std::pair<double, QColor> &rhs)
                                       {
                                           return lhs < rhs.first;
                                       });
    auto i0 = std::max(0, static_cast<int> (upperBound - colorMap) - 1);
    if (i0 == n - 1){i0 = i0 - 1;}
    if (i0 < 0){i0 = 0;}
#ifndef NDEBUG
    assert(i0 >= 0 && i0 < n - 1);
    assert(value >= colorMap[i0].first);
    assert(value <  colorMap[i0 + 1].first);
#endif
    if (method == QPhase::Widgets::ColorMaps::InterpolationMethod::Nearest)
    {
        v0 = colorMap[i0].first;
        v1 = colorMap[i0 + 1].first;
        if (std::abs(value - v0) < std::abs(value - v1))
        {
            return colorMap[i0].second;
        }
        else
        {
            return colorMap[i0 + 1].second;
        } 
    }
    else
    {
        return ::interpolate(value, colorMap[i0], colorMap[i0 + 1]);
    }
}

[[nodiscard]]
QColor interpolate(const double value, const std::pair<double, double> &range,
                   const int n,
                   const std::pair<double, QColor> *colorMap,
                   const QPhase::Widgets::ColorMaps::InterpolationMethod method
                      = QPhase::Widgets::ColorMaps::InterpolationMethod::Linear)
{                    
    // Transform to [0,1]
    // y = y0 + (x - x0)*(y1 - y0)/(x1 - x0) 
    //   =  0 + (x - x0)*( 1 - 0 )/(x1 - x0)
    //   =  (x - x0)/(x1 - x0)  
    double value01 = (value - range.first)/(range.second - range.first);
    return ::interpolate(value01, n, colorMap, method);
}

}
#endif
