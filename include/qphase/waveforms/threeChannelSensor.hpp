#ifndef QPHASE_WAVEFORMS_THREECHANNELSENSOR_HPP
#define QPHASE_WAVEFORMS_THREECHANNELSENSOR_HPP
#include <memory>
namespace QPhase::Waveforms
{
template<class T> class Channel;
class SimpleResponse;
class InstrumentResponse;
};
namespace QPhase::Waveforms
{
/// @class ThreeChannelSensor "threeChannelSensor.hpp" "qphase/waveforms/threeChannelSensor.hpp"
/// @brief Defines a three-channel (vertical, north, east) sensor.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T>
class ThreeChannelSensor
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor
    ThreeChannelSensor();
    /// @brief Copy constructor.
    /// @param[in] sensor  The three-channel sensor from which to initialize
    ///                    this class.
    ThreeChannelSensor(const ThreeChannelSensor &sensor);
    /// @brief Move constructor.
    /// @param[in,out] sensor   The three-channel sensor from which to
    ///                         initialize this class.  On exit, sensor's
    ///                         behavior is undefined.
    ThreeChannelSensor(ThreeChannelSensor &&sensor) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] sensor  The three-channel sensor to copy to this.
    /// @result A deep copy of the input sensor.
    ThreeChannelSensor& operator=(const ThreeChannelSensor &sensor);
    /// @brief Move assignment.
    /// @param[in,out] sensor  The three-channel sensor whose memory will be
    ///                         moved to this.  On exit, sensor's behavior is
    ///                         undefined.
    /// @result The memory from sensor moved to this.
    ThreeChannelSensor& operator=(ThreeChannelSensor &&sensor) noexcept;
    /// @}

    /// @name Vertical Channel
    /// @{

    /// @brief Sets the vertical channel.
    /// @param[in] channel   The vertical channel.
    void setVerticalChannel(const Channel<T> &channel);
    /// @brief Sets a vertical channel.
    /// @param[in,out] channel  The vertical channel to set.
    ///                         On exit, channel's behavior is undefined.
    void setVerticalChannel(Channel<T> &&channel);
    /// @result A reference to the vertical channel.
    /// @throws std::runtime_error if \c haveVerticalChannel() is false.
    [[nodiscard]] const Channel<T>& getVerticalChannelReference() const;
    /// @result The vertical channel.
    /// @throws std::runtime_error if \c haveVerticalChannel() is false.
    [[nodiscard]] Channel<T> getVerticalChannel() const;
    /// @result True indicates the vertical channel was set.
    [[nodiscard]] bool haveVerticalChannel() const noexcept;

    /// @brief Sets the north channel.  For a borehole or USGS sensor,
    ///        this may be the ??1 channel code.
    /// @param[in] channel   The north channel.
    void setNorthChannel(const Channel<T> &channel);
    /// @brief Sets a north channel.  For a borehose or USGS sensor,
    ///        this may be the ??1 channel code.
    /// @param[in,out] channel  The north channel to set.
    ///                         On exit, channel's behavior is undefined.
    void setNorthChannel(Channel<T> &&channel);
    /// @result A reference to the north channel.
    /// @throws std::runtime_error if \c haveNorthChannel() is false.
    [[nodiscard]] const Channel<T>& getNorthChannelReference() const;
    /// @result The north channel.
    /// @throws std::runtime_error if \c haveNorthChannel() is false.
    [[nodiscard]] Channel<T> getNorthChannel() const;
    /// @result True indicates the north channel was set.
    [[nodiscard]] bool haveNorthChannel() const noexcept;

    /// @brief Sets the east channel.  For a borehole or USGS sensor,
    ///        this may be the ??1 channel code.
    /// @param[in] channel   The east channel.
    void setEastChannel(const Channel<T> &channel);
    /// @brief Sets a east channel.  For a borehose or USGS sensor,
    ///        this may be the ??1 channel code.
    /// @param[in,out] channel  The east channel to set.
    ///                         On exit, channel's behavior is undefined.
    void setEastChannel(Channel<T> &&channel);
    /// @result A reference to the east channel.
    /// @throws std::runtime_error if \c haveEastChannel() is false.
    [[nodiscard]] const Channel<T>& getEastChannelReference() const;
    /// @result The east channel.
    /// @throws std::runtime_error if \c haveEastChannel() is false.
    [[nodiscard]] Channel<T> getEastChannel() const;
    /// @result True indicates the east channel was set.
    [[nodiscard]] bool haveEastChannel() const noexcept;
    /// @}

    /// @name Location
    /// @{

    /// @brief Sets the sensors's latitude in degrees.
    /// @param[in] latitude  The latitude in degrees which is measured positive
    ///                      north of the equator.
    /// @throws std::invalid_argument if the latitude is not in
    ///         the range [-90,90].
    void setLatitude(double latitude);
    /// @result The sensors's latitude in degrees.
    /// @throws std::runtime_error if \c haveLatitude() is false. 
    [[nodiscard]] double getLatitude() const;
    /// @result True indicates the sensor's latitude is set.
    [[nodiscard]] bool haveLatitude() const noexcept;

    /// @brief Sets the sensors's longitude in degrees.
    /// @param[in] longitude  The sensors's longitude.
    void setLongitude(double longitude) noexcept;
    /// @result The sensors's longitude in degrees.  This will be in the range
    ///         [-180, 180).
    /// @throws std::runtime_error if \c haveLongitude() is false.
    [[nodiscard]] double getLongitude() const;
    /// @result True indicates the longitude was set.
    [[nodiscard]] bool haveLongitude() const noexcept;

    /// @brief Sets the sensor's elevation.
    /// @param[in] elevation  The sensor's elevation in meters w.r.t sea-level.
    void setElevation(double elevation);
    /// @result The sensors's elevation in meters.
    /// @throws std::runtime_error if \c haveElevation() is false.
    [[nodiscard]] double getElevation() const;
    /// @result True indicates the sensor's elevation was set.
    [[nodiscard]] bool haveElevation() const noexcept;
    /// @}

    /// @name Location Code
    /// @{

    /// @brief Sets the location code.
    /// @param[in] locationCode  The location code - e.g., 01.
    void setLocationCode(const std::string &locationCode);
    /// @result The location code.  By default this is empty.
    [[nodiscard]] std::string getLocationCode() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets class and releases memory.
    void clear() noexcept;
    /// @brief Destructor.
    ~ThreeChannelSensor();
    /// @}
private:
    class ThreeChannelSensorImpl;
    std::unique_ptr<ThreeChannelSensorImpl> pImpl;
};
}
#endif
