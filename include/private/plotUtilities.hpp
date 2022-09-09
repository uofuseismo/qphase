#ifndef PRIVATE_PLOTUTILITIES_HPP
#define PRIVATE_PLOTUTILITIES_HPP
#include <iomanip>
#include <cmath>
#include <algorithm>
#ifndef NDEBUG
#include <cassert>
#endif
#include <QLine>
#include <QVector>
#include "qphase/waveforms/segment.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/channel.hpp"
namespace
{
/// @brief Transforms
///        \f[
///            y \in [y_{min}, y_{max}] \rightarrow \tilde{y} \in [a,b]
///        \f]
/// @param[in] y      The y to transfrom from [yMin, yMax] to [a, b].
/// @param[in] a      The lower bound of the target range.
/// @param[in] b      The upper bound of the target range.
/// @param[in] yMin   The minimum observed y value.
/// @param[in] dyInv  1/(max(y) - min(y))
/// @result The transformed y in the range [a, b].
template<typename T>
[[nodiscard]]
qreal minMaxRescale(const T y, const T a, const T b, const T yMin,
                    const T dyInv)
{
    return static_cast<qreal> (a + ((y - yMin)*(b - a))*dyInv);
}
/// @brief Transforms
///        \f[
///             y \in [y_{min}, y_{max}] \rightarrow \tilde{y} \in [-1,1]
///        \f]
/// @param[in] y      The y to transfrom from [yMin, yMax] to [-1, 1].
/// @param[in] yMin   The minimum observed y value.
/// @param[in] dyInv  1/(max(y) - min(y))
/// @result The transformed y in the range [-1, 1].
/*
template<typename T> 
[[nodiscard]] [[maybe_unused]]
qreal minMaxRescale(const T y, const T yMin, const T dyInv)
{
    constexpr T zero = 0;
    constexpr T one = 1;
    return minMaxRescale(y, -one, one, yMin, dyInv);
}
*/

/// @brief Gets the minimum and maximum for plotting in the plotting window.
/// @param[in] plotT0          The start time of plot in UTC seconds since
///                            the epoch.
/// @param[in] plotT1          The end time of the plot in UTC seconds since
///                            the epoch.
/// @param[in] samplingPeriod  The sampling period of the signal in seconds.
/// @param[in] startTime       The start time of the trace in UTC seconds since
///                            the epoch.
/// @param[in] signal          The signal from which to obtain the min/max.
template<typename T>
[[nodiscard]]
std::pair<T, T> getMinMaxForPlotting(const double plotT0,
                                     const double plotT1,
                                     const double samplingPeriod,
                                     const double startTime,
                                     const int nSamples,
                                     const T *__restrict__ signal)
{
    // Figure out min/max for all packets
    auto smallestValue = std::numeric_limits<T>::max();
    auto largestValue  = std::numeric_limits<T>::lowest();
    if (nSamples < 1){return std::pair(smallestValue, largestValue);}
    if (nSamples > 0 && signal == nullptr)
    {
        throw std::invalid_argument("Signal is NULL");
    }
    auto traceT0 = startTime;
    auto traceT1 = startTime + (nSamples - 1)*samplingPeriod;
    // Trace wholly contained in plot
    if (traceT0 >= plotT0 && traceT1 <= plotT1)
    {
        auto [vMin, vMax] = std::minmax_element(signal, signal + nSamples);
        smallestValue = std::min(smallestValue, *vMin);
        largestValue  = std::max(largestValue,  *vMax);
    }
    else
    {
       auto i1 = static_cast<int> ( std::floor( (plotT0 - traceT0)
                                                /samplingPeriod) );
       i1 = std::max(0, i1);
       auto i2 = static_cast<int> ( std::ceil( (plotT1 - traceT0)
                                               /samplingPeriod) ) + 1;
       i2 = std::min(nSamples, i2);
       for (int i = i1; i < i2; ++i)
       {
            smallestValue = std::min(smallestValue, signal[i]);
            largestValue  = std::max(largestValue,  signal[i]);
       }
    }
    return std::pair(smallestValue, largestValue);
}

template<typename T>
[[nodiscard]]
std::pair<T, T> getMinMaxForPlotting(const QPhase::Waveforms::Segment<T> &segment,
                                     const std::chrono::microseconds &plotT0MuS,
                                     const std::chrono::microseconds &plotT1MuS)
{
    double plotT0 = plotT0MuS.count()*1.e-6;
    double plotT1 = plotT1MuS.count()*1.e-6;
    double traceT0 = segment.getStartTime().count()*1.e-6;
    double dt = segment.getSamplingPeriod();
    auto nSamples = segment.getNumberOfSamples();
    const auto signal = segment.getDataPointer();
    return getMinMaxForPlotting(plotT0, plotT1, dt, traceT0, nSamples, signal);
}

template<typename T>
[[nodiscard]] [[maybe_unused]]
std::pair<T, T> getMinMaxForPlotting(const QPhase::Waveforms::Channel<T> &channel,
                                     const std::chrono::microseconds &plotT0MuS,
                                     const std::chrono::microseconds &plotT1MuS)
{
    T vMin = std::numeric_limits<T>::max();
    T vMax = std::numeric_limits<T>::lowest();
    auto waveform = channel.getWaveformReference();
    for (const auto &segment : waveform)
    {
        auto [v0, v1] = getMinMaxForPlotting(segment, plotT0MuS, plotT1MuS);
        vMin = std::min(vMin, v0);
        vMax = std::max(vMax, v1);
    }
    return std::pair(vMin, vMax);
}


/// @brief Creates the lines comprising a waveform.
/// @param[in] plotT0          The start time of the plot in seconds.
/// @param[in] plotT1          The end time of the plot in seconds.
/// @param[in] traceT0         The start time of the trace.
/// @param[in] traceT1         The end time of the trace.
/// @param[in] dt              The sampling rate in seconds.
/// @param[in] nSamples        The number of samples in the signal.
/// @param[in] signal          The signal to plot.  This is an array whose
///                            dimension is [nSamples].
/// @param[in] plotWidth       The number of horizontal pixels.
/// @param[in] plotHeight      The number of vertical pixels.
/// @param[in] heightFraction  The height fraction.  For example, if this is 
///                            0.95 then the absolute maximum amplitude will
///                            go 95 pct of the way to the plot min/max
///                            available vertical space.
/// @param[in] range           This forces the plot to be in a custom range.
template<typename T>
QVector<QLineF> createLines(const double plotT0, const double plotT1,
                            const double traceT0, const double traceT1,
                            const double dt,
                            const int nSamples, const T *__restrict__ signal,
                            const qreal plotWidth,
                            const qreal plotHeight,
                            const qreal heightFraction = 0.95,
                            const std::pair<T, T> *range = nullptr)
{
    constexpr qreal qZero = 0;
    QVector<QLineF> lines;
    // No data or plot isn't in the window
    if (plotT0 > traceT1 || plotT1 < traceT0 || dt == 0 || nSamples < 1)
    {
        return lines;
    }
    if (signal == nullptr){throw std::runtime_error("Signal is NULL");} 
    auto nPixels = static_cast<int> (plotWidth);
    if (nPixels < 1)
    {
        qWarning() << "Horizontal pixels is zero - setting to 1";
        nPixels = nPixels + 1;
    }
    // Get the parts of the trace that will be plotted
    int traceStartIndex = 0;
    if (plotT0 > traceT0)
    {
        traceStartIndex = static_cast<int> ( (plotT0 - traceT0)/dt );
#ifndef NDEBUG
        assert(traceStartIndex >= 0);
#endif
    }
    int traceEndIndex = nSamples;
    if (plotT1 < traceT1)
    {
        traceEndIndex = static_cast<int> ( (plotT1 - traceT0)/dt ) + 1;
        traceEndIndex = std::min(traceEndIndex, nSamples);
    }
#ifndef NDEBUG
    assert(traceStartIndex <= traceEndIndex);
#endif
    auto qPlotT0 = static_cast<qreal> (plotT0);
    auto qPlotInv = static_cast<qreal> (plotT1 - plotT0);
    if (qPlotInv > qZero){qPlotInv = 1/qPlotInv;}
    // Get the min/max signal amplitude in the range of interest
    qreal sMin = 0;
    qreal sMax = 1;
    if (range == nullptr)
    {
        auto [sMinPtr, sMaxPtr] = std::minmax_element(signal + traceStartIndex,
                                                      signal + traceEndIndex);
        sMin = static_cast<qreal> (*sMinPtr);
        sMax = static_cast<qreal> (*sMaxPtr);
    }
    else
    {
        sMin = range->first;
        sMax = range->second;
    }
    auto ds = sMax - sMin;
    auto dsInv = qZero;
    if (ds > 0){dsInv = 1/ds;}

    auto gapBetweenVerticalEdge = (1 - heightFraction)*plotHeight/2;
    auto heightMin = gapBetweenVerticalEdge;
    auto heightMax = plotHeight - gapBetweenVerticalEdge;
    // Draw the lines
    auto nLinesPlot = traceEndIndex - traceStartIndex - 1;
    auto step = static_cast<double> (nLinesPlot + 1)/nPixels;
    // Decimate
    if (step > 5)
    {
        lines.resize(nPixels);
        auto linesPtr = lines.data();
        for (int l = 0; l < nPixels; ++l)
        {
            int i1 = traceStartIndex
                   + static_cast<int> (std::round(l*step));
            int i2 = traceStartIndex
                   + static_cast<int> (std::round((l + 1)*step));
            if (l == 0){i1 = traceStartIndex;}
            if (l == nPixels - 1){i2 = traceEndIndex;}
            int j1 = i2;
            int j2 = traceStartIndex
                   + static_cast<int> (std::round((l + 2)*step));
            if (l == nPixels - 1)
            {
                j1 = i1;
                j2 = traceEndIndex;
            }
            auto sampleTime1 = static_cast<qreal> (traceT0 + i1*dt);
            auto t1 = minMaxRescale(sampleTime1, qZero, plotWidth,
                                    qPlotT0, qPlotInv);
            const auto [signalMin, signalMax]
                = std::minmax_element(signal + i1, signal + i2);
            const auto [nextSignalMin, nextSignalMax]
                = std::minmax_element(signal + j1, signal + j2);
            // Reverse target min/max so as to flip y (i.e., +y is plotted up)
            auto s1 = minMaxRescale(static_cast<qreal> (*signalMin),
                                    heightMax, heightMin,
                                    sMin, dsInv);
            auto s2 = minMaxRescale(static_cast<qreal> (*signalMax),
                                    heightMax, heightMin,
                                    sMin, dsInv);
            auto s1Next = minMaxRescale(static_cast<qreal> (*nextSignalMin),
                                        heightMax, heightMin,
                                        sMin, dsInv);
            auto s2Next = minMaxRescale(static_cast<qreal> (*nextSignalMax),
                                        heightMax, heightMin,
                                        sMin, dsInv);
            linesPtr[l].setLine(t1, std::max(s1, s1Next),
                                t1, std::min(s2, s2Next));
        }
    }
    else // Do not decimate
    {
        lines.resize(std::max(0, nLinesPlot));
        auto linesPtr = lines.data();
        for (int it = traceStartIndex; it < traceEndIndex - 1; ++it)
        {
            auto sampleTime1 = static_cast<qreal> (traceT0 + it*dt);
            auto sampleTime2 = static_cast<qreal> (traceT0 + (it + 1)*dt);
            auto t1 = minMaxRescale(sampleTime1, qZero, plotWidth,
                                    qPlotT0, qPlotInv);
            auto t2 = minMaxRescale(sampleTime2, qZero, plotWidth,
                                    qPlotT0, qPlotInv);
            // Reverse target min/max so as to flip y (i.e., +y is plotted up)
            auto s1 = minMaxRescale(static_cast<qreal> (signal[it]),
                                    heightMax, heightMin,
                                    sMin, dsInv);
            auto s2 = minMaxRescale(static_cast<qreal> (signal[it+1]),
                                    heightMax, heightMin,
                                    sMin, dsInv);
            linesPtr[it - traceStartIndex].setLine(t1, s1, t2, s2);
        }
    }
    return lines;
}
/// @brief Creates the lines comprising a waveform segment.
/// @param[in] plotT0          The start time of the plot in microseconds.
/// @param[in] plotT1          The end time of the plot in microseconds.
/// @param[in] plotWidth       The number of horizontal pixels.
/// @param[in] plotHeight      The number of vertical pixels.
/// @param[in] heightFraction  The height fraction.  For example, if this is 
///                            0.95 then the absolute maximum amplitude will
///                            go 95 pct of the way to the plot min/max
///                            available vertical space.
/// @param[in] range           This forces the plot to be in a custom range.
template<typename T>
QVector<QLineF> createLines(const QPhase::Waveforms::Segment<T> &segment,
                            const std::chrono::microseconds &plotT0MuS,
                            const std::chrono::microseconds &plotT1MuS,
                            const qreal plotWidth,
                            const qreal plotHeight,
                            const qreal heightFraction = 0.95,
                            const std::pair<T, T> *range = nullptr)
{
    QVector<QLineF> lines;
    double plotT0 = plotT0MuS.count()*1.e-6;
    double plotT1 = plotT1MuS.count()*1.e-6;
    double traceT0 = segment.getStartTime().count()*1.e-6;
    double traceT1 = segment.getEndTime().count()*1.e-6;
/*
std::cout << "faking it:" << " " << plotT0 << " " << plotT1 << std::endl;
plotT0= traceT0;
plotT1= traceT1;
std::cout << std::setprecision(16) << plotT0 << " " << plotT1 << " " << traceT0 << " " << traceT1 << std::endl;
*/
    double dt = segment.getSamplingPeriod();
    auto nSamples = segment.getNumberOfSamples(); 
    const auto signal = segment.getDataPointer();
    lines = createLines(plotT0, plotT1,
                        traceT0, traceT1,
                        dt, nSamples, signal,
                        plotWidth, plotHeight,
                        heightFraction,
                        range);
    return lines;
}
/// @brief Creates the lines comprising a waveform segment.
/// @param[in] plotT0          The start time of the plot in microseconds.
/// @param[in] plotT1          The end time of the plot in microseconds.
/// @param[in] plotWidth       The number of horizontal pixels.
/// @param[in] plotHeight      The number of vertical pixels.
/// @param[in] heightFraction  The height fraction.  For example, if this is 
///                            0.95 then the absolute maximum amplitude will
///                            go 95 pct of the way to the plot min/max
///                            available vertical space.
/// @param[in] range           This forces the plot to be in a custom range.
template<typename T>
QVector<QVector<QLineF>>
    createLines(const QPhase::Waveforms::Channel<T> &channel,
                const std::chrono::microseconds &plotT0MuS,
                const std::chrono::microseconds &plotT1MuS,
                const qreal plotWidth,
                const qreal plotHeight,
                const qreal heightFraction = 0.95,
                const std::pair<T, T> *range = nullptr)
{
    QVector<QVector<QLineF>> lines;
    auto waveform = channel.getWaveformReference();
    lines.reserve(waveform.getNumberOfSegments());
    for (const auto &segment : waveform)
    {
        try
        {
            auto linesForSegment = createLines<T>(segment,
                                                  plotT0MuS, plotT1MuS,
                                                  plotWidth, plotHeight,
                                                  heightFraction,
                                                  range);
            lines.push_back(std::move(linesForSegment));
        }
        catch (const std::exception &e)
        {
            qWarning() << e.what();
        }
    }
    return lines;
}

}
#endif
