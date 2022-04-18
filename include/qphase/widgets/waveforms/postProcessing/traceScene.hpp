#ifndef QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACESCENE_HPP
#define QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACESCENE_HPP
#include <QGraphicsScene>
#include <memory>
QT_BEGIN_NAMESPACE
 class QGraphicsSceneWheelEvent;
 class QSize;
QT_END_NAMESPACE
namespace QPhase::Widgets::Waveforms::PostProcessing
{
/// @class TraceScene "traceScene.hpp" "qphase/widgets/postProcessing/traceScene.hpp"
/// @brief This is a QGraphicScene that manages TraceItems.  This is for 
///        post-processing waveform visualization which has substantailly more
///        features than real-time waveform visualization.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class TraceScene : public QGraphicsScene
{
public:
    /// @brief Constructs a scene with a given trace width and height. 
    TraceScene(int traceWidth, int traceHeight, QObject *parent = nullptr);

    /// @brief Destructor.
    ~TraceScene() override;

    /// @brief Populates the trace items in the scene.  If there are no items
    ///        then this will just draw the default background.
    void populateScene();
    /// @brief Resizes the current trace scene.
    void resize(const QSize &newSize);

    TraceScene(const TraceScene &) = delete;
    TraceScene(TraceScene &&) noexcept = delete;
    TraceScene& operator=(const TraceScene &) = delete;
    TraceScene& operator=(TraceScene &&) noexcept = delete;
protected:
    /// @brief Handles zoom and horizontal scroll events
    //void wheelEvent(QGraphicsSceneWheelEvent *event) override;
private:
    class TraceSceneImpl;
    std::unique_ptr<TraceSceneImpl> pImpl;    
};
}
#endif