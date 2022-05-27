#include <chrono>
#include <QDebug>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QScrollBar>
#include "qphase/widgets/waveforms/stationView.hpp"
#include "qphase/widgets/waveforms/stationScene.hpp"

#define DEFAULT_TRACE_HEIGHT 150

using namespace QPhase::Widgets::Waveforms;

class StationView::StationViewImpl
{
public:
    StationScene *mScene{nullptr};
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    int mMinimumWidth{600};
    int mMinimumHeight{200};
    int mTraceHeight{DEFAULT_TRACE_HEIGHT};
    int mTraceWidth{400};
    TimeConvention mTimeConvention{TimeConvention::Absolute};
};

/// C'tor
StationView::StationView(QWidget *parent) :
    QGraphicsView(parent),
    pImpl(std::make_unique<StationViewImpl> ())
{
    setMinimumSize(pImpl->mMinimumWidth, pImpl->mMinimumHeight);

    auto boundingRect
        = this->mapToScene(this->viewport()->geometry()).boundingRect();
    int traceHeight = DEFAULT_TRACE_HEIGHT;
    int traceWidth  = boundingRect.width();
    
    pImpl->mScene = new StationScene(traceWidth, traceHeight);
    setScene(pImpl->mScene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::LosslessImageRendering);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalScrollBar()->setSliderPosition(1); // Start scrollbar near top
}

/// Destructor
StationView::~StationView() = default;

/// Resize event
void StationView::resizeEvent(QResizeEvent *event)
{
    auto newSize = event->size();
    int innerWidth = newSize.width();
    newSize.setWidth(innerWidth);
    if (pImpl->mScene){pImpl->mScene->resize(newSize);}
    QGraphicsView::resizeEvent(event);
}

/// Time limits
void StationView::setTimeLimits(const std::pair<std::chrono::microseconds,
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
        pImpl->mTimeConvention = TimeConvention::Absolute;
        pImpl->mScene->setAbsoluteTimeLimits(timeLimits);
    }
    catch (const std::exception &e)
    {
        qCritical() << __func__ << "Failed to set time limits: " << e.what();
    }
}

/// Force a redraw
void StationView::redrawScene()
{
    pImpl->mScene->update();
}

