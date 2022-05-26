#ifndef QPHASE_WIDGETS_WAVEFORMS_TRACEVIEW_HPP
#define QPHASE_WIDGETS_WAVEFORMS_TRACEVIEW_HPP
#include <memory>
#include <chrono>
#include <QGraphicsView>
#include "qphase/widgets/waveforms/enums.hpp"
QT_BEGIN_NAMESPACE
 class QResizeEvent;
QT_END_NAMESPACE
namespace QPhase::Widgets::Waveforms
{
 class StationScene;
 class StationItem;
}
namespace QPhase::Widgets::Waveforms
{
/// @class StationView "stationView.hpp" "qphase/widgets/stationView.hpp"
/// @brief This is a QGraphicView that manages a StationScene.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class StationView : public QGraphicsView
{
    Q_OBJECT;
public:
    /// @brief Constructor with a given parent.
    explicit StationView(QWidget *parent = nullptr);

    /// @brief Sets the plot's time conventions.
    void setTimeConvention(const TimeConvention convention) noexcept;
    /// @brief Sets the plot's temporal limits.
    /// @param[in] plotLimits  plotLimits.first is the start time of the plot
    ///                        while plotLimits.second is the end time of the
    ///                        plot.  
    void setTimeLimits(const std::pair<std::chrono::microseconds, std::chrono::microseconds> &plotLimits);

    /// @brief Forces the scene to be redrawn.
    void redrawScene();
    /// @brief Destructor.
    ~StationView() override;
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    class StationViewImpl;
    std::unique_ptr<StationViewImpl> pImpl;
};
}
#endif
