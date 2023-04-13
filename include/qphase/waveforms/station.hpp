#ifndef QPHASE_WAVEFORMS_STATION_HPP
#define QPHASE_WAVEFORMS_STATION_HPP
#include <memory>
namespace QPhase::Waveforms
{
template<class T> class SingleChannelVerticalSensor;
template<class T> class SingleChannelSensor;
template<class T> class ThreeChannelSensor;
template<class T> class Channel;
}
namespace QPhase::Waveforms
{
/// @class Station "station.hpp" "qphase/waveforms/station.hpp"
/// @brief A seismic station is a collection of single-channel and three-channel
///        sensors.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T>
class Station
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor. 
    Station();
    /// @brief Copy constructor.
    /// @param[in] station  The station from which to initialize this class.
    Station(const Station &station);
    /// @brief Move constructor.
    /// @param[in,out] station  The station from which to initialize this class.
    ///                         On exit, station's behavior is undefined.
    Station(Station &&station) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] station  The station to copy to this.
    /// @result A deep copy of the input station.
    Station& operator=(const Station &station);
    /// @brief Move assignment operator.
    /// @param[in,out] station  The station whose memory will be moved to this.
    ///                         On exit, station's behavior is undefined.
    /// @result The memory from station moved to this.
    Station& operator=(Station &&station) noexcept;
    /// @}

    /// @name Sensors
    /// @{

    /// @brief Adds a single-channel sensor.
    void add(const SingleChannelSensor<T> &sensor);
    void add(SingleChannelSensor<T> &&sensor);
    /// @brief Adds a single-channel vertical sensor.
    void add(const SingleChannelVerticalSensor<T> &sensor);
    void add(SingleChannelVerticalSensor<T> &&sensor);
    /// @brief Adds a three-channel sensor.
    void add(const ThreeChannelSensor<T> &sensor); 
    void add(ThreeChannelSensor<T> &&sensor);

    [[nodiscard]] const std::vector<ThreeChannelSensor<T>>& getThreeChannelSensorsReference() const noexcept; 
    [[nodiscard]] const std::vector<SingleChannelSensor<T>>& getSingleChannelSensorsReference() const noexcept;

    /// @brief Gets the three-channel sensors.

    /// @param[in] channel       The channel code.
    /// @param[in] locationCode  The location code.
    /// @throws std::invalid_argument if \c channelExists() is false.
    [[nodiscard]] Channel<T> getChannelReference(const std::string &channel,
                                                 const std::string &locationCode);
    /// @param[in] channel       The channel code.
    /// @param[in] locationCode  The location code.
    /// @result True indicates the channel exists.
    [[nodiscard]] bool channelExists(const std::string &channel,
                                     const std::string &locationCode) const;

    /// @result The cumulative number of channels.
    [[nodiscard]] int getNumberOfChannels() const noexcept;
    /// @result The cumulative number of vertical channels.
    [[nodiscard]] int getNumberOfVerticalChannels() const noexcept;
    /// @}

    /// @name Station Name
    /// @{

    /// @brief Sets the station's network code.
    /// @param[in] networkCode The station network code - e.g., UU.
    void setNetworkCode(const std::string &networkCode);
    /// @result The network code.
    /// @throws std::runtime_error if \c haveNetworkCode() is false.
    [[nodiscard]] std::string getNetworkCode() const;
    /// @result True indicates the network code was set.
    [[nodiscard]] bool haveNetworkCode() const noexcept;

    /// @brief Sets the station name.
    /// @param[in] name  The station name - e.g., BRTU.
    void setName(const std::string &name);
    /// @result The station name.
    /// @throws std::runtime_error if \c haveName() is false.
    [[nodiscard]] std::string getName() const;
    /// @result True indicates the station name was set.
    [[nodiscard]] bool haveName() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Releases all memory and resets the class.
    void clear() noexcept; 
    /// @brief Destructor.
    ~Station();
    /// @}
private:
    class StationImpl;
    std::unique_ptr<StationImpl> pImpl;
};
}
#endif
