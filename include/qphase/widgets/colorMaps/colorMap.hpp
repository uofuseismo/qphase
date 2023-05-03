#ifndef QPHASE_COLOR_MAPS_COLOR_MAP_HPP
#define QPHASE_COLOR_MAPS_COLOR_MAP_HPP
#include <QColor>
namespace QPhase::Widgets::ColorMaps
{
/// @brief Defines the interpolation method.
enum class InterpolationMethod
{
    Nearest = 0, /*!< Nearest-neighbor interpolation. */
    Linear = 1   /*!< Linear interpolation. */
};
/// @class IColorMap "colorMap.hpp" "qphase/widgets/colorMaps/colorMap.hpp"
/// @brief Abstract base class defining a colormap.  This allows a function to 
///        evaluate the color for a value x in the range [x0, x1].
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class IColorMap
{
public:
    /// @brief Destructor.
    virtual ~IColorMap() = default;
    /// @result The interpolated value on the range [x0, x1].
    [[nodiscard]] virtual QColor evaluate(const double x) const = 0;
    /// @result True indicates that class is initialized.
    [[nodiscard]] virtual bool isInitialized() const noexcept = 0;
};
}
#endif
