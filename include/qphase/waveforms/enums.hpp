#ifndef QPHASE_WAVEFORMS_ENUMS_HPP
#define QPHASE_WAVEFORMS_ENUMS_HPP
namespace QPhase::Waveforms
{
enum class Units
{
    Counts,        /*!< Digital counts. */
    Displacement,  /*!< Displacement in meters. */
    Velocity,      /*!< Velocity in meters per second. */
    Acceleration   /*!< Acceleration in meters per second per second. */
};
    
enum class SensorType
{
    Fairfield_ZLand_3C_Node
};
}
#endif
