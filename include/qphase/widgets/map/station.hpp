#ifndef QURTS_MAP_STATION_HPP
#define QURTS_MAP_STATION_HPP
#include <memory>
#include <QGeoView/QGVDrawItem.h>
QT_BEGIN_NAMESPACE
class QPainterPath;
class QPainter;
class QColor;
class QPen;
class QString;
QT_END_NAMESPACE

class QGVMap;

namespace QPhase::Widgets::Map
{
/// @class Station "station.hpp" "qphase/widgets/map/station.hpp"
/// @brief Draws a station on the map as a triangle.
/// @copyright Ben Baker (University of Utah) distributed under the MIT License.
class Station : public QGVDrawItem
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Station();
    /// @brief Copy constructor.
    /// @param[in] station  The station class from which to initialize this
    ///                     class.
    Station(const Station &station);
    /// @brief Move constructor.
    /// @param[in,out] station  The station class from which to initialize this
    ///                         class.  On exit, station's behavior is
    ///                         undefined.
    Station(Station &&station) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] station  The station to copy to this.
    /// @result A deep copy of the input station.
    Station& operator=(const Station &station);
    /// @brief Move assignment
    /// @param[in,out] station  The station whose memory will be moved to this.
    ///                         On exit, station's behavior is undefined.
    /// @result The memory from station moved to this.
    Station& operator=(Station &&station) noexcept;
    /// @}

    /// @name Properties
    /// @{

    /// @brief Sets the network name.
    /// @param[in] network  The station's network - e.g., UU.
    void setNetwork(const std::string &network) noexcept;
    /// @brief Sets the station's name.
    /// @param[in] station   The station's name - e.g., FORK.
    void setName(const std::string &name) noexcept;
    /// @brief Sets the station's latitude and longitude.
    void setLatitudeLongitude(const std::pair<double, double> &latLon);
    /// @brief Sets the station's elevation.
    void setElevation(double elevation) noexcept;
    /// @brief Sets the station's description.
    void setDescription(const std::string &description) noexcept;
    /// @brief Sets the pen properties that will draw the outline.
    ///        This includes line color, thickness, dashed vs. solid, etc.
    void setPen(const QPen &pen);
    /// @brief Sets the fill color.
    void setFillColor(const QColor &color);
    /// @brief Sets the size (in pixels) of the triangle.
    /// @note The default is 15.
    void setSize(double size);
    /// @brief Causes the station triangle to be inverted.
    void enableInverted() noexcept;
    /// @brief Causes the triangle to not be inverted.
    void disableInverted() noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~Station() override;
    /// @}
private: 
    void onProjection(QGVMap *geoMap) override;
    [[nodiscard]] virtual QPainterPath projShape() const override;
    void projPaint(QPainter *painter) override;
    [[nodiscard]] QString projTooltip(const QPointF &projPos) const override;
private:
    class StationImpl;
    std::unique_ptr<StationImpl> pImpl;
};
}
#endif
