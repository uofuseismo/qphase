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
/// @brief Defines the waveform type to plot.
enum class WaveformType
{
    Seismogram  /*!< This is a seismogram waveform. */
};
/// @brief Defines the ordering of stations in the station view.
enum class StationOrdering
{
    Alphabetical, /*!< Sorts stations alphabetically by name - e.g.,
                       NETWORK.STATION. This is the default ordering
                       strategy. */
    Distance,     /*!< Sorts stations by distance from the event. */ 
    Azimuth       /*!< Sorts stations by station azimuth.  This is
                       increases positive east of north. */
};
}
#endif
