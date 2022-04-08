#include <QGeoView/QGVMap.h>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include "qphase/widgets/map/map.hpp"
#include "qphase/widgets/map/background.hpp"
#include "qphase/widgets/map/boundary.hpp"
#include "qphase/widgets/map/event.hpp"
#include "qphase/widgets/map/station.hpp"
#include "qphase/widgets/map/map.hpp"
#include "qphase/database/internal/stationData.hpp"

using namespace QPhase::Widgets::Map;

namespace
{

QGV::GeoPos pairToGeoPos(const std::pair<double, double> &latLon)
{
    QGV::GeoPos result(latLon.first, latLon.second);
    return result;
}

std::vector<QGV::GeoPos> pairToGeoPos(
    const std::vector<std::pair<double, double>> &boundary)
{
    std::vector<QGV::GeoPos> result;
    result.reserve(boundary.size());
    for (const auto &p : boundary)
    {
        result.push_back(pairToGeoPos(p));
    }
    return result;
}

}

class Map::MapImpl
{
public:
    MapImpl() :
        mBackground(new Background())
    {
/*
        // Map center
        mMapCenter = pairToGeoPos(mOptions.getCenter());
        // Boundaries
        auto boundaries = mOptions.getBoundaries();
        for (const auto &b : boundaries)
        {
            mBoundaries.push_back(pairToGeoPos(b));
        }
        /// Scale
        mScale = mOptions.getScale();
*/
    }
/*
///private:
    Options mOptions;
*/
    Background *mBackground{nullptr};
    QNetworkAccessManager *mNetworkManager{nullptr};
    QNetworkDiskCache *mNetworkCache{nullptr};
    std::vector<QGVItem *> mItems;
    std::vector<std::vector<QGV::GeoPos>> mBoundaries;
    std::vector<Station> mStations;
    std::vector<Event> mEvents;
    QGV::GeoPos mMapCenter;
    double mScale = 1/5000.;  // Scale is inverse of kilometers
    Background::Layer mBackgroundLayer = Background::Layer::BING_SATELLITE;
};

/// C'tor
Map::Map(QWidget *parent) :
    QGVMap(parent),
    pImpl(std::make_unique<MapImpl> ()) 
{
    // Need ability to `download' maps.  If possible read already downloaded
    // maps.
//    auto mapCache = pImpl->mOptions.getTileCacheDirectory();
    pImpl->mNetworkCache = new QNetworkDiskCache(this);
//    pImpl->mNetworkCache->setCacheDirectory(mapCache.c_str());
    pImpl->mNetworkManager = new QNetworkAccessManager(this);
    pImpl->mNetworkManager->setCache(pImpl->mNetworkCache);
    QGV::setNetworkManager(pImpl->mNetworkManager);
    // Set the background once and for all
    pImpl->mBackground = new Background();
    pImpl->mBackground->initialize(this);
    pImpl->mBackground->selectLayer(pImpl->mBackgroundLayer);
}

/// Destructor
Map::~Map() = default;

/// Background selector
void Map::selectBackground(const Background::Layer layer)
{
    pImpl->mBackgroundLayer = layer;
    pImpl->mBackground->selectLayer(pImpl->mBackgroundLayer);
}

/// Update the stations
void Map::updateStations(const std::vector<Station> &stations)
{
    pImpl->mStations = stations;
    draw();
}

/// Update the events
void Map::updateEvents(const std::vector<Event> &events)
{
    pImpl->mEvents = events;
    draw();
}

/// Draw the map
void Map::draw()
{
    // Clear the items from the map
    for (auto &item : pImpl->mItems)
    {
        removeItem(item);
    }
    pImpl->mItems.clear();
    // Add boundaries
    for (const auto &boundary : pImpl->mBoundaries)
    {
        auto boundaryItem = new Boundary();
        boundaryItem->setBoundary(boundary);
        addItem(boundaryItem);
        pImpl->mItems.push_back(boundaryItem);
    }
    // Plot the stations
    for (const auto &station : pImpl->mStations)
    {
        auto stationItem = new Station(station);
        stationItem->setVisible(true);
        addItem(stationItem);
        pImpl->mItems.push_back(stationItem);
    }
    // Plot the earthquakes
    for (const auto &event : pImpl->mEvents)
    {
        auto eventItem = new Event(event);
        eventItem->setVisible(true);
        addItem(eventItem);
        pImpl->mItems.push_back(eventItem);
    }
}
