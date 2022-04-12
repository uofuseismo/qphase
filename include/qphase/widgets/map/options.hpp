#ifndef QPHASE_WIDGETS_MAP_OPTIONS_HPP
#define QPHASE_WIDGETS_MAP_OPTIONS_HPP
#include <memory>
namespace QGV 
{
 class GeoPos;
} 
namespace QPhase::Widgets::Map
{
/// @class Options "options.hpp" "qphase/widgets/map/options.hpp"
/// @brief Defines options for the map.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Options
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Options();
    /// @brief Copy constructor.
    /// @param[in] options  The options class from which to initialize
    ///                     this class.
    Options(const Options &options);
    /// @brief Move constructor.
    /// @param[in,out] options  The options class from which to initialize
    ///                         this class.  On exit, options's behavior is
    ///                         undefined.
    Options(Options &&options) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy constructor.
    /// @param[in] options  The options class to copy to this.
    /// @result A deep copy of the map options.
    Options& operator=(const Options &options);
    /// @brief Move constructor.
    /// @param[in,out] options  The options class whose memory will be moved
    ///                         to this.  On exit, options's behavior is 
    ///                         undefined.
    /// @result The memory from options moved to this.
    Options& operator=(Options &&options) noexcept;
    /// @}

    /// @name Parameters
    /// @{

    /// @brief Sets the map center.
    /// @param[in] center  The latitude (center.first) and longitude
    ///                    (center.second) in degrees.
    /// @throws std::invalid_argument if the latitude is out of bounds.
    void setCenter(const std::pair<double, double> &center);
    /// @result The map center.
    [[nodiscard]] std::pair<double, double> getCenter() const noexcept;

    /// @brief Adds a boundary.
    /// @note These are closed loops.
    void addBoundary(const std::vector<std::pair<double, double>> &boundary); 
    /// @result The map boundaries.
    [[nodiscard]] std::vector<std::vector<std::pair<double, double>>> getBoundaries() const noexcept;

    /// @brief The directory where map tiles will be saved.
    void setTileCacheDirectory(const std::string &directory);
    /// @result The tile cache directory.
    [[nodiscard]] std::string getTileCacheDirectory() const noexcept;

    /// @brief Sets the map scale where the scale is inversely proportional to
    ///        km.  So if you want a map about 100 km wide use 1/100.  If you
    ///        want a regional map use 1/5000.
    /// @param[in] scale  The map scale in units of 1/km. 
    /// @throws std::invalid_argument if the scale is not positive.
    void setScale(double scale);
    /// @result Map scale in 1/km.
    [[nodiscard]] double getScale() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the map options.
    void clear() noexcept;
    /// @brief Destructor.
    ~Options();
    /// @}
private:
    class OptionsImpl;
    std::unique_ptr<OptionsImpl> pImpl;
};
}
#endif
