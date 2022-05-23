#ifndef QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEVIEW_HPP
#define QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEVIEW_HPP
#include <memory>
#include <chrono>
#include <QGraphicsView>
QT_BEGIN_NAMESPACE
 class QResizeEvent;
QT_END_NAMESPACE
namespace QPhase::Widgets::Waveforms::PostProcessing
{
 class TraceScene;
}
namespace QPhase::Widgets::Waveforms::PostProcessing
{
/// @class TraceView "traceView.hpp" "qphase/widgets/postProcessing/traceView.hpp"
/// @brief This is a QGraphicView that manages a TraceView.  This is for
///        post-processing waveform visualization which has substantailly more
///        features than real-time waveform visualization.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class TraceView : public QGraphicsView
{
    Q_OBJECT;
public:
    /// @brief Defines the time convention for the plot. 
    enum class TimeConvention
    {
        Absolute,  /*!< Absolute time. */
        Relative   /*!< All time is relative to a specified time. */
    };
public:
    /// @brief Constructor with a given parent.
    explicit TraceView(QWidget *parent = nullptr);

    /// @brief Sets the plot's time conventions.
    void setTimeConvention(const TraceView::TimeConvention convention) noexcept;
    /// @brief Sets the plot's temporal limits.
    /// @param[in] plotLimits  plotLimits.first is the start time of the plot
    ///                        while plotLimits.second is the end time of the
    ///                        plot.  
    void setTimeLimits(const std::pair<std::chrono::microseconds, std::chrono::microseconds> &plotLimits);

    /// @brief Destructor.
    ~TraceView() override;
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    class TraceViewImpl;
    std::unique_ptr<TraceViewImpl> pImpl;
};
}
#endif
