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

}
#endif
