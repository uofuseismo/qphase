#ifndef QPHASE_DATABASE_INTERNAL_MAGNITUDE_HPP
#define QPHASE_DATABASE_INTERNAL_MAGNITUDE_HPP
#include <memory>
namespace QPhase::Database::Internal
{
/// @name Magnitude "magnitude.hpp" "qphase/database/internal/magnitude.hpp"
/// @brief Defines a magnitude.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class Magnitude
{
public:
    /// @name Constructors
    /// @{
 
    /// @brief Constructor.
    Magnitude();
    /// @brief Copy constructor.
    /// @param[in] magnitude  The magnitude class to copy to this.
    Magnitude(const Magnitude &magnitude);
    /// @brief Move constructor.
    /// @param[in,out] magnitude  The magnitude class from which to initialize
    ///                           this class.  On exit, magnitude's behavior is
    ///                           undefined.
    Magnitude(Magnitude &&magnitude) noexcept;
    /// @}

    /// @name Operators
    /// @{
 
    /// @brief Copy assignment operator.
    /// @param[in] magnitude  The magnitude class to copy to this.
    /// @result A deep copy of the input magnitude.
    Magnitude& operator=(const Magnitude &magnitude);
    /// @brief Move assignment operator.
    /// @param[in,out] magnitude  The magnitude class to move to this.  On exit,
    ///                           magnitude's behavior is undefined.
    /// @result The memory from magnitude moved to this.
    Magnitude& operator=(Magnitude &&magnitude) noexcept;
    /// @}

    /// @name Parameters
    /// @{

    /// @brief Sets the magnitude type.
    /// @param[in] type  The magnitude type - e.g., 'l' or 'Ml' for Richter
    ///                  magnitude.
    /// @throws std::invalid_argument if the type is empty.
    void setType(const std::string &type);
    /// @result The magnitude type.
    /// @throws std::runtime_error if \c haveType() is false.
    [[nodiscard]] std::string getType() const;
    /// @result True indicates the magnitude type was set.
    [[nodiscard]] bool haveType() const noexcept;

    /// @brief Sets the magnitude value.
    /// @param[in] value  The magnitude value - e.g., 5.6.
    void setValue(double value) noexcept;
    /// @result The magnitude.
    /// @throws std::runtime_error if \c haveValue() is false.
    [[nodiscard]] double getValue() const;
    /// @result True indicates the value was set.
    [[nodiscard]] bool haveValue() const noexcept;

    /// @brief Sets the magnitude identifier.
    /// @param[in] identifier  The magnitude identifier.
    void setIdentifier(int64_t identifier) noexcept;
    /// @result The magnitude identifier.
    /// @throws std::runtime_error if \c haveIdentifier() is false.
    [[nodiscard]] int64_t getIdentifier() const;
    /// @result True indicates the identifier was set.
    [[nodiscard]] bool haveIdentifier() const noexcept;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor.
    ~Magnitude();
    /// @}
private:
    class MagnitudeImpl;
    std::unique_ptr<MagnitudeImpl> pImpl;
};
[[nodiscard]] bool operator==(const Magnitude &lhs, const Magnitude &rhs);
[[nodiscard]] bool operator!=(const Magnitude &lhs, const Magnitude &rhs);
}
#endif
