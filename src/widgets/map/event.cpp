#include <array>
#include <sstream>
#include <QBrush>
#include <QDateTime>
#include <QPainter>
#include <QPen>
#include <QString>
#include <chrono>
#include "qphase/widgets/map/event.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/magnitude.hpp"

using namespace QPhase::Widgets::Map;

class Event::EventImpl
{
public:
    EventImpl()
    {
        mPen.setWidth(0);
        mPen.setColor(Qt::black);
        mPen.setCosmetic(true);
    }
    QGV::GeoPos mGeoPosition;
    QRectF mProjRect;
    QString mIdentifier;
    QString mMagnitudeType;
    QString mType;
    QPen mPen;
    QColor mBrushColor{Qt::blue};
    Qt::BrushStyle mBrushStyle = Qt::BrushStyle::SolidPattern;
    QString mToolTip{"Event information"};
    qreal mSize = 11; 
    qreal mDepth = 0;
    qreal mMagnitude = 0;
    std::chrono::microseconds mOriginTime{0};
    bool mHaveLatitudeAndLongitude = false;
    bool mHaveDepth = false;
    bool mHaveMagnitude = false;
    bool mHaveOriginTime = false;
    bool mFill = true;
};

/// Constructor
Event::Event() :
    pImpl(std::make_unique<EventImpl> ()) 
{
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Copy c'tor
Event::Event(const Event &event)
{
    *this = event;
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Move c'tor
Event::Event(Event &&event) noexcept
{
    *this = std::move(event);
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Copy assignment
Event& Event::operator=(const Event &event)
{
    if (&event == this){return *this;}
    pImpl = std::make_unique<EventImpl> (*event.pImpl);
    return *this;
}

/// Constructor from event
Event::Event(const QPhase::Database::Internal::Event &eventData) :
    pImpl(std::make_unique<EventImpl> ()) 
{
    Event event;
    // Most important thing is the locations (these will throw)
    auto origin = eventData.getOrigin();
    event.setLatitudeLongitude(std::pair{origin.getLatitude(),
                                         origin.getLongitude()});
    if (origin.haveTime()){event.setOriginTime(origin.getTime());}
    if (origin.haveDepth()){event.setDepth(origin.getDepth());}
    // Some auxiliary stuff
    if (eventData.haveMagnitude())
    {
        auto magnitude = eventData.getMagnitude();
        std::string type = "?";
        if (magnitude.haveValue())
        {
            if (magnitude.haveType()){type = magnitude.getType();}
            event.setMagnitude(magnitude.getValue(), type);
        }
    }
    if (eventData.haveIdentifier())
    {
        event.setIdentifier(eventData.getIdentifier());
    }
    *this = event;
}

/// Move assignment
Event& Event::operator=(Event &&event) noexcept
{
    if (&event == this){return *this;}
    pImpl = std::move(event.pImpl);
    return *this;
}

/// Destructor
Event::~Event() = default;

/// Calculates how to draw the event on the map
void Event::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    auto projLocation = geoMap->getProjection()->geoToProj(pImpl->mGeoPosition);
    auto x0 = static_cast<qreal> (projLocation.x() - pImpl->mSize/2);
    auto y0 = static_cast<qreal> (projLocation.y() - pImpl->mSize/2);
    pImpl->mProjRect = QRectF(x0, y0,
                              static_cast<qreal> (pImpl->mSize),
                              static_cast<qreal> (pImpl->mSize));
}

QPainterPath Event::projShape() const
{
    QPainterPath path;
    if (!pImpl->mHaveLatitudeAndLongitude)
    {
         return path;
    }
    path.addEllipse(pImpl->mProjRect);
    return path;
}

/// Actually handle the paint event
void Event::projPaint(QPainter *painter)
{
    painter->setPen(pImpl->mPen);
    auto eventShape = projShape();
    painter->drawPath(eventShape);
    if (pImpl->mFill)
    {
        QBrush brush(pImpl->mBrushColor, pImpl->mBrushStyle);
        painter->fillPath(eventShape, brush);
    }
}

/// Sets the event's location
void Event::setLatitudeLongitude(const std::pair<double, double> &latLon)
{
    pImpl->mGeoPosition.setLat(latLon.first);
    pImpl->mGeoPosition.setLon(latLon.second);
    pImpl->mHaveLatitudeAndLongitude = true;
}

/// Sets the event's location
void Event::setDepth(const double depth) noexcept
{
    pImpl->mDepth = depth;
    pImpl->mHaveDepth = true;
}

void Event::setIdentifier(const uint64_t identifier)
{
    setIdentifier(std::to_string(identifier));
}

void Event::setIdentifier(const std::string &identifier)
{
    pImpl->mIdentifier = QString::fromStdString(identifier);
}

/// Sets the magnitude 
void Event::setMagnitude(const double magnitude,
                         const std::string &magnitudeType)
{
    pImpl->mMagnitude = magnitude;
    // AQMS will put event in database as 'd' or 'l'
    if (magnitudeType.size() == 1)
    {
        pImpl->mMagnitudeType = "M"
                              + QString::fromStdString(magnitudeType);
    }
    else
    {
        pImpl->mMagnitudeType = QString::fromStdString(magnitudeType);
    }
    pImpl->mHaveMagnitude = true;
}

/// Sets the event type
void Event::setType(const std::string &eventType)
{
    pImpl->mType = QString::fromStdString(eventType);
}

/// Sets the origin time
[[maybe_unused]]
void Event::setOriginTime(const double time) noexcept
{
    std::chrono::microseconds
        tMicroSeconds{static_cast<int64_t> (std::round(time*1.e6))};
    setOriginTime(tMicroSeconds);
}

void Event::setOriginTime(const std::chrono::microseconds &time) noexcept
{
    pImpl->mOriginTime = time;
    pImpl->mHaveOriginTime = true;
}

/// Sets the size
void Event::setSize(const double size)
{
    if (size < 0){throw std::invalid_argument("Size cannot be negative");}
    pImpl->mSize = static_cast<qreal> (size);
}

/// Sets the fill color
void Event::setFillColor(const QColor &color)
{
    pImpl->mBrushColor = color;
}

/// Returns the event's name as as tooltip
QString Event::projTooltip(const QPointF &projPos) const
{
    QString result;
    if (!pImpl->mIdentifier.isEmpty())
    {
        if (!pImpl->mType.isEmpty())
        {
            result = pImpl->mType + ": " + pImpl->mIdentifier + "\n";
        }
        else
        {
            result = "Event: " + pImpl->mIdentifier + "\n";
        }
    }
    else
    {
        if (!pImpl->mType.isEmpty())
        {
            result = pImpl->mType + ": ID Unknown\n";
        }
        else
        {
            result = "Event: ID Unknown\n";
        }
    }

    if (pImpl->mHaveOriginTime)
    {
        std::stringstream ss;
        auto originTimeMilliSeconds
            = static_cast<qint64>
                    (static_cast<double> (pImpl->mOriginTime.count())*1.e-3);
        auto originTimeUTC
            = QDateTime::fromMSecsSinceEpoch(originTimeMilliSeconds,
                                             Qt::UTC);
        auto originTimeLocal
            = QDateTime::fromMSecsSinceEpoch(originTimeMilliSeconds,
                                             Qt::LocalTime); 
        result = result
               + "Origin Time (UTC): "
               //+ originTimeUTC.toString("yyyy-MM-dd hh:mm:ss.z") + " (UTC)\n"
               + originTimeUTC.toString(Qt::ISODate)
               + "\nOrigin Time (Local): "
               + originTimeLocal.toString(Qt::ISODate) + "\n";
    }

    if (pImpl->mHaveLatitudeAndLongitude && pImpl->mHaveDepth)
    {
        result = result + "\n(Latitude,Longitude,Depth) = ("
               + pImpl->mGeoPosition.latToString() + ","
               + pImpl->mGeoPosition.lonToString()  + ","
               + QString::number(pImpl->mDepth)+ ")";
    }
    else
    {
        if (pImpl->mHaveLatitudeAndLongitude)
        {
            result = result + "\n(Latitude,Longitude,Depth) = ("
                   + pImpl->mGeoPosition.latToString() + ","
                   + pImpl->mGeoPosition.lonToString()  + ")";
        }
        else if (pImpl->mHaveDepth)
        {
            result = result + "\nDepth = "
                  + QString::number(pImpl->mDepth);
        }
    }

    if (pImpl->mHaveMagnitude)
    {
        result = result + "\nMagnitude: " + QString::number(pImpl->mMagnitude);
        if (!pImpl->mMagnitudeType.isEmpty())
        {
            result = result + " " + pImpl->mMagnitudeType;
        }
    }

    return result;
}

/// Allows user to set pen
void QPhase::Widgets::Map::Event::setPen(const QPen &pen)
{
    pImpl->mPen = pen;
}
