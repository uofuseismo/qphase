#ifndef QPHASE_WIDGETS_WAVEFORMS_ENUMS_HPP
#define QPHASE_WIDGETS_WAVEFORMS_ENUMS_HPP
namespace QPhase::Widgets::Waveforms
{

/// @brief Defines the time convention for the plot. 
enum class TimeConvention
{
    Absolute,  /*!< Absolute time. */
    Relative   /*!< All time is relative to a specified time. */
};
/// @brief Toggles between plotting vertical/horizontal/all channels.
enum class VisibleChannels 
{
    All,       /*!< Plot all waveforms. */
    Vertical,  /*!< Plot only vertical channels. */
    Horizontal /*!< Plot only horizontal channels. */
};

}
#endif
