#include <iostream>
#include <cmath>
#include <vector>
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QGraphicsSceneWheelEvent>
#include <QString>
#include "qphase/widgets/waveforms/stationScene.hpp"
#include "qphase/widgets/waveforms/stationItem.hpp"
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
        int nChannels = 0;
        if (mStations)
        {
            for (const auto &station : *mStations)
            {
                nChannels = nChannels + station.getNumberOfChannels();
            }
        }
        if (nChannels > 0)
        {
            denominator = std::min(nChannels, mMaxTracesPerScene);
        }
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
    std::chrono::microseconds mOriginalEarliestTime{0};
    std::chrono::microseconds mOriginalLatestTime{0};
    std::map<QString, StationItem *> mStationItems;
    double mZoomFactor{1.1};
    int mNumberOfZooms{0};
    int mTraceWidth{400};
    int mTraceHeight{150};
    int mMaxTracesPerScene = 9;
    TimeConvention mTimeConvention{TimeConvention::Absolute};
    bool mNormalZoom{true}; // Wheel forward zooms in
    bool mNormalTimeAdvance{true}; // Wheel in goes back in time
    bool mRedraw{true}; // Something was updated that requires a redraw
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
    if (timeLimits.first > timeLimits.second)
    {
        throw std::invalid_argument("timeLimits.first > timeLimits.second");
    }
    pImpl->mPlotEarliestTime = timeLimits.first;
    pImpl->mPlotLatestTime = timeLimits.second;
    pImpl->mOriginalEarliestTime = timeLimits.first;
    pImpl->mOriginalLatestTime = timeLimits.second;
    pImpl->mTimeConvention = TimeConvention::Absolute;
    for (auto &stationItem : pImpl->mStationItems)
    {
        stationItem.second->setAbsoluteTimeLimits(timeLimits);
    }
    updatePlot();
}

/// Update the plot
void StationScene::updatePlot()
{
/*
    for (auto &stationItem : pImpl->mStationItems)
    {
        //stationItem.update();
    }
*/
}

/// Populate scene 
void StationScene::populateScene()
{
    int nStations = 0;
    if (pImpl->mStations)
    {
        nStations = static_cast<int> (pImpl->mStations->size());
    }
    if (nStations == 0)
    {
        qDebug() << "No data in trace scene.  Setting default background...";
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
    }
    else
    {
        int nTraces = 0;
        for (const auto &station : *pImpl->mStations)
        {
            nTraces = nTraces + station.getNumberOfChannels();
        }
        qDebug() << "Creating new trace scene with " << nTraces << " traces...";
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
        pImpl->mStationItems.clear();
        int nTotalChannels = 0;
        for (const auto &station : *pImpl->mStations) 
        {
            auto nChannels = station.getNumberOfChannels();
            QRectF stationPlotArea{0, 0,
                                   static_cast<qreal> (traceWidth),
                                   static_cast<qreal> (traceHeight*nChannels)};
            auto stationItem = new StationItem(station, stationPlotArea);
            pImpl->mStationItems.insert(std::pair(stationItem->getName(),
                                                  stationItem));
            stationItem->setPos(0, 1 + nTotalChannels*traceHeight);
            stationItem->setAbsoluteTimeLimits(axisLimits);
            nTotalChannels = nTotalChannels
                           + stationItem->getNumberOfChannels();
            addItem(stationItem);
        }
    }
}

/// Wheel event - handles zooming and scrolling left / right
void StationScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 
    bool updatePlot = false;
    bool handled = false;
    bool haveData = false;
    if (pImpl->mStations){haveData = !pImpl->mStations->empty();}
/*std::any_of(pImpl->mWaveforms.begin(),
                                pImpl->mWaveforms.end(),
                                [](const Waveform &w)
                                {
                                   return w.mVisible;
                                });
*/
    // Is there anything to do?
    std::chrono::microseconds plotT0{0};
    std::chrono::microseconds plotT1{0};
    if (haveData)
    {
        // Get the current plot limits
        auto tMin = pImpl->mPlotEarliestTime.count();
        auto tMax = pImpl->mPlotLatestTime.count();
        auto durationMuS = tMax - tMin;
        auto halfDurationMuS = static_cast<int64_t> (std::round(durationMuS/2));
        // Get the location to approximately zoom in/out on
        auto xPosition = static_cast<double> (event->scenePos().x());
        double centerFraction = xPosition/width();
        auto currentCenterTimeMuS = tMin + halfDurationMuS;
        auto pickedCenterTimeMuS
            = tMin
            + static_cast<int64_t> (std::round(durationMuS*centerFraction));
        // Don't snap to the location under the mouse.  Makes continuous
        // scrolling awkward.  Instead head in the desired direction.
        auto newCenterTimeMuS 
            = static_cast<int64_t> (std::round(0.9*currentCenterTimeMuS
                                             + 0.1*pickedCenterTimeMuS));
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
                //qDebug() << "Zooming in";
                auto newDurationMuS
                    = static_cast<int64_t> (std::round(durationMuS
                                                      /pImpl->mZoomFactor));
                auto halfDurationMuS
                    = static_cast<int64_t> (std::round(newDurationMuS/2.));
                plotT0 = std::chrono::microseconds{newCenterTimeMuS
                                                 - halfDurationMuS};
                plotT1 = std::chrono::microseconds{newCenterTimeMuS
                                                 + halfDurationMuS};
                plotT0 = std::max(pImpl->mOriginalEarliestTime, plotT0);
                plotT1 = std::min(pImpl->mOriginalLatestTime,   plotT1);
                pImpl->mNumberOfZooms = pImpl->mNumberOfZooms + 1;
                updatePlot = true;
            }
            // Handle zoom out
            else if (!zoomIn && pImpl->mNumberOfZooms > 0)
            {
                //qDebug() << "Zooming out";
                auto newDurationMuS
                    = static_cast<int64_t> (std::round(durationMuS
                                                      *pImpl->mZoomFactor));
                auto halfDurationMuS
                    = static_cast<int64_t> (std::round(newDurationMuS/2.));
                plotT0 = std::chrono::microseconds{newCenterTimeMuS
                                                 - halfDurationMuS};
                plotT1 = std::chrono::microseconds{newCenterTimeMuS
                                                 + halfDurationMuS};
                plotT0 = std::max(pImpl->mOriginalEarliestTime, plotT0);
                plotT1 = std::min(pImpl->mOriginalLatestTime,   plotT1);
                pImpl->mNumberOfZooms = pImpl->mNumberOfZooms - 1;
                if (pImpl->mNumberOfZooms == 0)
                {
                    //qDebug() << "Zooming out to max";
                    plotT0 = pImpl->mOriginalEarliestTime;
                    plotT1 = pImpl->mOriginalLatestTime;
                }
                updatePlot = true;
            }
        }
        // Scroll forward/backward in time
        else if (event->modifiers() & Qt::ShiftModifier)
        {
            handled = true;
            double shiftFactor = 0.1;
            // Are we panning left or right?
            bool advanceTime = (event->delta() < 0);
            if (!pImpl->mNormalTimeAdvance){advanceTime = !advanceTime;}
            // Move forward in time
            if (advanceTime)
            {
                if (tMax < pImpl->mOriginalLatestTime.count())
                {
                    auto t0 = tMin
                            + static_cast<int64_t> (std::round(shiftFactor
                                                              *durationMuS));
                    auto t1 = t0 + durationMuS;
                    // Shifted beyond trace
                    if (t1 > pImpl->mOriginalLatestTime.count())
                    {
                        t1 = pImpl->mOriginalLatestTime.count();
                        t0 = std::max(pImpl->mOriginalEarliestTime.count(),
                                      t1 - durationMuS);
                    }
                    plotT0 = std::chrono::microseconds{t0};
                    plotT1 = std::chrono::microseconds{t1};
                    updatePlot = true;
                }
            }
            else
            {
                if (tMin > pImpl->mOriginalEarliestTime.count())
                {
                    auto t0 = tMin
                            - static_cast<int64_t> (std::round(shiftFactor
                                                              *durationMuS));
                    auto t1 = t0 + durationMuS;
                    // Shifted beyond trace
                    if (t0 < pImpl->mOriginalEarliestTime.count())
                    {
                        t0 = pImpl->mOriginalEarliestTime.count();
                        t1 = std::min(pImpl->mOriginalLatestTime.count(),
                                      t0 + durationMuS);
                    }
                    plotT0 = std::chrono::microseconds{t0};
                    plotT1 = std::chrono::microseconds{t1};
                    updatePlot = true;
                }
            }
        }
    }
    // Actually update the plot?
    if (updatePlot)
    {
        pImpl->mPlotEarliestTime = plotT0;
        pImpl->mPlotLatestTime = plotT1;
        for (auto &stationItem : pImpl->mStationItems)
        {
            stationItem.second->setAbsoluteTimeLimits(std::pair{plotT0, plotT1});
        }
        update();
    }
    // Was the event handled?
    event->ignore();
    if (handled){event->accept();}
}

/// Sets the stations
void StationScene::setStations(
    std::shared_ptr<std::vector<QPhase::Waveforms::Station<double>>> &stations)
{
    if (stations == nullptr){throw std::invalid_argument("Stations is NULL");}
    if (stations->empty())
    {
        throw std::invalid_argument("No stations");
    }
    pImpl->mStations = stations;
    pImpl->recomputeTraceHeight();
    populateScene();
}

//template class QPhase::Widgets::Waveforms::StationScene<double>;
//template class QPhase::Widgets::Waveforms::StationScene<float>;
