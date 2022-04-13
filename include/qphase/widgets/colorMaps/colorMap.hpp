#ifndef QPHASE_COLORMAPS_COLORMAP_HPP
#define QPHASE_COLORMAPS_COLORMAP_HPP
QT_BEGIN_NAMESPACE
 class QColor;
QT_END_NAMESPACE
namespace QPhase::Widgets::ColorMaps
{
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
    [[nodiscard]] bool isInitialized() const noexcept = 0;
};
}
#endif
