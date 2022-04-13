#ifndef QPHASE_WEBSERVICES_COMCAT_EVENT_HPP
#define QPHASE_WEBSERVICES_COMCAT_EVENT_HPP
#include <memory>
namespace QPhase::WebServices::Comcat
{
/// @name Event "event.hpp" "qphase/webServices/comcat/event.hpp"
/// @brief Defines an event returned from a ComCat geoJSON query.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Event
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Event();
    /// @brief Copy constructor.
    /// @param[in] event  The event class from which to initialize this class.
    Event(const Event &event);
    /// @brief Move constructor.
    /// @param[in,out] event  The event class from which to initialize this
    ///                       class.  On exit, event's behavior is undefined.
    Event(Event &&event) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment.
    /// @param[in] event  The event to copy to this.
    /// @result A deep copy of event.
    Event& operator=(const Event &event);
    /// @brief Move assignment operator.
    /// @param[in,out] event  The event whose memory will be moved to this.
    ///                       On exit, event's behavior is undefined.
    /// @result The memory from event moved to this.
    Event& operator=(Event &&event) noexcept;
    /// @}

    /// @name Properties
    /// @{

    /// @brief Sets the event's latitude.
    /// @param[in] latitude  The latitude of the event in degrees.
    /// @throws std::invalid_arument if the latitude is not in the
    ///         range [-90,90].
    void setLatitude(double latitude);
    /// @result The event's latitude in degrees.
    /// @throws std::runtime_error if \c haveLatitude() is false.
    [[nodiscard]] double getLatitude() const;
    /// @result True indicates the latitude was set.
    [[nodiscard]] bool haveLatitude() const noexcept;

    /// @brief Sets the event's longitude.
    /// @param[in] longitude  The longitude of the event in degrees.
    /// @note The longitude will be shifted to the range [-180,180).
    void setLongitude(double longitude);
    /// @result The event's longitude in degrees.
    /// @throws std::runtime_error if \c haveLongitude() is false.
    [[nodiscard]] double getLongitude() const;
    /// @result True indicates that the longitude was set.
    [[nodiscard]] bool haveLongitude() const noexcept;

    /// @brief Sets the event's depth.
    /// @param[in] depth  The depth of the event in km.
    void setDepth(double depth) noexcept;
    /// @result The depth of the event in km.
    /// @throws std::runtime_error if \c haveDepth() is false.
    [[nodiscard]] double getDepth() const;
    /// @result True indicates that the depth was set.
    [[nodiscard]] bool haveDepth() const noexcept;

    /// @brief Sets the origin time.
    /// @param[in] originTime  The origin time (UTC) in seconds from the epoch.
    void setOriginTime(double originTime) noexcept;
    /// @result The origin time (UTC) in seconds from the epoch.
    /// @throws std::runtime_error if \c haveOriginTime() is false.
    [[nodiscard]] double getOriginTime() const;
    /// @result True indicates that origin time was set.
    [[nodiscard]] bool haveOriginTime() const noexcept;

    /// @brief Sets the event's magnitude.
    /// @param[in] magnitude  The event's magnitude.
    /// @throws std::invalid_argument if this is not in the range [-10,10].
    void setMagnitude(double magnitude);
    /// @result The magnitude of the event.
    /// @throws std::invalid_argument if \c haveMagnitude() is false. 
    [[nodiscard]] double getMagnitude() const;
    /// @result True indicates that the magnitude was set.
    [[nodiscard]] bool haveMagnitude() const noexcept;

    /// @brief Sets the event's magnitude type - e.g., Ml, Md, Mw, etc.
    /// @param[in] magnitudeType  The event magnitude type.
    /// @throws std::invalid_argument if the magnitude type is empty.
    void setMagnitudeType(const std::string &magnitudeType);
    /// @result The magnitude type.
    /// @throws std::runtime_error if \c haveMagnitudeType() is false.
    [[nodiscard]] std::string getMagnitudeType() const;
    /// @result True indicates that the magnitud type was set.
    [[nodiscard]] bool haveMagnitudeType() const noexcept;

    /// @brief Sets the event identifier.
    /// @param[in] id  The event identifer.
    void setIdentifier(uint64_t id) noexcept;
    /// @result The event identifier.
    /// @throws std::runtime_error if \c haveIdentifier() is false.
    [[nodiscard]] uint64_t getIdentifier() const;
    /// @result True indicates that the event identifier was set.
    [[nodiscard]] bool haveIdentifier() const noexcept;

    /// @brief Sets the USGS URL for the event.
    /// @param[in] url  The URL for the event.
    /// @throws std::invalid_argument if the url is empty.
    void setURL(const std::string &url);
    /// @result The URL fo the event.
    /// @throws std::runtime_error if \c haveURL() is false.
    [[nodiscard]] std::string getURL() const;
    /// @result True indicates that the URL was set.
    [[nodiscard]] bool haveURL() const noexcept;

    /// @brief Sets the authoritative network - e.g., UU for University of Utah
    ///        Seismograph Stations.
    /// @param[in] network  The authoriative network.
    /// @note This will be capitalized internally.
    /// @throws std::invalid_argument if the network is empty.
    void setNetwork(const std::string &network);
    /// @result The authoriative network.
    /// @throws std::runtime_error if \c haveNetwork() is false.
    [[nodiscard]] std::string getNetwork() const;
    /// @result True indicates the network was set.
    [[nodiscard]] bool haveNetwork() const noexcept;

    /// @brief Indicates if the event has been reviewed or if it is an
    ///        automatic solution.
    /// @param[in] reviewStatus  The review status of the event.
    /// @throws std::invalid_argument if reviewStatus is empty.
    void setReviewStatus(const std::string &reviewStatus);
    /// @result The review status.
    /// @throws std::runtime_error if \c haveReviewStatus() is false.
    [[nodiscard]] std::string getReviewStatus() const;
    /// @result True indicates that the review status was set.
    [[nodiscard]] bool haveReviewStatus() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Releases memory and resets the class.
    void clear() noexcept;
    /// @brief Destructor
    ~Event();
    /// @}
private:
    class EventImpl;
    std::unique_ptr<EventImpl> pImpl;
};
}
#endif
