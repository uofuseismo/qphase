#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QString>
#include "qphase/widgets/waveforms/stationItem.hpp"
#include "qphase/widgets/waveforms/channelItem.hpp"
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"
#include "qphase/waveforms/singleChannelVerticalSensor.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"

using namespace QPhase::Widgets::Waveforms;

class StationItem::StationItemImpl
{
public:
    QRectF mLocalBounds{QRect{0, 0, 1, 1}};
    QRectF mGlobalBounds{mLocalBounds};
    QPainterPath mLocalBoundsPainterPath;
    QString mName;
    std::chrono::microseconds mPlotEarliestTime{0};
    std::chrono::microseconds mPlotLatestTime{0};
    int mNumberOfChannels = 0;
    VisibleChannels mVisibleChannels = VisibleChannels::All;
};

/// Constructor
StationItem::StationItem(const QRectF &globalBounds, QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    pImpl(std::make_unique<StationItemImpl> ()) 
{
    // Setup geometry
    pImpl->mGlobalBounds = globalBounds;
    pImpl->mLocalBounds  = globalBounds;
    pImpl->mLocalBoundsPainterPath.addRect(pImpl->mLocalBounds);
    // Call paint for all redraws
    setCacheMode(QGraphicsItem::CacheMode::NoCache);
}

/// C'tor with waveform
template<typename U>
StationItem::StationItem(
    const QPhase::Waveforms::Station<U> &stationWaveforms,
    const QRectF &globalBounds,
    QGraphicsItem *parent) :
    StationItem(globalBounds, parent)
{
    setWaveforms(stationWaveforms);
}

/// Sets the station data
template<typename U>
void StationItem::setWaveforms(
    const QPhase::Waveforms::Station<U> &stationWaveforms)
{
    // Set the name
    pImpl->mName.clear();
    try
    {
        auto name = stationWaveforms.getNetworkCode() + "."
                  + stationWaveforms.getName();
        pImpl->mName = QString::fromStdString(name);
    }
    catch (const std::exception &e)
    {
        qWarning() << "Failed to set name: " << e.what();
    }
    auto threeComponentSensors
        = stationWaveforms.getThreeChannelSensorsReference();
    auto singleComponentVerticalSensors
        = stationWaveforms.getSingleChannelVerticalSensorsReference(); 
    auto singleComponentSensors
        = stationWaveforms.getSingleChannelSensorsReference();
    auto nChannels = stationWaveforms.getNumberOfChannels();
    pImpl->mNumberOfChannels = nChannels;
    auto channelWidth  = boundingRect().width();
    auto channelHeight = boundingRect().height()/std::max(1, nChannels);
    // Make the channels
    QRectF channelPlotArea{0, 0, channelWidth, channelHeight};
    int iChannel = 0;
    for (const auto &sensor : threeComponentSensors)
    {
        auto nameBase = pImpl->mName;
        auto locationCode = QString::fromStdString(sensor.getLocationCode());
        auto verticalChannel = sensor.getVerticalChannelReference();
        auto name = nameBase + "."
                  + QString::fromStdString(verticalChannel.getChannelCode());
        if (!locationCode.isEmpty()){name = name + "." + locationCode;}
        auto zChannelItem = new ChannelItem<U>(channelPlotArea, this);
        zChannelItem->setAbsoluteTimeLimits(std::pair(pImpl->mPlotEarliestTime,
                                                      pImpl->mPlotLatestTime));
        zChannelItem->setWaveform(verticalChannel, WaveformType::Seismogram);
        zChannelItem->setPos(0, iChannel*channelHeight);
        zChannelItem->setName(name);
        iChannel = iChannel + 1;

        auto northChannel = sensor.getNorthChannelReference();
        name = nameBase + "." 
             + QString::fromStdString(northChannel.getChannelCode());
        if (!locationCode.isEmpty()){name = name + "." + locationCode;}
        auto nChannelItem = new ChannelItem<U>(channelPlotArea, this);
        nChannelItem->setAbsoluteTimeLimits(std::pair(pImpl->mPlotEarliestTime,
                                                      pImpl->mPlotLatestTime));
        nChannelItem->setWaveform(northChannel, WaveformType::Seismogram);
        nChannelItem->setPos(0, iChannel*channelHeight);
        nChannelItem->setName(name);
        iChannel = iChannel + 1;

        auto eastChannel = sensor.getEastChannelReference();
        name = nameBase + "." 
             + QString::fromStdString(eastChannel.getChannelCode());
        if (!locationCode.isEmpty()){name = name + "." + locationCode;}
        auto eChannelItem = new ChannelItem<U>(channelPlotArea, this);
        eChannelItem->setAbsoluteTimeLimits(std::pair(pImpl->mPlotEarliestTime,
                                                      pImpl->mPlotLatestTime));
        eChannelItem->setWaveform(eastChannel, WaveformType::Seismogram);
        eChannelItem->setPos(0, iChannel*channelHeight);
        eChannelItem->setName(name);
        iChannel = iChannel + 1;
    }
    for (const auto &sensor : singleComponentVerticalSensors)
    {
        auto verticalChannel = sensor.getVerticalChannelReference();
        auto locationCode = QString::fromStdString(sensor.getLocationCode());
        auto name = pImpl->mName + "." 
                  + QString::fromStdString(verticalChannel.getChannelCode());
        if (!locationCode.isEmpty()){name = name + "." + locationCode;}
        auto zChannelItem = new ChannelItem<U>(channelPlotArea, this);
        zChannelItem->setAbsoluteTimeLimits(std::pair(pImpl->mPlotEarliestTime,
                                                      pImpl->mPlotLatestTime));
        zChannelItem->setWaveform(verticalChannel, WaveformType::Seismogram);
        zChannelItem->setPos(0, iChannel*channelHeight);
        zChannelItem->setName(name);
        iChannel = iChannel + 1; 
    }
    for (const auto &sensor : singleComponentSensors)
    {
        auto channel = sensor.getChannelReference();
        auto locationCode = QString::fromStdString(sensor.getLocationCode());
        auto name = pImpl->mName + "." 
                  + QString::fromStdString(channel.getChannelCode());
        if (!locationCode.isEmpty()){name = name + "." + locationCode;}
        auto channelItem = new ChannelItem<U>(channelPlotArea, this);
        channelItem->setAbsoluteTimeLimits(std::pair(pImpl->mPlotEarliestTime,
                                                     pImpl->mPlotLatestTime));
        channelItem->setWaveform(channel, WaveformType::Seismogram);
        channelItem->setPos(0, iChannel*channelHeight);
        channelItem->setName(name);
        iChannel = iChannel + 1;
    }
}

/// The station name
QString StationItem::getName() const noexcept
{
    return pImpl->mName;
}

/// The number of channels
int StationItem::getNumberOfChannels() const noexcept
{
    return pImpl->mNumberOfChannels;
}

/// Destructor
StationItem::~StationItem() = default;

/// The shape in local coordinates
QPainterPath StationItem::shape() const
{
    return pImpl->mLocalBoundsPainterPath;
}

/// Bounding rect
QRectF StationItem::boundingRect() const
{
    return pImpl->mGlobalBounds;
}

/// Limits
void StationItem::setAbsoluteTimeLimits(
    const std::pair<std::chrono::microseconds, 
                    std::chrono::microseconds> &plotLimits)
{
    if (plotLimits.first > plotLimits.second)
    {
        throw std::invalid_argument("plotLimits.first > plotLimits.second");
    }
    pImpl->mPlotEarliestTime = plotLimits.first;
    pImpl->mPlotLatestTime   = plotLimits.second;
    for (auto &childItem : childItems())
    {
        auto channelItem = reinterpret_cast<ChannelItem<> *> (childItem);
        channelItem->setAbsoluteTimeLimits(plotLimits);
    }
}


///--------------------------------------------------------------------------///
///                          Template Instantiation                          ///
///--------------------------------------------------------------------------///
template QPhase::Widgets::Waveforms::StationItem::StationItem(
    const QPhase::Waveforms::Station<double> &stationWaveforms,
    const QRectF &, QGraphicsItem *); 
template QPhase::Widgets::Waveforms::StationItem::StationItem(
    const QPhase::Waveforms::Station<float> &stationWaveforms,
    const QRectF &, QGraphicsItem *);
template void QPhase::Widgets::Waveforms::StationItem::setWaveforms(
    const QPhase::Waveforms::Station<double> &);
template void QPhase::Widgets::Waveforms::StationItem::setWaveforms(
    const QPhase::Waveforms::Station<float> &); 

