#ifndef QPHASE_WIDGETS_WAVEFORMS_REAL_TIME_CHANNEL_SCENE_HPP
#define QPHASE_WIDGETS_WAVEFORMS_REAL_TIME_CHANNEL_SCENE_HPP
#include <QGraphicsScene>
#include <memory>
#include "qphase/widgets/waveforms/enums.hpp"
QT_BEGIN_NAMESPACE
 class QGraphicsSceneWheelEvent;
 class QSize;
QT_END_NAMESPACE
namespace QPhase::Waveforms
{
 template<class T> class Channel;
}
namespace QPhase::Widgets::Waveforms::RealTime
{
/// @class ChannelScene "channelScene.hpp" "qphase/widgets/realTime/channelScene.hpp"
/// @brief This is a QGraphicScene that manages channels.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class ChannelScene : public QGraphicsScene
{
public:
    /// @brief Constructs a scene with a given trace width and height. 
    ChannelScene(int traceWidth, int traceHeight, QObject *parent = nullptr);

    /// @brief Destructor.
    ~ChannelScene() override;

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
    /// @brief Sets the channels to plot.
    void setChannels(std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>> &channels);

    ChannelScene(const ChannelScene &) = delete;
    ChannelScene(ChannelScene &&) noexcept = delete;
    ChannelScene& operator=(const ChannelScene &) = delete;
    ChannelScene& operator=(ChannelScene &&) noexcept = delete;
protected:
    /// @brief Handles zoom and horizontal scroll events
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
private:
    /// @brief Redraws channels items in scene.
    void updatePlot();
private:
    class ChannelSceneImpl;
    std::unique_ptr<ChannelSceneImpl> pImpl;    
};
}
#endif
