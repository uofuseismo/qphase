#include <QDebug>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QScrollBar>
#include "qphase/widgets/waveforms/postProcessing/traceView.hpp"
#include "qphase/widgets/waveforms/postProcessing/traceScene.hpp"

using namespace QPhase::Widgets::Waveforms::PostProcessing;

class TraceView::TraceViewImpl
{
public:
    TraceScene *mScene{nullptr};
    int mMinimumWidth{600};
    int mMinimumHeight{200};
    int mTraceHeight{150};
    int mTraceWidth{400};
};

/// C'tor
TraceView::TraceView(QWidget *parent) :
    QGraphicsView(parent),
    pImpl(std::make_unique<TraceViewImpl> ())
{
    setMinimumSize(pImpl->mMinimumWidth, pImpl->mMinimumHeight);

    auto boundingRect
        = this->mapToScene(this->viewport()->geometry()).boundingRect();
    int traceHeight = 150;
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

