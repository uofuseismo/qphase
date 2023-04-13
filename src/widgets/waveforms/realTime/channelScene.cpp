#include <iostream>
#include <cmath>
#include <vector>
#include <QColor>
#include <QFont>
#include <QGraphicsSceneWheelEvent>
#include <QString>
#include "qphase/widgets/waveforms/realTime/channelScene.hpp"
#include "qphase/widgets/waveforms/channelItem.hpp"
#include "qphase/waveforms/channel.hpp"

using namespace QPhase::Widgets::Waveforms::RealTime;

class ChannelScene::ChannelSceneImpl
{
public:
    ChannelSceneImpl(const int traceWidth,
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
        if (mChannels != nullptr)
        {
            int nChannels = static_cast<int> (mChannels->size());
            if (nChannels > 0)
            {
                denominator = std::min(nChannels, mMaxTracesPerScene);
            }
        }
        mTraceHeight
            = static_cast<int> (std::floor(availableHeight/denominator));
    }
///private:
    std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>>
        mChannels{nullptr};
    QSize mCurrentSize;
    QColor mBackgroundColor{Qt::white};
    QString mBackgroundName{tr("Channel Viewer")};
    QFont mBackgroundFont{"Helvetica", 22, QFont::Light, false};
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    std::chrono::microseconds mOriginalEarliestTime{0};
    std::chrono::microseconds mOriginalLatestTime{0};
    std::map<QString, ChannelItem<double> *> mChannelItems;
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
ChannelScene::ChannelScene(const int traceWidth,
                           const int traceHeight,
                           QObject *parent) :
    QGraphicsScene(parent),
    pImpl(std::make_unique<ChannelSceneImpl> (traceWidth, traceHeight))
{
    pImpl->mCurrentSize = QSize(static_cast<int> (width()),
                                static_cast<int> (height()));
    setBackgroundBrush(pImpl->mBackgroundColor);
    populateScene();
}

/// Destructor
ChannelScene::~ChannelScene() = default;

/// Resize event
void ChannelScene::resize(const QSize &newSize)
{
    pImpl->mCurrentSize = newSize;
    pImpl->mTraceWidth = newSize.width();
    pImpl->recomputeTraceHeight();
    populateScene(); // replot
}

/// Time limits
void ChannelScene::setAbsoluteTimeLimits(
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
    for (auto &channelItem : pImpl->mChannelItems)
    {
        channelItem.second->setAbsoluteTimeLimits(timeLimits);
    }
    updatePlot();
}

/// Update the plot
void ChannelScene::updatePlot()
{
/*
    for (auto &channelItem : pImpl->mChannelItems)
    {
        //channelItem.update();
    }
*/
}

/// Populate scene 
void ChannelScene::populateScene()
{
    int nTraces = 0;
    if (pImpl->mChannels != nullptr)
    {
        nTraces = static_cast<int> (pImpl->mChannels->size());
    }
    if (nTraces == 0)
    {
        qDebug() << "No data in channel scene.  Setting default background...";
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
        return;
    }
    qDebug() << "Creating new channel scene with " << nTraces << " traces...";
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

/// Wheel event - handles zooming and scrolling left / right
void ChannelScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 
    bool updatePlot = false;
    bool handled = false;
    bool haveData = false;
    if (pImpl->mChannels){haveData = !pImpl->mChannels->empty();}
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
        auto halfDurationMuS
            = static_cast<int64_t> (std::round(static_cast<double> (durationMuS)
                                              *0.5));
        // Get the location to approximately zoom in/out on
        auto xPosition = static_cast<double> (event->scenePos().x());
        double centerFraction = xPosition/width();
        int64_t currentCenterTimeMuS = tMin + halfDurationMuS;
        auto pickedCenterTimeMuS
            = tMin
            + static_cast<int64_t> (std::round(static_cast<double> (durationMuS)
                                             *centerFraction));
        // Don't snap to the location under the mouse.  Makes continuous
        // scrolling awkward.  Instead head in the desired direction.
        auto newCenterTimeMuS 
            = static_cast<int64_t>
                    (std::round(0.9*static_cast<double> (currentCenterTimeMuS)
                              + 0.1*static_cast<double> (pickedCenterTimeMuS)));
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
                    = static_cast<int64_t>
                      (std::round(static_cast<double> (durationMuS)
                                 /pImpl->mZoomFactor));
                auto newHalfDurationMuS
                    = static_cast<int64_t>
                      (std::round(static_cast<double> (newDurationMuS)*0.5));
                plotT0 = std::chrono::microseconds{newCenterTimeMuS
                                                 - newHalfDurationMuS};
                plotT1 = std::chrono::microseconds{newCenterTimeMuS
                                                 + newHalfDurationMuS};
                plotT0 = std::max(pImpl->mOriginalEarliestTime, plotT0);
                plotT1 = std::min(pImpl->mOriginalLatestTime,   plotT1);
                pImpl->mNumberOfZooms = pImpl->mNumberOfZooms + 1;
                updatePlot = true;
            }
            // Handle zoom out
            else if (pImpl->mNumberOfZooms > 0) // !zoomIn
            {
                //qDebug() << "Zooming out";
                auto newDurationMuS
                    = static_cast<int64_t>
                      (std::round(static_cast<double> (durationMuS)
                                 *pImpl->mZoomFactor));
                auto newHalfDurationMuS
                    = static_cast<int64_t> 
                      (std::round(static_cast<double> (newDurationMuS)*0.5));
                plotT0 = std::chrono::microseconds{newCenterTimeMuS
                                                 - newHalfDurationMuS};
                plotT1 = std::chrono::microseconds{newCenterTimeMuS
                                                 + newHalfDurationMuS};
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
                            + static_cast<int64_t>
                              (std::round(shiftFactor
                                         *static_cast<double> (durationMuS)));
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
                            - static_cast<int64_t>
                              (std::round(shiftFactor
                                         *static_cast<double> (durationMuS)));
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
        for (auto &channelItem : pImpl->mChannelItems)
        {
            channelItem.second->setAbsoluteTimeLimits(std::pair{plotT0, plotT1});
        }
        update();
    }
    // Was the event handled?
    event->ignore();
    if (handled){event->accept();}
}

/// Sets the channels
void ChannelScene::setChannels(
    std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>> &channels)
{
    if (channels == nullptr){throw std::invalid_argument("Channels is NULL");}
    if (channels->empty())
    {
        throw std::invalid_argument("No channels");
    }
    pImpl->mChannels = channels;
    pImpl->recomputeTraceHeight();
    populateScene();
}

