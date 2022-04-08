#ifndef QPHASE_WIDGETS_MAP_EVENT_HPP
#define QPHASE_WIDGETS_MAP_EVENT_HPP
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
/// @class Event "event.hpp" "qphase/widgets/map/event.hpp"
/// @brief Draws an event on the map as a circle.
/// @copyright Ben Baker (University of Utah) distributed under the MIT License.
class Event : public QGVDrawItem
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Event();
    /// @brief Copy constructor.
    /// @param[in] event  The event class from which to initialize this class. 
    Event(const Event &event);
    /// @brief Move c'tor
    /// @param[in,out] event  The event class from which to initialize this
    ///                       class.  On exit, event's behavior is undefined.
    Event(Event &&event) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment
    /// @param[in] event  The event class to copy to this.
    /// @result A deep copy of the input event.
    Event& operator=(const Event &event);
    /// @brief Move assignment
    /// @param[in,out] event  The event class whose memory will be moved to
    ///                       this.  On exit, event's behavior is undefined.
    /// @result The memory from event moved to this.
    Event& operator=(Event &&event) noexcept;
    /// @}

    /// @name Properties
    /// @{

    /// @brief Sets the event's latitude and longitude.
    /// @param[in] latLon  The latitude and longitude in degrees.
    void setLatitudeLongitude(const std::pair<double, double> &latLon);
    /// @brief Sets the event's depth.
    /// @param[in] depth  The latitude and longitude in kilometers.
    void setDepth(double depth) noexcept;
    /// @brief Sets the event origin time.
    /// @param[in] time  The event's origin time (UTC) in seconds since the
    ///                  epoch.
    void setOriginTime(double time) noexcept;
    /// @brief Sets the event origin time.
    /// @param[in] time  The event's origin time (UTC) in microseconds since
    ///                  the epoch.
    void setOriginTime(const std::chrono::microseconds &time) noexcept;
    /// @brief Sets the event identifier.
    /// @param[in] identifier  The event's identifier.
    void setIdentifier(const std::string &identifier);
    /// @brief Sets the event identifier.  Commonly, this is just a number
    ///        at a regional seismic network.
    /// @param[in] identifier  The event's identifier.
    void setIdentifier(uint64_t identifier);
    /// @brief Sets the magnitude.
    /// @param[in] magnitude      The magnitude.
    /// @param[in] magnitudeType  The magnitude type.  Note, this will be
    ///                           prepended by "M" so there's no reason to
    ///                           enter Ml for a Richter magnitude.
    void setMagnitude(double magnitude,
                      const std::string &magnitudeType = "l");
    /// @brief Sets the event type - e.g., "Earthquake" or "Quarry Blast".
    /// @param[in] eventType  The event type.
    void setType(const std::string &eventType);

    /// @brief Sets the size (in pixels) of the circle.
    /// @note The default is 15.
    /// @throws std::invalid_argument if this is negative.
    void setSize(double size);
    /// @brief Sets the pen properties that will draw the outline.
    ///        This includes line color, thickness, dashed vs. solid, etc.
    void setPen(const QPen &pen);
    /// @brief Sets the fill color.
    void setFillColor(const QColor &color);
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~Event() override;
    /// @}

private:
    void onProjection(QGVMap *geoMap) override;
    [[nodiscard]] QPainterPath projShape() const override;
    void projPaint(QPainter *painter) override;
    [[nodiscard]] QString projTooltip(const QPointF &projPos) const override;
private:
    class EventImpl;
    std::unique_ptr<EventImpl> pImpl;
};
}
#endif
