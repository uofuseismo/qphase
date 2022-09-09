#include <iostream>
#include <QDateTime>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QString>
#include "qphase/widgets/waveforms/channelItem.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/segment.hpp"
#include "private/plotUtilities.hpp"

using namespace QPhase::Widgets::Waveforms;

namespace
{
/// @result The waveform descriptor for the pen.
[[nodiscard]] QPen waveformDescriptorToPen(const WaveformType descriptor)
{
    QPen pen;
    if (descriptor == WaveformType::Seismogram)
    {
        pen.setColor(Qt::black);
        pen.setWidth(0);
        pen.setStyle(Qt::SolidLine);
    }
    return pen;
}
}

template<class T>
class ChannelItem<T>::ChannelItemImpl
{
public:
    void redrawWaveform(const WaveformType waveformType = WaveformType::Seismogram,
                        const std::pair<T, T> *range = nullptr)
    {
        // Remove old lines
        mLinesToPlot.erase(
            std::remove_if(mLinesToPlot.begin(), mLinesToPlot.end(),
                           [&](const std::pair<WaveformType, QVector<QLineF>> &lines)
                           {
                               return lines.first == waveformType;
                           }),
                           mLinesToPlot.end());
        auto plotT0MuS = mPlotStartTime;
        auto plotT1MuS = mPlotEndTime;
        auto plotWidth  = static_cast<qreal> (mLocalBounds.width());
        auto plotHeight = static_cast<qreal> (mLocalBounds.height());
        auto linesToPlot = createLines(mChannel,
                                       plotT0MuS, plotT1MuS,
                                       plotWidth, plotHeight,
                                       mWaveformHeightFraction,
                                       range);
        for (auto &lines : linesToPlot)
        {
            mLinesToPlot.push_back(std::pair{waveformType,
                                             std::move(lines)});
        }
    }
    QPhase::Waveforms::Channel<T> mChannel;
    QVector<std::pair<WaveformType, QVector<QLineF>>> mLinesToPlot;
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
    qreal mWaveformHeightFraction = 0.95;
    std::chrono::microseconds mPlotStartTime{0};
    std::chrono::microseconds mPlotEndTime{0};
    int mMajorTicks = 5;
    int mMinorTicks = 2*mMajorTicks - 1;
};

/// C'tor
template<class T>
ChannelItem<T>::ChannelItem(const QRectF &globalBounds,
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
template<class T>
ChannelItem<T>::ChannelItem(const QPhase::Waveforms::Channel<T> &channel,
                            const QRectF &globalShape,
                            const WaveformType waveformType,
                            QGraphicsItem *parent) :
    ChannelItem(globalShape, parent)
{
    setWaveform(channel, waveformType);
}

/// Destructor
template<class T>
ChannelItem<T>::~ChannelItem() = default;

/// Set the waveform
template<class T>
void ChannelItem<T>::setWaveform(const QPhase::Waveforms::Channel<T> &channel,
                                 const WaveformType waveformType)
{
    pImpl->mChannel = channel;
    //auto plotT0MuS = pImpl->mPlotStartTime;
    //auto plotT1MuS = pImpl->mPlotEndTime;
//    auto [vMin, vMax] = getMinMaxForPlotting(channel, plotT0MuS, plotT1MuS);
    const std::pair<T, T> *range = nullptr;
    pImpl->redrawWaveform(waveformType, range);
/*
    qreal heightFraction{pImpl->mWaveformHeightFraction};
    const std::pair<U, U> *range = nullptr;

    // Get plot width/height
    auto plotWidth  = static_cast<qreal> (pImpl->mLocalBounds.width());
    auto plotHeight = static_cast<qreal> (pImpl->mLocalBounds.height());
    auto linesToPlot = createLines(channel,
                                   plotT0MuS, plotT1MuS,
                                   plotWidth, plotHeight,
                                   heightFraction,
                                   range);
    for (auto &lines : linesToPlot)
    {
        pImpl->mLinesToPlot.push_back(std::pair{waveformType,
                                                std::move(lines)});
    }
*/
}

/// The shape in local coordinates
template<class T>
QPainterPath ChannelItem<T>::shape() const
{
    return pImpl->mLocalBoundsPainterPath;
}

/// Bounding rect
template<class T>
QRectF ChannelItem<T>::boundingRect() const
{
    return pImpl->mGlobalBounds;
}

template<class T>
void ChannelItem<T>::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *,
                           QWidget *)
{
    drawGrid(painter);
    drawName(painter);
    for (const auto &linesToPlot : pImpl->mLinesToPlot)
    {
        painter->setPen(waveformDescriptorToPen(linesToPlot.first));
        painter->drawLines(linesToPlot.second);
    }
}

/// Set the channel's name
template<class T>
void ChannelItem<T>::setName(const QString &name)
{
    pImpl->mName = name;
}

/// Draw the channel's name
template<class T>
void ChannelItem<T>::drawName(QPainter *painter)
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
template<class T>
void ChannelItem<T>::drawGrid(QPainter *painter)
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
    auto dtGrid = static_cast<double> (tDurationMuS)/(pImpl->mMajorTicks - 1);
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
        if (tDurationMuS < 1000000) // 10 second -> MM:SS.SSS
        {
            tString = tLabel.toString("mm:ss.zzz");
        }
/*
        else if (tDurationMuS < 10000000) // 10 seconds -> MM:SS.SS
        {
std::cout << "this one" << std::endl;
            tString = tLabel.toString("mm:ss.z");
        }
*/
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
template<class T>
void ChannelItem<T>::setAbsoluteTimeLimits(
    const std::pair<std::chrono::microseconds, 
                    std::chrono::microseconds> &plotLimits)
{
    if (plotLimits.first > plotLimits.second)
    {
        throw std::invalid_argument("plotLimits.first > plotLimits.second");
    }
    // Possibly redraw
    bool lRedraw = false;
    if (pImpl->mPlotStartTime != plotLimits.first || 
        pImpl->mPlotEndTime   != plotLimits.second) 
    {
        lRedraw = true;
    }
    pImpl->mPlotStartTime = plotLimits.first;
    pImpl->mPlotEndTime = plotLimits.second;
    // Drawing event necessary.  Trigger event.
    if (lRedraw)
    {
        pImpl->redrawWaveform();
    }
}

/// Current
template<class T>
void ChannelItem<T>::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        auto xCursorPosition = static_cast<double> (event->pos().x());
        auto boundingRectangle = boundingRect();
        auto xLeft  = boundingRectangle.left();
        auto xRight = boundingRectangle.right();
        auto width  = boundingRectangle.width();
        constexpr qreal one{1}; 
        auto xPosition = (xCursorPosition - xLeft)/std::max(one, width);
        auto tDuration = pImpl->mPlotEndTime - pImpl->mPlotStartTime;
        auto iCursorTime = pImpl->mPlotStartTime.count()
                         + static_cast<int64_t>
                           (std::round(tDuration.count()*xPosition));
        auto cursorTimeMilliSeconds
            = static_cast<qint64> (std::round(iCursorTime/1000.));
        auto tLabel = QDateTime::fromMSecsSinceEpoch(cursorTimeMilliSeconds,
                                                     Qt::UTC);
        QString tString;
        tString = tLabel.toString("mm:ss.z");
        qDebug() << pImpl->mName
                 << " " << xCursorPosition << xLeft << xRight << tLabel;
        event->accept();
    } 
    event->ignore(); 
}

///--------------------------------------------------------------------------///
///                             Template Instantiation                       ///
///--------------------------------------------------------------------------///
/*
template QPhase::Widgets::Waveforms::ChannelItem::ChannelItem(
    const QPhase::Waveforms::Channel<double> &,
    const QRectF &, const WaveformType, QGraphicsItem *);
template QPhase::Widgets::Waveforms::ChannelItem::ChannelItem(
    const QPhase::Waveforms::Channel<float> &,
    const QRectF &, const WaveformType, QGraphicsItem *);
template void QPhase::Widgets::Waveforms::ChannelItem::setWaveform(
    const QPhase::Waveforms::Channel<double> &,
    const WaveformType);
template void QPhase::Widgets::Waveforms::ChannelItem::setWaveform(
    const QPhase::Waveforms::Channel<float> &,
    const WaveformType);
*/
template class QPhase::Widgets::Waveforms::ChannelItem<double>;
template class QPhase::Widgets::Waveforms::ChannelItem<float>;
