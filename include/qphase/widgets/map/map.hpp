#ifndef QPHASE_WIDGETS_MAP_MAP_HPP
#define QPHASE_WIDGETS_MAP_MAP_HPP
#include <memory>
#include <QGeoView/QGVMap.h>
#include "qphase/widgets/map/background.hpp"

QT_BEGIN_NAMESPACE
class QWidget;
class QContextMenuEvent;
QT_END_NAMESPACE

namespace QPhase::Database::Internal
{
class StationData;
}
namespace QPhase::Widgets::Map
{
class Event;
class Station;
}

namespace QPhase::Widgets::Map
{
/// @name Map "map.hpp" "qphase/widgets/map/map.hpp"
/// @brief This widget creates the QGV map.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Map : public QGVMap
{
    //Q_OBJECT
public:
    /// @brief Constructor with a given parent.
    explicit Map(QWidget *parent = nullptr);
    /// @brief Selects the map's background.
    /// @param[in] layer  The background layer.
    void selectBackground(const Background::Layer layer);
    //void plotStationsFromAQMS(
    //    const std::vector<URTS::Database::AQMS::StationData> &stations);
    /// @brief Updates and plots the stations.
    /// @param[in] stations  The stations to plot. 
    void updateStations(const std::vector<Station> &stations);
    /// @brief Updates and plots the events.
    /// @param[in] events   The events to plot.
    void updateEvents(const std::vector<Event> &events);
    /// @brief Recenters the map.
    /// @param[in] latitudeAndLongitude  The latitude and longitude in degrees
    ///                                  that will be the map's new center.
    void recenter(const std::pair<double, double> &latitudeLongitude);
    /// @brief Draws the map.
    void draw();

    /// @brief Destructor.
    ~Map() override;
private:
    class MapImpl;
    std::unique_ptr<MapImpl> pImpl;
};
}
#endif
