#include <iostream>
#include <QDateTime>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QString>
#include "qphase/widgets/waveforms/channelItem.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/segment.hpp"

using namespace QPhase::Widgets::Waveforms;

class ChannelItem::ChannelItemImpl
{
public:
    QString mName;
    QPen mNamePen{Qt::black};
    QPen mWaveformPen{Qt::black, 0, Qt::SolidLine};
    QPen mBorderPen{Qt::black, 2}; 
    QPen mTimeLabelsPen{Qt::black};
    QPen mGridPen{Qt::gray, 0, Qt::DashLine};
    QPen mMajorTickPen{Qt::black, 0, Qt::SolidLine};
    QPen mMinorTickPen{Qt::black, 0, Qt::SolidLine};
    QFont mNameFont{"Monospace", 9, QFont::Normal, false};
    QFont mTimeLabelsFont{"Monospace", 9, QFont::Light, false};
    QRectF mLocalBounds{QRect{0, 0, 1, 1}};
    QRectF mGlobalBounds{mLocalBounds};
    QPainterPath mLocalBoundsPainterPath;
    qreal mMajorTickHeight = 10;
    qreal mMinorTickHeight = 4;
    std::chrono::microseconds mPlotStartTime{0};
    std::chrono::microseconds mPlotEndTime{0};
    int mMajorTicks = 5;
    int mMinorTicks = 2*mMajorTicks - 1;
};

/// C'tor
ChannelItem::ChannelItem(const QRectF &globalBounds,
                         QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    pImpl(std::make_unique<ChannelItemImpl> ())
{
    // Setup geometry
    pImpl->mGlobalBounds = globalBounds;
    pImpl->mLocalBounds  = globalBounds;
    pImpl->mLocalBoundsPainterPath.addRect(pImpl->mLocalBounds);
    // Grapics item can be selected
    setFlag(QGraphicsItem::ItemIsSelectable);
    // Call paint for all redraws
    setCacheMode(QGraphicsItem::CacheMode::NoCache);
}

/// C'tor
template<typename U>
ChannelItem::ChannelItem(const QPhase::Waveforms::Channel<U> &channel,
                         const QRectF &globalShape,
                         QGraphicsItem *parent) :
    ChannelItem(globalShape, parent)
{
}

/// Destructor
ChannelItem::~ChannelItem() = default;

/// The shape in local coordinates
QPainterPath ChannelItem::shape() const
{
    return pImpl->mLocalBoundsPainterPath;
}

/// Bounding rect
QRectF ChannelItem::boundingRect() const
{
    return pImpl->mGlobalBounds;
}

void ChannelItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *,
                        QWidget *)
{
    drawGrid(painter);
    drawName(painter);
}

/// Set the channel's name
void ChannelItem::setName(const QString &name)
{
    pImpl->mName = name;
}

/// Draw the channel's name
void ChannelItem::drawName(QPainter *painter)
{
    if (pImpl->mName.isEmpty()){return;}
    // Draw the name in a text box in the lower left corner
    painter->setPen(pImpl->mNamePen);
    painter->setFont(pImpl->mNameFont);
    auto width = pImpl->mLocalBounds.width();
    auto height = pImpl->mLocalBounds.height();
    qreal pixelWidth{100};
    qreal pixelHeight{12};
    qreal boxBase = height - pixelHeight - 3;  
    QRectF nameBox(width*0.005, boxBase, pixelWidth, pixelHeight);
    painter->drawText(nameBox, Qt::AlignLeft | Qt::AlignTop, pImpl->mName);
}

/// Draw grid
void ChannelItem::drawGrid(QPainter *painter)
{
    // Draw the outline.  This consists of a line above and below the trace.
    painter->setPen(pImpl->mBorderPen);
    auto borderRect = boundingRect();
    painter->drawLine(QPointF(0,                  0),
                      QPointF(borderRect.width(), 0)); 
    painter->drawLine(QPointF(0,                  borderRect.height()),
                      QPointF(borderRect.width(), borderRect.height()));
    // Grid lines for major ticks
    painter->setPen(pImpl->mGridPen);
    auto dMajorTick = static_cast<qreal> (borderRect.width())
                                        /(pImpl->mMajorTicks - 1);
    for (int it = 0; it < pImpl->mMajorTicks; ++it)
    {
        painter->drawLine(QPointF(it*dMajorTick, 0),
                          QPointF(it*dMajorTick, borderRect.height()));
    }
    // Draw the tick marks
    painter->setPen(pImpl->mMajorTickPen);
    auto tickHeight = pImpl->mMajorTickHeight;
    for (int it = 0; it < pImpl->mMajorTicks; ++it)
    {
        painter->drawLine(QPointF(it*dMajorTick, 0),
                          QPointF(it*dMajorTick, tickHeight));
    }
    // Draw the minor ticks
    painter->setPen(pImpl->mMinorTickPen);
    tickHeight = pImpl->mMinorTickHeight;
    if (pImpl->mMinorTicks > 0) 
    {
        auto dMinorTick = dMajorTick/(pImpl->mMinorTicks + 1);
        for (int it = 0; it < pImpl->mMajorTicks - 1; ++it)
        {
            for (int jt = 0; jt < pImpl->mMinorTicks; ++jt)
            {
                auto tickLocation = it*dMajorTick + (jt + 1)*dMinorTick;
                painter->drawLine(QPointF(tickLocation, 0),
                                  QPointF(tickLocation, tickHeight));
            }
        }
    }
    // Draw the times on the minor ticks
    auto tDurationMuS = pImpl->mPlotEndTime.count()
                      - pImpl->mPlotStartTime.count();
    auto dtGrid = static_cast<double> (tDurationMuS/(pImpl->mMajorTicks - 1)); 
    painter->setPen(pImpl->mTimeLabelsPen);
    painter->setFont(pImpl->mTimeLabelsFont);
    // Don't plot the last time stamp - goes off the screen
    for (int it = 0; it < pImpl->mMajorTicks - 1; ++it)
    {
        auto t = pImpl->mPlotStartTime.count()
               + static_cast<int64_t> (std::round(dtGrid*it));
        auto tMilliSeconds = static_cast<qint64> (std::round(t/1000.));
        auto tLabel = QDateTime::fromMSecsSinceEpoch(tMilliSeconds, Qt::UTC);
        QString tString;
        if (tDurationMuS < 1000000) // 1 second -> MM:SS.SSS
        {
            tString = tLabel.toString("mm:ss.zzz");
        }
        else if (tDurationMuS < 10000000) // 10 seconds -> MM:SS.SS
        {
            tString = tLabel.toString("mm:ss.zz");
        }
        else if (tDurationMuS < 3600000000) // 1 hour -> HH:MM:SS
        {
            tString = tLabel.toString("hh:mm:ss");
        }
        else // YEAR-MM-DD
        {
            tString = tLabel.toString("yyyy-MM-dd");
        }
        auto pixelSize = pImpl->mTimeLabelsFont.pixelSize();
        if (pixelSize < 0)
        {
            pixelSize
                = static_cast<int> (pImpl->mTimeLabelsFont.pointSizeF()*1.33);
        }
        QRectF timeBox(dMajorTick*it + 1, pImpl->mMinorTickHeight,
                       tString.size()*pixelSize, pixelSize);
        painter->drawText(timeBox, Qt::AlignLeft, tString);
    }
}

/// Limits
void ChannelItem::setAbsoluteTimeLimits(
    const std::pair<std::chrono::microseconds, 
                    std::chrono::microseconds> &plotLimits)
{
    if (plotLimits.first > plotLimits.second)
    {
        throw std::invalid_argument("plotLimits.first > plotLimits.second");
    }
    pImpl->mPlotStartTime = plotLimits.first;
    pImpl->mPlotEndTime = plotLimits.second;
}

///--------------------------------------------------------------------------///
///                             Template Instantiation                       ///
///--------------------------------------------------------------------------///
template QPhase::Widgets::Waveforms::ChannelItem::ChannelItem(
    const QPhase::Waveforms::Channel<double> &,
    const QRectF &, QGraphicsItem *);
template QPhase::Widgets::Waveforms::ChannelItem::ChannelItem(
    const QPhase::Waveforms::Channel<float> &,
    const QRectF &, QGraphicsItem *);
