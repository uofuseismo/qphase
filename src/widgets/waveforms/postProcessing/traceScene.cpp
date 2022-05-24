#include <cmath>
#include <vector>
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QGraphicsSceneWheelEvent>
#include <QString>
#include "qphase/widgets/waveforms/postProcessing/traceScene.hpp"
#include "qphase/widgets/waveforms/postProcessing/traceItem.hpp"
#include "qphase/waveforms/waveform.hpp"

using namespace QPhase::Widgets::Waveforms::PostProcessing;

namespace
{
struct Waveform
{
    std::shared_ptr<QPhase::Waveforms::Waveform<double>> mWaveform{nullptr};
    bool mVisible{true}; 
};

[[nodiscard]]
std::pair<std::chrono::microseconds, std::chrono::microseconds>
    getPlotLimits(std::vector<Waveform> waveforms,
                  const bool lVisible = true)
{
    std::pair<std::chrono::microseconds, std::chrono::microseconds> result;
/*
    if (lVisible)
    {
        for (const auto &waveform : waveforms)
        {
            if (waveform.mVisible)
            {
                result.first = std::min(result.first,
                                        waveform.mWaveform->getEarliestTime());
                result.second = std::max(result.second,
                                         waveform.mWaveform->getLatestTime());
            }
        }
    }
    else
    {
        for (const auto &waveform : waveforms)
        {
            result.first = std::min(result.first,
                                    waveform.mWaveform->getEarliestTime());
            result.second = std::max(result.second,
                                     waveform.mWaveform->getLatestTime());
        }
    }
    return result; 
*/
}
 
}

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
        if (!mWaveforms.empty())
        {
            denominator = std::min(static_cast<int> (mWaveforms.size()),
                                   mMaxTracesPerScene);
        }
        mTraceHeight
            = static_cast<int> (std::floor(availableHeight/denominator));
    }
///private:
    std::vector<Waveform> mWaveforms;
    QSize mCurrentSize;
    QColor mBackgroundColor{Qt::white};
    QString mBackgroundName{tr("Trace Viewer")};
    QFont mBackgroundFont;
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    double mZoomFactor{1.1};
    int mNumberOfZooms{0};
    int mTraceWidth{400};
    int mTraceHeight{150};
    int mMaxTracesPerScene = 11;
    TimeConvention mTimeConvention{TimeConvention::Absolute};
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

/// Time limits
void TraceScene::setAbsoluteTimeLimits(
    const std::pair<std::chrono::microseconds, std::chrono::microseconds>
    &timeLimits)
{
    if (timeLimits.first >= timeLimits.second)
    {   
        throw std::invalid_argument("timeLimits.first >= timeLimits.second");
    }
    pImpl->mPlotEarliestTime = timeLimits.first;
    pImpl->mPlotLatestTime = timeLimits.second;
    pImpl->mTimeConvention = TimeConvention::Absolute;
    for (auto &item : items())
    {
        //auto traceItem = reinterpret_cast<TraceItem *> (item);
        //traceItem->setXAxisLimits(axisLimits);
    }
}


/// Populate scene 
void TraceScene::populateScene()
{
    bool haveData = std::any_of(pImpl->mWaveforms.begin(),
                                pImpl->mWaveforms.end(),
                                [](const Waveform &w) 
                                {
                                   return w.mVisible;
                                });
auto nTraces = static_cast<int> (pImpl->mWaveforms.size());
    if (!haveData)
    {
        qDebug() << "No data in trace scene.  Setting default background...";
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
    }
    else
    {
        qDebug() << "Creating new trace scene...";
        // Get axis limits
        auto axisLimits = std::pair(pImpl->mPlotEarliestTime,
                                    pImpl->mPlotLatestTime);
        if (pImpl->mTimeConvention == TimeConvention::Relative)
        {
            qCritical() << "Not done";
        } 
        int traceWidth = pImpl->mTraceWidth;
        int traceHeight = pImpl->mTraceHeight;
        setSceneRect(0, 0, traceWidth, traceHeight*nTraces);
        clear();
    }
}

/// Wheel event - handles zooming and scrolling left / right
void TraceScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 
    bool updatePlot = false;
    bool handled = false;
    bool haveData = std::any_of(pImpl->mWaveforms.begin(),
                                pImpl->mWaveforms.end(),
                                [](const Waveform &w)
                                {
                                   return w.mVisible;
                                });
    // Is there anything to do?
    if (haveData)
    {
/*
        auto tMin = pImpl->mPlotEarliestTime.count();
        auto tMax = pImpl->mPlotLatestTime.count();
        auto durationInMicroSeconds = tMax - tMin;
        auto halfDurationInMicroSeconds
            = static_cast<int64_t> (std::round(durationInMicroSeconds/2));
        auto newDurationInMicroSeconds = durationInMicroSeconds;
        auto xPosition = static_cast<double> (event->scenePos().x());
        double centerFraction = xPosition/width();
        auto currentCenterTimeInMicroSeconds = tMin
                                             + halfDurationInMicroSeconds;
        auto pickedCenterTime
            = tMin
            + static_cast<int64_t> (std::round(durationInMicroSeconds
                                             *centerFraction));
*/
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
