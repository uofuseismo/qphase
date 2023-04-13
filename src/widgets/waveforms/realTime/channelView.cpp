#include <cmath>
#include <chrono>
#include <vector>
#include <QDebug>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QScrollBar>
#include "qphase/widgets/waveforms/realTime/channelView.hpp"
#include "qphase/widgets/waveforms/realTime/channelScene.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/connection/sqlite3.hpp"

#define DEFAULT_TRACE_HEIGHT 150

namespace
{
template<typename T>
std::pair<std::chrono::microseconds, std::chrono::microseconds> 
    getStopEndTimes(const QPhase::Waveforms::Channel<T> &channel)
{
    std::chrono::microseconds tMin{std::numeric_limits<int64_t>::max()};
    std::chrono::microseconds tMax{std::numeric_limits<int64_t>::lowest()};
    try
    {
        tMin = channel.getWaveformReference().getEarliestTime();
        tMax = channel.getWaveformReference().getLatestTime();
    }
    catch (...)
    {
    }
    return std::pair{tMin, tMax};
}

}

using namespace QPhase::Widgets::Waveforms::RealTime;

class ChannelView::ChannelViewImpl
{
public:
    ChannelScene *mScene{nullptr};
    QPhase::Database::Connection::SQLite3 mDatabase;
    QPhase::Database::Internal::Event mEvent;
    std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>> mChannels;
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    int mMinimumWidth{600};
    int mMinimumHeight{200};
    int mTraceHeight{DEFAULT_TRACE_HEIGHT};
    int mTraceWidth{400};
    TimeConvention mTimeConvention{TimeConvention::Absolute};
};

/// C'tor
ChannelView::ChannelView(QWidget *parent) :
    QGraphicsView(parent),
    pImpl(std::make_unique<ChannelViewImpl> ())
{
    setMinimumSize(pImpl->mMinimumWidth, pImpl->mMinimumHeight);

    auto boundingRect
        = this->mapToScene(this->viewport()->geometry()).boundingRect();
    int traceHeight = DEFAULT_TRACE_HEIGHT;
    int traceWidth  = static_cast<int> (boundingRect.width());
    
    pImpl->mScene = new ChannelScene(traceWidth, traceHeight);
    setScene(pImpl->mScene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::LosslessImageRendering);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalScrollBar()->setSliderPosition(1); // Start scrollbar near top
}

/// Destructor
ChannelView::~ChannelView() = default;

/// Resize event
void ChannelView::resizeEvent(QResizeEvent *event)
{
    auto newSize = event->size();
    int innerWidth = newSize.width();
    newSize.setWidth(innerWidth);
    if (pImpl->mScene){pImpl->mScene->resize(newSize);}
    QGraphicsView::resizeEvent(event);
}

/// Time limits
void ChannelView::setTimeLimits(const std::pair<std::chrono::microseconds,
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
void ChannelView::redrawScene()
{
    pImpl->mScene->update();
}

/// Sets the channels
void ChannelView::setChannels(
    std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>> &channels)
{
    if (channels == nullptr){throw std::invalid_argument("Channels is NULL");}
    if (channels->empty())
    {
        throw std::invalid_argument("No channels");
    }
    // If not present then set the time limits
    if (pImpl->mPlotEarliestTime.count() == 0 &&
        pImpl->mPlotLatestTime.count() == 0)
    {
        std::chrono::microseconds tMin{std::numeric_limits<int64_t>::max()};
        std::chrono::microseconds tMax{std::numeric_limits<int64_t>::lowest()};
        for (const auto &channel : *channels)
        {
            auto [t0, t1] = ::getStopEndTimes(channel);
            tMin = std::min(tMin, t0);
            tMax = std::max(tMax, t1);
        }
        setTimeLimits(std::pair(tMin, tMax));
    }
    // Now get a pointer to the channels 
    pImpl->mChannels = channels;
    pImpl->mScene->setChannels(channels);
    redrawScene();
}

/// Sets the event that is being processed
void ChannelView::setEvent(const QPhase::Database::Internal::Event &event)
{
    if (event.haveIdentifier())
    {
        throw std::invalid_argument("Event identifier not set");
    }
    pImpl->mEvent = event;
} 

QPhase::Database::Internal::Event ChannelView::getEvent() const
{
    return pImpl->mEvent;
}

void ChannelView::clearEvent() noexcept
{
    pImpl->mEvent.clear();
}

