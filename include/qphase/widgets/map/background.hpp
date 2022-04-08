#ifndef QPHASE_WIDGETS_MAP_BACKGROUND_HPP
#define QPHASE_WIDGETS_MAP_BACKGROUND_HPP
#include <memory>
class QGVMap;
namespace QPhase::Widgets::Map
{
/// @class Background "background.hpp" "qphase/widgets/map/background.hpp"
/// @brief Defines the background map tiles on which to plot additional
///        items.  
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Background
{
public:
    /// @brief Supported map layer types.
    enum class Layer
    {   
        OSM,                      /*!< Open-Street Map relief map. */
        BING_SATELLITE,           /*!< Bing Satellite map. */
        MAPBOX_SATELLITE_STREETS  /*!< MapBox satellite-streets map. */ 
    };  
public:
    /// @brief Constructor.
    Background();
    /// @brief Destructor. 
    ~Background();
    /// @brief Sets a pointer to the QGV map to which the layers will be added.
    /// @param[in] geoMap  A pointer to the QGV map to set on this.
    /// @throws std::invalid_argument if geoMap is NULL.
    /// @note By this point the QGV map must have a network handler setup.
    void initialize(QGVMap *geoMap);
    /// @result True indicates the class is initialized.
    [[nodiscard]] bool isInitialized() const noexcept;
    /// @brief Allows the user to toggle between different background layers.
    void selectLayer(Background::Layer backgroundLayer);

    
    Background(const Background &) = delete;
    Background(Background &&) noexcept = delete;
    Background& operator=(const Background &) = delete;
    Background& operator=(Background &&) noexcept = delete;
private:
    class BackgroundImpl;
    std::unique_ptr<BackgroundImpl> pImpl;
};
}
#endif
