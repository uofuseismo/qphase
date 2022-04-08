#include <QPainter>
#include <QPen>
#include <QString>
#include <vector>
#include "qphase/widgets/map/boundary.hpp"

using namespace QPhase::Widgets::Map;

class Boundary::BoundaryImpl
{
public:
    BoundaryImpl()
    {
        mPen.setWidth(3);
        mPen.setColor(Qt::gray);
        mPen.setCosmetic(true);
    }
    std::vector<QGV::GeoPos> mGeoPosBoundary;
    QVector<QPointF> mBoundary;
    QPen mPen;
    QString mToolTip{"Network Boundary"};
};

/// Constructor
Boundary::Boundary() :
    pImpl(std::make_unique<BoundaryImpl> ()) 
{
    setVisible(true);
}

/// Destructor
Boundary::~Boundary() = default;

/// Sets the boundary
void Boundary::setBoundary(const std::vector<QGV::GeoPos> &boundary)
{
    if (boundary.size() < 2)
    {
        qWarning("Boundary must have more than 2 points");
    }
    pImpl->mGeoPosBoundary = boundary;
    pImpl->mBoundary.clear();
}

void Boundary::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    pImpl->mBoundary.clear();
    pImpl->mBoundary.reserve(
            static_cast<qsizetype> (pImpl->mGeoPosBoundary.size()));
    for (const auto &point : pImpl->mGeoPosBoundary)
    {
        pImpl->mBoundary.push_back(geoMap->getProjection()->geoToProj(point));
    }
}

/// Sets the pen
[[maybe_unused]]
void Boundary::setPen(const QPen &pen)
{
    pImpl->mPen = pen;
}

/// Tool tip when contacting boundary
[[maybe_unused]]
void Boundary::setToolTip(const QString &toolTip)
{
    pImpl->mToolTip = toolTip;
}

QPainterPath Boundary::projShape() const
{
    QPainterPath path;
    if (pImpl->mBoundary.empty()){return path;}
    path.moveTo(pImpl->mBoundary[0]);
    for (auto &point : pImpl->mBoundary)
    {
        path.lineTo(point);
    }
    return path;
}

/// Actually handle the paint event
void Boundary::projPaint(QPainter *painter)
{
    painter->setPen(pImpl->mPen);
    painter->drawPath(projShape());
}

QString Boundary::projTooltip(const QPointF &projPos) const
{
    //auto geo = getMap()->getProjection()->projToGeo(projPos); 
    //return "Position: (" + geo.latToString() + "," + geo.lonToString() + ")";
    return pImpl->mToolTip;
}

