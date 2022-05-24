#include <QFont>
#include <QMap>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QString>
#include <QVector>
#include "qphase/widgets/waveforms/postProcessing/traceItem.hpp"
#include "qphase/database/internal/arrival.hpp"

using namespace QPhase::Widgets::Waveforms::PostProcessing;

class TraceItem::TraceItemImpl
{
public:
    std::set<QPhase::Database::Internal::Arrival> mArrivals;
    QString mName;
    QPen mNamePen{Qt::black};
    QPen mWaveformPen{Qt::black, 0, Qt::SolidLine};
    QPen mBorderPen{Qt::black, 2};
    QPen mTimeLabelsPen{Qt::black};
    QPen mGridPen{Qt::gray, 0, Qt::DashLine};
    QPen mMajorTickPen{Qt::black, 0, Qt::SolidLine};
    QPen mMinorTickPen{Qt::black, 0, Qt::SolidLine};
    QFont mNameFont{"Monospace", 9, QFont::Normal, false};
    QFont mTimeLabelsFont{"Monospace", 11, QFont::Light, false};
    QRectF mLocalBounds{QRect{0, 0, 1, 1}};
    QRectF mGlobalBounds{mLocalBounds};
    QPainterPath mLocalBoundsPainterPath;
};

/// Constructor
TraceItem::TraceItem(const QRectF &globalBounds, QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    pImpl(std::make_unique<TraceItemImpl> ())
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

/// The shape in local coordaintes
QPainterPath TraceItem::shape() const
{
    return pImpl->mLocalBoundsPainterPath;
}

/// Bounding rect
QRectF TraceItem::boundingRect() const
{
    return pImpl->mGlobalBounds;
}

/// Sets the name
void TraceItem::setName(const std::string &name)
{
    pImpl->mName = QString::fromStdString(name);
}

std::string TraceItem::getName() const noexcept
{
    return pImpl->mName.toStdString();
}

void TraceItem::setNamePen(const QPen &pen)
{
    pImpl->mNamePen = pen;
}

void TraceItem::setNameFont(const QFont &font)
{
    pImpl->mNameFont = font;
}

/// Draw the station's name
void TraceItem::drawName(QPainter *painter)
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
    QRectF snclBox(width*0.005, boxBase, pixelWidth, pixelHeight);
    painter->drawText(snclBox, Qt::AlignLeft | Qt::AlignTop, pImpl->mName);
}

/// Draw the waveform
void TraceItem::paint(QPainter *painter, 
                      const QStyleOptionGraphicsItem *,
                      QWidget *)
{
    //drawGrid();
    drawName(painter);
}

/// Destructor
TraceItem::~TraceItem() = default;

