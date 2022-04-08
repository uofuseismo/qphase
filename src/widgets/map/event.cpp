#include <array>
#include <sstream>
#include <QBrush>
#include <QDateTime>
#include <QPainter>
#include <QPen>
#include <QString>
#include <chrono>
#include "qphase/widgets/map/event.hpp"

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
}

/// Sets the event type
void Event::setType(const std::string &eventType)
{
    pImpl->mType = QString::fromStdString(eventType);
}

/// Sets the origin time
void Event::setOriginTime(const double time) noexcept
{
//    pImpl->mOriginTime = Time::UTC(time);
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
//        std::stringstream ss;
//        ss << pImpl->mOriginTime;
//        result = result + "Origin Time: "
//               + QString::fromStdString(ss.str()) + " (UTC)\n";
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
        result = result + "Magnitude: " + QString::number(pImpl->mMagnitude);
        if (!pImpl->mMagnitudeType.isEmpty())
        {
            result = result + " " + pImpl->mMagnitudeType;
        }
    }

    return result;
}
