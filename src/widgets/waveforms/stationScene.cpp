#include <cmath>
#include <vector>
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QGraphicsSceneWheelEvent>
#include <QString>
#include "qphase/widgets/waveforms/stationScene.hpp"
//#include "qphase/widgets/waveforms/stationItem.hpp"
#include "qphase/widgets/waveforms/channelItem.hpp"
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/channel.hpp"

using namespace QPhase::Widgets::Waveforms;

class StationScene::StationSceneImpl
{
public:
    StationSceneImpl(const int traceWidth,
                     const int traceHeight) :
        mTraceWidth(std::max(200, traceWidth)),
        mTraceHeight(std::max(20, traceHeight))
    {
        //mBackgroundFont.setStyleHint(QFont::System);
    }

    /// Recomputes the trace height given the current plot size
    void recomputeTraceHeight()
    {
        auto availableHeight = static_cast<double> (mCurrentSize.height());
        int denominator = 1;
        //auto nChannels = mStations->getNumberOfChannels();
/*
        if (!mWaveforms.empty())
        {
            denominator = std::min(static_cast<int> (mWaveforms.size()),
                                   mMaxTracesPerScene);
        }
*/
        mTraceHeight
            = static_cast<int> (std::floor(availableHeight/denominator));
    }
///private:
    std::shared_ptr<std::vector<QPhase::Waveforms::Station<double>>> mStations;
    QSize mCurrentSize;
    QColor mBackgroundColor{Qt::white};
    QString mBackgroundName{tr("Station Viewer")};
    QFont mBackgroundFont{"Helvetica", 22, QFont::Light, false};
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
StationScene::StationScene(const int traceWidth,
                           const int traceHeight,
                           QObject *parent) :
    QGraphicsScene(parent),
    pImpl(std::make_unique<StationSceneImpl> (traceWidth, traceHeight))
{
    pImpl->mCurrentSize = QSize(width(), height());
    setBackgroundBrush(pImpl->mBackgroundColor);
    populateScene();
}

/// Destructor
StationScene::~StationScene() = default;

/// Resize event
void StationScene::resize(const QSize &newSize)
{
    pImpl->mCurrentSize = newSize;
    pImpl->mTraceWidth = newSize.width();
    pImpl->recomputeTraceHeight();
    populateScene(); // replot
}

/// Time limits
void StationScene::setAbsoluteTimeLimits(
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
        //auto traceItem = reinterpret_cast<StationItem *> (item);
        //traceItem->setXAxisLimits(axisLimits);
    }
}


/// Populate scene 
void StationScene::populateScene()
{
bool haveData = false;
/*
    bool haveData = std::any_of(pImpl->mWaveforms.begin(),
                                pImpl->mWaveforms.end(),
                                [](const Waveform &w) 
                                {
                                   return w.mVisible;
                                });
*/
    int nStations = 0;
    if (pImpl->mStations)
    {
        nStations = static_cast<int> (pImpl->mStations->size());
    }
    if (!haveData)
    {
        qDebug() << "No data in trace scene.  Setting default background...";
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
    }
    else
    {
        qDebug() << "Creating new trace scene...";
/*
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
*/
    }
}

/// Wheel event - handles zooming and scrolling left / right
void StationScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 
    bool updatePlot = false;
    bool handled = false;
    bool haveData = false;
/*std::any_of(pImpl->mWaveforms.begin(),
                                pImpl->mWaveforms.end(),
                                [](const Waveform &w)
                                {
                                   return w.mVisible;
                                });
*/
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

//template class QPhase::Widgets::Waveforms::StationScene<double>;
//template class QPhase::Widgets::Waveforms::StationScene<float>;
