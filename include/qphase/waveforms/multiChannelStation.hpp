#ifndef QPHASE_WAVEFORMS_MULTI_CHANNEL_STATION_HPP
#define QPHASE_WAVEFORMS_MULTI_CHANNEL_STATION_HPP
#include <vector>
#include <memory>
namespace QPhase::Waveforms
{
template<class T> class Channel;
};
namespace QPhase::Waveforms
{
/// @class MultiChannelStation "multiChannelStation.hpp" "qphase/waveforms/multiChannelStation.hpp"
/// @brief Defines a multi-channel station - i.e. it is a collection of
///        single-channel and three-channel stations. 
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T>
class MultiChannelStation
{
    using WaveformType = std::vector<Waveform<T>>;
public:
    using iterator = typename WaveformType::iterator;
    using const_iterator = typename WaveformType::const_iterator;
public:
    /// @brief Defines the ordering of the channels.
    enum class Ordering
    {
        Default /*!< For example, a four channel-station {HHZ, HHN, HHE, EHZ}. */
    };
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    MultiChannelStation();
    /// @brief Copy constructor.
    /// @param[in] station  The multi-channel station from which to initialize
    ///                     this class.
    MultiChannelStation(const MultiChannelStation &station);
    /// @brief Move constructor.
    /// @param[in,out] station  The multi-channel station from which to
    ///                         initialize this class.  On exit, station's
    ///                         behavior is undefined.
    MultiChannelStation(MultiChannelStation &&station) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] station  The multi-channel station to copy to this.
    /// @result A deep copy of the input station.
    MultiChannelStation& operator=(const MultiChannelStation &station);
    /// @brief Move assignment.
    /// @param[in,out] station  The multi-channel station from whose memory
    ///                         will be moved to this.  On exit, station's
    ///                         behavior is undefined.
    /// @result The memory from station moved to this.
    MultiChannelStation& operator=(MultiChannelStation &&station) noexcept;
    /// @}

    /// @brief Adds channels to the collections.
    /// @param[in] channels  The channels to add to the collection.
    /// @note This will overwrite all existing channels.
    /// @throws std::invalid_argument if any of the channels differ in
    ///         network or station codes or a duplicate channel exists. 
    void setChannels(const std::vector<T> &channels);
    /// @brief Adds a channel to the collection.
    /// @param[in] overwrite  If the channel exists then it will be overwritten.
    void addChannel(const Waveform<T> &channel, const bool overwrite = false);
    /// @brief Clears the channels.
    void clearChannels() noexcept;

    /// @result The network code.
    [[nodiscard]] std::string getNetworkCode() const;
    /// @result True indicates the network code is known.
    [[nodiscard]] bool haveNetworkCode() const noexcept;

    /// @result The station code.
    [[nodiscard]] std::string getStationCode() const;
    /// @result True indicates the station code is known.
    [[nodiscard]] bool haveStationCode() const noexcept;

    /// @result The channel codes.
    //[[nodiscard]] std::vector<std::string> getChannelCodes() const;
    //[[nodiscard]] std::vector<std::string> getLocationCodes() const;
   

    /// @result The number of channels.
    [[nodiscard]] int getNumberOfChannels() const noexcept;

    /// @name Destructors
    /// @{

    /// @brief Resets the class and releases memory.
    void clear() noexcept;
    /// @brief Destructor.
    ~MultiChannelStation();
    /// @}
private:
    class MultiChannelStationImpl;
    std::unique_ptr<MultiChannelStationImpl> pImpl;
};
}
#endif
