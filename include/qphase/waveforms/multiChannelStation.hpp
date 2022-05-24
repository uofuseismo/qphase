#ifndef QPHASE_WAVEFORMS_MULTICHANNELSTATION_HPP
#define QPHASE_WAVEFORMS_MULTICHANNELSTATION_HPP
#include <memory>
namespace QPhase::Waveforms
{
template<class T> class Waveform;
};
namespace QPhase::Waveforms
{
class MultiChannelStation
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    MultiChannelStation();
    /// @brief Copy constructor.
    /// @param[in] station  The multi-channel station from which to initialize
    ///                     this class.

    /// @brief Move constructor.
    /// @param[in,out] station  The multi-channel station from which to
    ///                         initialize this class.  On exit, station's
    ///                         behavior is undefined.

    /// @}

    /// @name Operators
    /// @{

    /// @}

    /// @result The number of channels.
    [[nodiscard]] int getNumberOfChannels() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Resets the class and releases memory.
    void clear();
    /// @brief Destructor.
    ~MultiChannelStation();
    /// @}
private:
    class MultiChannelStationImpl;
    std::unique_ptr<MultiChannelStationImpl> pImpl;
};
}
#endif
