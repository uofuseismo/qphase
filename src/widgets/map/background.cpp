#include <QGeoView/QGVMap.h>
#include <QGeoView/QGVLayerBing.h>
#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetText.h>
#include "qphase/widgets/map/background.hpp"
#include "qphase/widgets/map/QGVMapBox.hpp"

using namespace QPhase::Widgets::Map;

class Background::BackgroundImpl
{
public:
    BackgroundImpl()
    {
        const QString customURI = "http://c.tile.opentopomap.org/${z}/${x}/${y}.png";
        mLayers.push_back({Background::Layer::OSM,
                           new QGVLayerOSM(customURI)});
        mLayers.push_back({Background::Layer::BING_SATELLITE,
                           new QGVLayerBing(QGV::TilesType::Satellite)});
        mLayers.push_back({Background::Layer::MAPBOX_SATELLITE_STREETS,
                           new QGVLayerMapBox(QGV::TilesType::Satellite)});
    }
    QList<QPair<Background::Layer, QGVLayer *>> mLayers;
    QGVMap *mGeoMapPointer = nullptr;
    QGVWidgetText *mFooter = nullptr;
    bool mInitialized = false;
};

/// C'tor
Background::Background() :
    pImpl(std::make_unique<BackgroundImpl> ())
{
}

/// Destructor
Background::~Background() = default;

/// Initialize the background
void Background::initialize(QGVMap *geoMap)
{
    if (geoMap == nullptr){throw std::invalid_argument("geoMap is NULL");}
    // Get copy of pointer
    pImpl->mGeoMapPointer = geoMap;
    // Add layers to map and disable them
    for (auto &pair : pImpl->mLayers)
    {
        auto layer = pair.second;
        layer->setVisible(false);
        geoMap->addItem(layer);
    }
    // Create a footer
    pImpl->mFooter = new QGVWidgetText();
    pImpl->mGeoMapPointer->addWidget(pImpl->mFooter);
    pImpl->mInitialized = true;
}

/// Initialized?
bool Background::isInitialized() const noexcept
{
    return pImpl->mInitialized;
}

/// Select the background layer to view
void Background::selectLayer(const Background::Layer backgroundLayer)
{
    if (!isInitialized())
    {
        qCritical("Background layers not initialized");
    }
    // Enable the selected layer
    for (auto &pair : pImpl->mLayers)
    {
        auto layer = pair.second;
        if (pair.first == backgroundLayer)
        {
            if (layer->isSelected()){return;}
            layer->setVisible(true);
            pImpl->mFooter->setText(layer->getName() + ", " + layer->getDescription());
        }
    }
    // Hide old background layer             
    for (auto &pair : pImpl->mLayers)
    {
        auto layer = pair.second;
        if (pair.first != backgroundLayer)
        {
            layer->setVisible(false);
        }
    }
}
