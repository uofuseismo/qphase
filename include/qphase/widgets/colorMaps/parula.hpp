#ifndef QURTS_COLORMAPS_PARULA_HPP
#define QURTS_COLORMAPS_PARULA_HPP
#include <memory>
#include "qphase/widgets/colorMaps/colorMap.hpp"
namespace QPhase::Widgets::ColorMaps
{
/// @class Parula "parula.hpp" "qphase/widgets/colorMaps/parula.hpp"
/// @brief The parula color map from Matlab.
/// @copyright Ben Baker (University of Utah) distributed under the MIT License.
class Parula : public IColorMap
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Parula();
    /// @brief Copy constructor.
    /// @param[in] parula  The color map from which to initialize this class.
    Parula(const Parula &parula);
    /// @brief Move constructor.
    /// @param[in,out] parula  The color map from which to initialize this
    ///                        this class.  On exit, parula's behavior is
    ///                        undefined.
    Parula(Parula &&parula) noexcept;
    /// @}

    /// @name Operators
    /// @{
 
    /// @brief Copy assignment operator.
    /// @param[in] parula  The color map to copy to this.
    /// @result A deep copy of the input parula class.
    Parula& operator=(const Parula &parula);
    /// @brief Move assignment operator.
    /// @param[in,out] parula  The color map whose memory will be moved to this.
    ///                        On exit, parula's behavior is undefined.
    /// @result The memory from parula moved to this.
    Parula& operator=(Parula &&parula) noexcept;
    /// @}

    /// @brief Initializes the interpolator to work on the range.
    /// @throws std::invalid_argument if range.first >= range.second.
    void initialize(const std::pair<double, double> &range);
    /// @result The interpolated color on the interval.
    [[nodiscard]] QColor evaluate(double x) const override;
    /// @result True indicates that class is initialized.
    [[nodiscard]] bool isInitialized() const noexcept override;

    /// @name Destructors
    /// @{

    ~Parula() override;
    /// @}
private:
    class ParulaImpl;
    std::unique_ptr<ParulaImpl> pImpl;
};
}
#endif
