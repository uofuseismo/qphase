#include <cmath>
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QGraphicsSceneWheelEvent>
#include <QString>
#include "qphase/widgets/waveforms/postProcessing/traceScene.hpp"

using namespace QPhase::Widgets::Waveforms::PostProcessing;

class TraceScene::TraceSceneImpl
{
public:
    TraceSceneImpl(const int traceWidth,
                   const int traceHeight) :
        mTraceWidth(std::max(200, traceWidth)),
        mTraceHeight(std::max(20, traceHeight))
    {
        mBackgroundFont.setStyleHint(QFont::Monospace);
        mBackgroundFont.setPointSize(22);
        mBackgroundFont.setWeight(QFont::Light);
        mBackgroundFont.setItalic(false);
    }

    /// Recomputes the trace height given the current plot size
    void recomputeTraceHeight()
    {
        auto availableHeight = static_cast<double> (mCurrentSize.height());
        int denominator = 1;
        //if (!mChannelList.empty())
        //{
        //    denominator = std::min(static_cast<int> (mChannelList.size()),
        //                           mMaxTracesPerScene);
        //}
        mTraceHeight
            = static_cast<int> (std::floor(availableHeight/denominator));
    }
///private:
    QSize mCurrentSize;
    QColor mBackgroundColor{Qt::white};
    QString mBackgroundName{tr("Trace Viewer")};
    QFont mBackgroundFont;
    double mZoomFactor{1.1};
    int mNumberOfZooms{0};
    int mTraceWidth{400};
    int mTraceHeight{150};
    bool mNormalZoom{true}; // Wheel forward zooms in
    bool mNormalTimeAdvance{true}; // Wheel in goes back in time
};


/// C'tor
TraceScene::TraceScene(const int traceWidth,
                       const int traceHeight,
                       QObject *parent) :
    QGraphicsScene(parent),
    pImpl(std::make_unique<TraceSceneImpl> (traceWidth, traceHeight))
{
    pImpl->mCurrentSize = QSize(width(), height());
    setBackgroundBrush(pImpl->mBackgroundColor);
    populateScene();
}

/// Destructor
TraceScene::~TraceScene() = default;

/// Resize event
void TraceScene::resize(const QSize &newSize)
{
    pImpl->mCurrentSize = newSize;
    pImpl->mTraceWidth = newSize.width();
    pImpl->recomputeTraceHeight();
    populateScene(); // replot
}

/// Populate scene 
void TraceScene::populateScene()
{
    bool drawBackgroundOnly = true;
    if (drawBackgroundOnly)
    {
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
    }
    else
    {

    }
}

/// Wheel event - handles zooming and scrolling left / right
void TraceScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 
    bool updatePlot = false;
    bool handled = false;
    bool haveData = false;
haveData = true;
    // Is there anything to do?
    if (haveData)
    {
        // Zoom in/out
        if (event->modifiers() & Qt::ControlModifier)
        {
            handled = true;
            // Are we zooming in or out?
            bool zoomIn = (event->delta() > 0);
            if (!pImpl->mNormalZoom){zoomIn = !zoomIn;}
            // Handle zoom in
            if (zoomIn)
            {
                qDebug() << "Zooming in";
                pImpl->mNumberOfZooms = pImpl->mNumberOfZooms + 1;
                updatePlot = true;
            }
            // Handle zoom out
            else if (!zoomIn && pImpl->mNumberOfZooms > 0)
            {
                qDebug() << "Zooming out";
                pImpl->mNumberOfZooms = pImpl->mNumberOfZooms - 1;
                updatePlot = true;
            }
        }
        // Scroll forward/backward in time
        else if (event->modifiers() & Qt::ShiftModifier)
        {
            handled = true;
            // Are we panning left or right?
            bool advanceTime = (event->delta() < 0);
            if (!pImpl->mNormalTimeAdvance){advanceTime = !advanceTime;}
            // Move forward in time
            if (advanceTime)
            {
                qDebug() << "Moving to earlier time";
            }
            else
            {
                qDebug() << "Moving to later time";
            }
        }
    }
    // Actually update the plot?
    if (updatePlot)
    {
    }
    // Was the event handled?
    event->ignore();
    if (handled){event->accept();}
}
