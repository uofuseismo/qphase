#ifndef QPHASE_WIDGETS_WAVEFORMS_STATION_SCENE_HPP
#define QPHASE_WIDGETS_WAVEFORMS_STATION_SCENE_HPP
#include <QGraphicsScene>
#include <memory>
#include "qphase/widgets/waveforms/enums.hpp"
QT_BEGIN_NAMESPACE
 class QGraphicsSceneWheelEvent;
 class QSize;
QT_END_NAMESPACE
namespace QPhase::Waveforms
{
 template<class T> class Station;
}
namespace QPhase::Widgets::Waveforms
{
/// @class StationScene "stationScene.hpp" "qphase/widgets/stationScene.hpp"
/// @brief This is a QGraphicScene that manages stations.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class StationScene : public QGraphicsScene
{
public:
    /// @brief Constructs a scene with a given trace width and height. 
    StationScene(int traceWidth, int traceHeight, QObject *parent = nullptr);

    /// @brief Destructor.
    ~StationScene() override;

    /// @brief Populates the trace items in the scene.  If there are no items
    ///        then this will just draw the default background.
    void populateScene();
    /// @brief Resizes the current trace scene.
    void resize(const QSize &newSize);
    /// @brief Sets the time limits.
    /// @param[in] plotLimits  plotLimits.first is the start time (UTC) in
    ///                        microseconds since the epoch of the plot while
    ///                        plotLimits.second is the end time (UTC) in
    ///                        microseconds since the epoch of the plot.
    void setAbsoluteTimeLimits(const std::pair<std::chrono::microseconds,
                                               std::chrono::microseconds> &plotLimits);
    /// @brief Sets the time about which to plot relative.
    /// @param[in] time  The time in UTC seconds since the epoch.  
    ///                  This will represent the "zero" time.
    void setRelativeTimeLimits(const std::chrono::microseconds &time);
    /// @brief Sets the stations to plot.
    void setStations(std::shared_ptr<std::vector<QPhase::Waveforms::Station<double>>> &stations);
    void redrawWaveforms();

    StationScene(const StationScene &) = delete;
    StationScene(StationScene &&) noexcept = delete;
    StationScene& operator=(const StationScene &) = delete;
    StationScene& operator=(StationScene &&) noexcept = delete;
protected:
    /// @brief Handles zoom and horizontal scroll events
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
private:
    /// @brief Redraws station items in scene.
    void updatePlot();
private:
    class StationSceneImpl;
    std::unique_ptr<StationSceneImpl> pImpl;    
};
}
#endif
