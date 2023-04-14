#include <cmath>
#include <chrono>
#include <vector>
#include <QDebug>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QScrollBar>
#include "qphase/widgets/waveforms/stationView.hpp"
#include "qphase/widgets/waveforms/stationScene.hpp"
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/waveforms/singleChannelVerticalSensor.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/connection/sqlite3.hpp"

#define DEFAULT_TRACE_HEIGHT 150

namespace
{
template<typename T>
std::pair<std::chrono::microseconds, std::chrono::microseconds> 
    getStopEndTimes(const QPhase::Waveforms::Station<T> &station)
{
    std::chrono::microseconds tMin{std::numeric_limits<int64_t>::max()};
    std::chrono::microseconds tMax{std::numeric_limits<int64_t>::lowest()};
    auto sensors3C = station.getThreeChannelSensorsReference();
    for (const auto &channel : sensors3C)
    {
        try
        {
            tMin = std::min(tMin, channel.getVerticalChannelReference()
                                     .getWaveformReference().getEarliestTime());
            tMax = std::max(tMax, channel.getVerticalChannelReference()
                                     .getWaveformReference().getLatestTime());
        }
        catch (...)
        {
        }
        try
        {
            tMin = std::min(tMin, channel.getNorthChannelReference()
                                     .getWaveformReference().getEarliestTime());
            tMax = std::max(tMax, channel.getNorthChannelReference()
                                     .getWaveformReference().getLatestTime());
        }
        catch (...)
        {
        }
        try
        {
            tMin = std::min(tMin, channel.getEastChannelReference()
                                     .getWaveformReference().getEarliestTime());
            tMax = std::max(tMax, channel.getEastChannelReference()
                                     .getWaveformReference().getLatestTime());
        }
        catch (...)
        {
        }
    }
    auto sensors1C = station.getSingleChannelVerticalSensorsReference();
    for (const auto &channel : sensors1C)
    {
        try
        {
            tMin = std::min(tMin, channel.getVerticalChannelReference()
                                     .getWaveformReference().getEarliestTime());
            tMax = std::max(tMax, channel.getVerticalChannelReference()
                                     .getWaveformReference().getLatestTime());
        }
        catch (...)
        {
        }
    }
    auto sensors1NVC = station.getSingleChannelSensorsReference();
    for (const auto &channel : sensors1NVC)
    {
        try
        {
            tMin = std::min(tMin, channel.getChannelReference()
                                     .getWaveformReference().getEarliestTime());
            tMax = std::max(tMax, channel.getChannelReference()
                                     .getWaveformReference().getLatestTime());
        }
        catch (...)
        {
        }
    }
    return std::pair{tMin, tMax};
}

}

using namespace QPhase::Widgets::Waveforms;

class StationView::StationViewImpl
{
public:
    StationScene *mScene{nullptr};
    QPhase::Database::Connection::SQLite3 mDatabase;
    QPhase::Database::Internal::Event mEvent;
    std::shared_ptr<std::vector<QPhase::Waveforms::Station<double>>> mStations;
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
    int traceWidth  = static_cast<int> (boundingRect.width());
    
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
        qCritical() << "Scene is NULL";
        return;
    }
    if (timeLimits.first > timeLimits.second)
    {
        throw std::runtime_error("timeLimits.first > timeLimits.second");
    }
    try
    {
        pImpl->mTimeConvention = TimeConvention::Absolute;
        pImpl->mScene->setAbsoluteTimeLimits(timeLimits);
    }
    catch (const std::exception &e)
    {
        qCritical() << "Failed to set time limits: " << e.what();
        return; 
    }
    pImpl->mPlotEarliestTime = timeLimits.first;
    pImpl->mPlotLatestTime   = timeLimits.second;
    redrawScene();
}

/// Force a redraw
void StationView::redrawScene()
{
    pImpl->mScene->update();
}

/// Sets the stations
void StationView::setStations(
    std::shared_ptr<std::vector<QPhase::Waveforms::Station<double>>> &stations)
{
    if (stations == nullptr){throw std::invalid_argument("Stations is NULL");}
    if (stations->empty())
    {
        throw std::invalid_argument("No stations");
    }
    // If not present then set the time limits
    if (pImpl->mPlotEarliestTime.count() == 0 &&
        pImpl->mPlotLatestTime.count() == 0)
    {
        std::chrono::microseconds tMin{std::numeric_limits<int64_t>::max()};
        std::chrono::microseconds tMax{std::numeric_limits<int64_t>::lowest()};
        for (const auto &station : *stations)
        {
            auto [t0, t1] = ::getStopEndTimes(station);
            tMin = std::min(tMin, t0);
            tMax = std::max(tMax, t1);
        }
        setTimeLimits(std::pair(tMin, tMax));
    }
    // Now get a pointer to the stations
    pImpl->mStations = stations;
    pImpl->mScene->setStations(stations);
    redrawScene();
}

/// Sets the event that is being processed
void StationView::setEvent(const QPhase::Database::Internal::Event &event)
{
    if (!event.haveIdentifier())
    {
        throw std::invalid_argument("Event identifier not set");
    }
    pImpl->mEvent = event;
} 

QPhase::Database::Internal::Event StationView::getEvent() const
{
    return pImpl->mEvent;
}

void StationView::clearEvent() noexcept
{
    pImpl->mEvent.clear();
}

/*
template<>
void StationView::setStations<float>(
    std::shared_ptr<std::vector<QPhase::Waveforms::Station<float>>> &stations)
{
    if (stations == nullptr){throw std::invalid_argument("Stations is NULL");}
    if (stations->empty())
    {
        throw std::invalid_argument("No stations");
    }
    pImpl->mDoubleStations = nullptr;
    pImpl->mFloatStations = stations;
}
*/
