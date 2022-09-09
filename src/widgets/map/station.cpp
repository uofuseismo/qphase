#include <array>
#include <limits>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QString>
#include "qphase/widgets/map/station.hpp"
#include "qphase/database/internal/stationData.hpp"

using namespace QPhase::Widgets::Map;

class Station::StationImpl
{
public:
    StationImpl()
    {
        mPen.setWidth(0);
        mPen.setColor(Qt::black);
        mPen.setCosmetic(true);
    }
    QString mStationName;
    QString mNetwork;
    QString mName;
    QString mDescription;
    QGV::GeoPos mGeoPosition;
    std::array<QPointF, 3> mPosition;
    QPen mPen;
    QColor mBrushColor{Qt::red};
    Qt::BrushStyle mBrushStyle = Qt::BrushStyle::SolidPattern;
    QString mToolTip{"Network Station"};
    qreal mSize = 10; 
    double mElevation = std::numeric_limits<double>::lowest();
    bool mHaveLocation = false;
    bool mInverted = false;
    bool mFill = true;
};

/// Constructor
Station::Station() :
    pImpl(std::make_unique<StationImpl> ()) 
{
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Constructor from stationData
Station::Station(const QPhase::Database::Internal::StationData &stationData) :
    pImpl(std::make_unique<StationImpl> ())
{
    Station station;
    // Most important thing is the locations (these will throw)
    station.setLatitudeLongitude(
        std::pair {stationData.getLatitude(), stationData.getLongitude() });
    // Can probably work around this
    if (stationData.haveNetwork())
    {
        station.setNetwork(stationData.getNetwork());
    }
    if (stationData.haveStation())
    {
        station.setName(stationData.getStation());
    }
    if (stationData.haveElevation())
    {
        station.setElevation(stationData.getElevation());
    }
    auto description = stationData.getDescription();
    if (!description.empty())
    {
        station.setDescription(description);
    }
    *this = station;
}

/// Copy c'tor
Station::Station(const Station &station)
{
    *this = station;
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Move c'tor
Station::Station(Station &&station) noexcept
{
    *this = std::move(station);
    setFlags(QGV::ItemFlag::IgnoreScale);
    setSelectable(true);
}

/// Copy assignment
Station& Station::operator=(const Station &station)
{
    if (&station == this){return *this;}
    pImpl = std::make_unique<StationImpl> (*station.pImpl);
    return *this;
}

/// Move assignment
Station& Station::operator=(Station &&station) noexcept
{
    if (&station == this){return *this;}
    pImpl = std::move(station.pImpl);
    return *this;
}

/// Destructor
Station::~Station() = default;

/// Invert
void Station::enableInverted() noexcept
{
    pImpl->mInverted = true;
}

void Station::disableInverted() noexcept
{
    pImpl->mInverted = false;
}

void Station::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    auto projLocation = geoMap->getProjection()->geoToProj(pImpl->mGeoPosition);
    if (!pImpl->mInverted)
    {
        pImpl->mPosition[0] = QPointF(projLocation.x() - pImpl->mSize/2,
                                      projLocation.y() - pImpl->mSize/2);
        pImpl->mPosition[1] = QPointF(projLocation.x() + pImpl->mSize/2,
                                      projLocation.y() - pImpl->mSize/2);
        pImpl->mPosition[2] = QPointF(projLocation.x(),
                                      projLocation.y() + pImpl->mSize/2);
    }
    else
    {
        pImpl->mPosition[0] = QPointF(projLocation.x() - pImpl->mSize/2,
                                      projLocation.y() + pImpl->mSize/2);
        pImpl->mPosition[1] = QPointF(projLocation.x() + pImpl->mSize/2,
                                      projLocation.y() + pImpl->mSize/2);
        pImpl->mPosition[2] = QPointF(projLocation.x(),
                                      projLocation.y() - pImpl->mSize/2);
    }
}

QPainterPath Station::projShape() const
{
    QPainterPath path;
    if (!pImpl->mHaveLocation){return path;}
    path.moveTo(pImpl->mPosition[0]);
    for (auto &point : pImpl->mPosition)
    {
        path.lineTo(point);
    }
    path.lineTo(pImpl->mPosition[0]);
    return path;
}

/// Actually handle the paint event
void Station::projPaint(QPainter *painter)
{
    painter->setPen(pImpl->mPen);
    auto stationShape = projShape();
    painter->drawPath(stationShape);
    if (pImpl->mFill)
    {
        QBrush brush(pImpl->mBrushColor, pImpl->mBrushStyle);
        painter->fillPath(stationShape, brush);
    }
}

/// Sets the station's location
void Station::setLatitudeLongitude(const std::pair<double, double> &latLon)
{
    pImpl->mGeoPosition.setLat(latLon.first);
    pImpl->mGeoPosition.setLon(latLon.second);
    pImpl->mHaveLocation = true;
}

/// Sets the station's elevation
void Station::setElevation(const double elevation) noexcept
{
    pImpl->mElevation = elevation;
}

/// Sets the station's name
void Station::setName(const std::string &name) noexcept
{
    pImpl->mStationName = QString::fromStdString(name);
    pImpl->mName = pImpl->mNetwork + "." + pImpl->mStationName;
}

/// Sets the station's description
void Station::setDescription(const std::string &description) noexcept
{
    pImpl->mDescription = QString::fromStdString(description);
}

/// Sets the station's network
void Station::setNetwork(const std::string &network) noexcept
{
    pImpl->mNetwork = QString::fromStdString(network);
    pImpl->mName = pImpl->mNetwork + "." + pImpl->mStationName;
}

/// Sets the size
void Station::setSize(const double size)
{
    if (size < 0){throw std::invalid_argument("Size cannot be negative");}
    pImpl->mSize = static_cast<qreal> (size);
}

/// Sets the fill color
void Station::setFillColor(const QColor &color)
{
    pImpl->mBrushColor = color;
}

/// Returns the station's name as as tooltip
QString Station::projTooltip(const QPointF &projPos) const
{
    auto result =   "Name: " + pImpl->mName
                + "\nPosition: ("
                + pImpl->mGeoPosition.latToString() + ","
                + pImpl->mGeoPosition.lonToString();
    if (pImpl->mElevation > std::numeric_limits<double>::lowest())
    {
        QString cElev = QString::number(pImpl->mElevation);
        result = result + "," + cElev;
    }
    result = result + ")";
    if (pImpl->mDescription.size() > 0L)
    {
        result = result
               + "\nDescription: "+ pImpl->mDescription;
    }
    return result;
}
