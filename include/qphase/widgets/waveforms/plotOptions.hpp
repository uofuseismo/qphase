#ifndef QPHASE_WIDGETS_WAVEFORMS_PLOT_OPTIONS_HPP
#define QPHASE_WIDGETS_WAVEFORMS_PLOT_OPTIONS_HPP
#include <memory>
#include "qphase/widgets/waveforms/enums.hpp"
namespace QPhase::Widgets::Waveforms
{
/// @class PlotOptions "plotOptions.hpp" "qphase/widgets/waveforms/plotOptions.hpp"
/// @brief Options for plotting waveforms with the station view.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class PlotOptions
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    PlotOptions();
    /// @brief Copy constructor.
    /// @param[in] options  The plot options from which to initialize 
    ///                     this class.
    PlotOptions(const PlotOptions &options);
    /// @brief Move constructor.
    /// @param[in,out] options  The plot options from which to initialize
    ///                         this class.  On exit, options's behavior
    ///                         is undefined.
    PlotOptions(PlotOptions &&options) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] options  The options to copy to this.
    /// @result A deep copy of the options.
    PlotOptions& operator=(const PlotOptions &options);
    /// @brief Move assignment.
    /// @param[in,out] options  The options whose memory will be moved to this.
    ///                         On exit, options's behavior is undefined.
    /// @result The memory from options moved to this.
    PlotOptions& operator=(PlotOptions &&options) noexcept;
    /// @}

    /// @brief Sets the station ordering.
    /// @param[in] ordering   Defines the ordering of stations.
    void setStationOrdering(const StationOrdering ordering) noexcept;
    /// @result The station ordering.
    [[nodiscard]] StationOrdering getStationOrdering() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor.
    ~PlotOptions();
    /// @}
private:
    class PlotOptionsImpl;
    std::unique_ptr<PlotOptionsImpl> pImpl;
};
}
#endif
