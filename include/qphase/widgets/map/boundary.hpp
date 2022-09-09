#ifndef QPHASE_WIDGETS_MAP_BOUNDARY_HPP
#define QPHASE_WIDGETS_MAP_BOUNDARY_HPP
#include <memory>
#include <QGeoView/QGVDrawItem.h>
QT_BEGIN_NAMESPACE
class QPainterPath;
class QPainter;
class QColor;
class QPen;
class QString;
QT_END_NAMESPACE

namespace QGV
{
class GeoPos;
}
class QGVMap;

namespace QPhase::Widgets::Map
{
/// @class Boundary "boundary.hpp" "qphase/widgets/map/boundary.hpp"
/// @brief Draws a boundary on the map.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Boundary : public QGVDrawItem
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Boundary();
    /// @}

    /// @brief Sets the boundary which is defined by a set of (lat, lon) pairs.
    /// @note  This should be a closed loop - i.e., the last point is the same
    ///        The first point.
    void setBoundary(const std::vector<QGV::GeoPos> &boundary);
    /// @brief Sets the pen properties that will draw the outline.
    ///        This includes line color, thickness, dashed vs. solid, etc.
    [[maybe_unused]] void setPen(const QPen &pen);
    /// @brief When the mouse contacts the cursor this tooltip will
    ///        be displayed.
    [[maybe_unused]] void setToolTip(const QString &toolTip);

    /// @name Destructor
    /// @{

    /// @brief Destructor.
    ~Boundary() override;
    /// @}

    Boundary(const Boundary &) = delete;
    Boundary(Boundary &&) noexcept = delete;
    Boundary& operator=(const Boundary &) = delete;
    Boundary& operator=(Boundary &&) noexcept = delete;
private:
    void onProjection(QGVMap *geoMap) override;
    [[nodiscard]] QPainterPath projShape() const override;
    void projPaint(QPainter *painter) override;
    [[nodiscard]] QString projTooltip(const QPointF &projPos) const override;
private:
    class BoundaryImpl;
    std::unique_ptr<BoundaryImpl> pImpl;
};
}
#endif
