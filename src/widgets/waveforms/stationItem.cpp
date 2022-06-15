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

using namespace QPhase::Widgets::Waveforms;

class StationItem::StationItemImpl
{
public:
    QRectF mLocalBounds{QRect{0, 0, 1, 1}};
    QRectF mGlobalBounds{mLocalBounds};
    QPainterPath mLocalBoundsPainterPath;
    QString mName;
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
        pImpl->mName.fromStdString(stationWaveforms.getNetworkCode() + "."
                                 + stationWaveforms.getName());
    }
    catch (const std::exception &e)
    {
        qWarning() << "Failed to set name: " << e.what();
    }
    auto nChannels = stationWaveforms.getNumberOfChannels();
    pImpl->mNumberOfChannels = nChannels;
    auto channelWidth  = boundingRect().width(); 
    auto channelHeight = boundingRect().height()/std::max(1, nChannels);
//    for (const auto &channel : stationWaveforms)
    {
        QRectF channelShape{0, 0, channelWidth, channelHeight};
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

/// The shape in local coordaintes
QPainterPath StationItem::shape() const
{
    return pImpl->mLocalBoundsPainterPath;
}

/// Bounding rect
QRectF StationItem::boundingRect() const
{
    return pImpl->mGlobalBounds;
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

