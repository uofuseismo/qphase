#ifndef QPHASE_WAVEFORMS_CHANNEL_HPP
#define QPHASE_WAVEFORMS_CHANNEL_HPP
#include <memory>
namespace QPhase::Waveforms
{
template<class T> class Waveform;
class SimpleResponse;
}
namespace QPhase::Waveforms
{
/// @class Channel "channel.hpp" "qphase/waveforms/channel.hpp"
/// @brief A sensor's channel is the waveform and accompanying metadata.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T>
class Channel
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Channel();
    /// @brief Copy constructor.
    /// @param[in] channel  The channel from which to iniitialize this class.
    Channel(const Channel &channel);
    /// @brief Move constructor.
    Channel(Channel &&channel) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @param[in] channel  The channel to copy to this.
    /// @result A deep copy of the input channel.
    Channel& operator=(const Channel &channel);
    /// @param[in,out] channel  The channel whose memory will be moved to this.
    ///                         On exit, channel's behavior is undefined.
    /// @result The memory from channel moved to this.
    Channel& operator=(Channel &&channel) noexcept;
    /// @}

    /// @name Waveform
    /// @{

    /// @brief Sets the channel's waveform.
    /// @param[in] waveform  The channel's waveform.
    void setWaveform(const Waveform<T> &waveform);
    void setWaveform(Waveform<T> &&waveform);
    /// @result a reference to the channel waveform. 
    [[nodiscard]] const Waveform<T> &getWaveformReference() const;
    /// @result The channel's waveform.
    [[nodiscard]] Waveform<T> getWaveform() const;
    /// @result True indicates the waveform was set.
    [[nodiscard]] bool haveWaveform() const noexcept;
    /// @}

    /// @name Waveform Name
    /// @{

    /// TODO DELETE
    /// @brief Sets the station's network code.
    /// @param[in] network  The network code - e.g., UU.
    [[deprecated]] void setNetworkCode(const std::string &network);
    /// @result The network code.    
    /// @throws std::runtime_error if \c haveNetworkCode() is false.
    [[deprecated]] [[nodiscard]] std::string getNetworkCode() const;
    /// @result True indicates the network code was set.
    [[deprecated]] [[nodiscard]] bool haveNetworkCode() const noexcept;

    /// TODO DELETE
    /// @brief Sets the station's name.
    /// @param[in] station  The station's name - e.g., CTU.
    [[deprecated]] void setStationName(const std::string &station);
    /// @result The station name.
    /// @throws std::runtime_error if \c haveStation() is false.
    [[deprecated]] [[nodiscard]] std::string getStationName() const;
    /// @result True indicates the station name was set.
    [[deprecated]] [[nodiscard]] bool haveStationName() const noexcept;

    /// @brief Sets the channel code.
    /// @param[in] channel  The channel code - e.g., HHZ.
    /// @throws std::invalid_argument if this is not 3 letters.
    void setChannelCode(const std::string &channel);
    /// @result The channel code.
    [[nodiscard]] std::string getChannelCode() const;
    /// @result True indicates the channel code was set.
    [[nodiscard]] bool haveChannelCode() const noexcept;

    /// TODO DELETE
    /// @brief Sets the location code.
    /// @param[in] locationCode  The location code - e.g., 01.
    [[deprecated]] void setLocationCode(const std::string &locationCode);
    /// @result The location code.  By default this is empty.
    [[deprecated]] [[nodiscard]] std::string getLocationCode() const noexcept;
    /// @}

    /// @name Orientation
    /// @{

    /// @brief Sets the channel's dip.
    /// @param[in] dip  The channel's dip in degrees measured positive down
    ///                 from vertical.  This must be in the range [-90,90].
    /// @throws std::invalid_argument if the dip is out of range.
    void setDip(double dip);
    /// @result The dip in degrees.
    [[nodiscard]] double getDip() const;
    /// @result True indicates the dip was set.
    [[nodiscard]] bool haveDip() const noexcept; 

    /// @brief Sets the channel's azimuth.
    /// @param[in] azimuth  The channel's azimuth in degrees measured positive
    ///                     east from north.
    /// @throws std::invalid_argument if the azimuth is not in the
    ///         range [0,360].
    void setAzimuth(double azimuth);
    /// @result The channel's azimuth.
    /// @throws std::runtime_error if \c haveAzimuth() is false.
    [[nodiscard]] double getAzimuth() const;
    /// @result True indicates the channel's azimuth was set.
    [[nodiscard]] bool haveAzimuth() const noexcept;
    /// @}

    /// @name Simple Response (Optional)
    /// @{

    /// @brief Sets a simple response.  This is useful for quick-and-dirty
    ///        `deconvolutions' which amount to multiplying the trace by
    ///        a scalar.  This is appropriate for when the energy of signal of
    ///        interest is primarily captured by the flat passband of the 
    ///        instrument. 
    /// @param[in] response  The simple response.
    /// @throws std::invalid_argument if the scalar constant, input, or output
    ///         uses are not set.
    void setSimpleResponse(const SimpleResponse &response);
    /// @result The simple response.
    /// @throws std::runtime_error if \c haveSimpleResponse() is false.
    [[nodiscard]] SimpleResponse getSimpleResponse() const;
    /// @result True indicates the simple response was set.
    [[nodiscard]] bool haveSimpleResponse() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets class and releases memory.
    void clear() noexcept;
    /// @brief Destructor.
    ~Channel();
    /// @}
public:
    class ChannelImpl;
    std::unique_ptr<ChannelImpl> pImpl;
};
}
#endif
