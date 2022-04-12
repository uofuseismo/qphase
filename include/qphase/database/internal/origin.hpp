#ifndef QPHASE_DATABASE_INTERNAL_ORIGIN_HPP
#define QPHASE_DATABASE_INTERNAL_ORIGIN_HPP
#include <memory>
#include <chrono>
namespace QPhase::Database::Internal
{
/// @name Origin "origin.hpp" "qphase/database/internal/origin.hpp"
/// @brief Defines an origin.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Origin
{
public:
    /// @brief Defines the origin's review status.
    enum class ReviewStatus : int8_t
    {   
        AUTOMATIC = 0,  /*!< This is an automatically created origin and yes to
                             be reviewed. */
        INCOMPLETE = 1, /*!< This event was manually reviewed but has to be
                             finalized. */
        FINALIZED = 2,  /*!< This is a reviewed origin. */
        CANCELLED = 3   /*!< This origin has been cancelled. */
    };  
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    Origin();
    /// @brief Copy constructor.
    /// @param[in] origin  The origin class from which to initialize this class.
    Origin(const Origin &origin);
    /// @brief Move constructor.
    /// @param[in,out] origin  The origin class from which to initialize this
    ///                        class.  On exit, origin's behavior is undefined.
    Origin(Origin &&origin) noexcept;
    /// @}

    /// @name Operators
    /// @{

    /// @brief Copy assignment operator.
    /// @param[in] origin  The origin to copy to this.
    /// @result A deep copy of the input origin.
    Origin& operator=(const Origin &origin);
    /// @brief Move assignment operator.
    /// @param[in,out] origin  The origin whose memory will be moved to this.
    ///                        On exit, origin's behavior is undefined.
    /// @result The memory from origin moved to this.
    Origin& operator=(Origin &&origin) noexcept;
    /// @} 

    /// @name Required Parameters
    /// @{

    /// @brief Sets the event latitude.
    /// @param[in] latitude  The event latitude in degrees. 
    /// @throws std::invalid_argument if the latitude is not in the
    ///         range [-90,90].
    void setLatitude(double latitude);
    /// @result The event latitude in degrees.
    /// @throws std::runtime_error if \c haveLatitude() is false.
    [[nodiscard]] double getLatitude() const;
    /// @result True indicates that latitude was set.
    [[nodiscard]] bool haveLatitude() const noexcept;

    /// @brief Sets the event longitude.
    /// @param[in] longitude  The event longitude in degrees.  This is
    ///                       positive east.
    void setLongitude(double longitude) noexcept;
    /// @result The event longitude in degrees.  This will be in the range
    ///         [-180,180).
    /// @throws std::runtime_error if \c haveLongitude() is false.
    [[nodiscard]] double getLongitude() const;
    /// @result True indicates that longitude was set.
    [[nodiscard]] bool haveLongitude() const noexcept;

    /// @brief Sets the event depth in kilometers.
    void setDepth(double depth) noexcept;
    /// @result The event depth in kilometers.
    [[nodiscard]] double getDepth() const;
    /// @result True indicates the depth was set.
    [[nodiscard]] bool haveDepth() const noexcept;

    /// @brief Sets the event origin time.
    /// @param[in] time  The event origin time in seconds (UTC) from the epoch.
    void setTime(double time) noexcept;
    /// @brief Sets the event origin time.
    /// @param[in] time  The event origin time in microseconds (UTC) from
    ///                  the epoch (Jan 1, 1970).
    void setTime(const std::chrono::microseconds &time) noexcept;
    /// @result The event origin time.
    /// @throws std::runtime_error if \c haveTime() is false.
    [[nodiscard]] std::chrono::microseconds getTime() const;
    /// @result True indicates the event time was set.
    [[nodiscard]] bool haveTime() const noexcept;

    /// @brief Origin identifier.
    void setIdentifier(int64_t identifier) noexcept;
    /// @result The origin identifier.
    /// @throws std::runtime_error if \c haveIdentifier() is false.
    [[nodiscard]] int64_t getIdentifier() const;
    /// @result True indicates the origin identifier was set.
    [[nodiscard]] bool haveIdentifier() const noexcept;
    /// @}

    /// @name Optional Parameters
    /// @{

    /// @brief Sets the origin's review status.
    /// @param[in] status  The review status.
    void setReviewStatus(Origin::ReviewStatus status) noexcept;
    /// @result The review status.
    [[nodiscard]] Origin::ReviewStatus getReviewStatus() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor
    ~Origin();
    /// @}
private:
    class OriginImpl;
    std::unique_ptr<OriginImpl> pImpl;
};
[[nodiscard]] bool operator==(const Origin &lhs, const Origin &rhs);
[[nodiscard]] bool operator!=(const Origin &lhs, const Origin &rhs);
}
#endif
