#include <chrono>
#include <QDebug>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QScrollBar>
#include "qphase/widgets/waveforms/postProcessing/traceView.hpp"
#include "qphase/widgets/waveforms/postProcessing/traceScene.hpp"

#define DEFAULT_TRACE_HEIGHT 150

using namespace QPhase::Widgets::Waveforms::PostProcessing;

class TraceView::TraceViewImpl
{
public:
    TraceScene *mScene{nullptr};
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    int mMinimumWidth{600};
    int mMinimumHeight{200};
    int mTraceHeight{DEFAULT_TRACE_HEIGHT};
    int mTraceWidth{400};
    TraceView::TimeConvention mTimeConvention{TraceView::TimeConvention::Absolute};
};

/// C'tor
TraceView::TraceView(QWidget *parent) :
    QGraphicsView(parent),
    pImpl(std::make_unique<TraceViewImpl> ())
{
    setMinimumSize(pImpl->mMinimumWidth, pImpl->mMinimumHeight);

    auto boundingRect
        = this->mapToScene(this->viewport()->geometry()).boundingRect();
    int traceHeight = DEFAULT_TRACE_HEIGHT;
    int traceWidth  = boundingRect.width();
    
    pImpl->mScene = new TraceScene(traceWidth, traceHeight);
    setScene(pImpl->mScene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::LosslessImageRendering);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalScrollBar()->setSliderPosition(1); // Start scrollbar near top
}

/// Destructor
TraceView::~TraceView() = default;

/// Resize event
void TraceView::resizeEvent(QResizeEvent *event)
{
    auto newSize = event->size();
    int innerWidth = newSize.width();
    newSize.setWidth(innerWidth);
    if (pImpl->mScene){pImpl->mScene->resize(newSize);}
    QGraphicsView::resizeEvent(event);
}

/// Time limits
void TraceView::setTimeLimits(const std::pair<std::chrono::microseconds,
                                              std::chrono::microseconds>
                               &timeLimits)
{
    if (pImpl->mScene == nullptr)
    {
        qCritical() << __func__ << "Scene is NULL";
        return;
    }
    try
    {
        pImpl->mTimeConvention = TraceView::TimeConvention::Absolute;
        pImpl->mScene->setAbsoluteTimeLimits(timeLimits);
    }
    catch (const std::exception &e)
    {
        qCritical() << __func__ << "Failed to set time limits: " << e.what();
    }
}
