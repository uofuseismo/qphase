#ifndef QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEVIEW_HPP
#define QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEVIEW_HPP
#include <memory>
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
    /// @brief Constructor with a given parent.
    explicit TraceView(QWidget *parent = nullptr);

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
