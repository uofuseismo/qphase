#include <string>
#include <cmath>
#include <limits>
#include "qphase/database/internal/magnitude.hpp"

using namespace QPhase::Database::Internal;

class Magnitude::MagnitudeImpl
{
public:
    std::string mType;
    double mValue{0};
    int64_t mIdentifier{0};
    bool mHaveValue{false};
    bool mHaveIdentifier{false};
};

/// C'tor
Magnitude::Magnitude() :
    pImpl(std::make_unique<MagnitudeImpl> ())
{
}

/// Copy c'tor
Magnitude::Magnitude(const Magnitude &magnitude)
{
    *this = magnitude;
}

/// Move c'tor
Magnitude::Magnitude(Magnitude &&magnitude) noexcept
{
    *this = std::move(magnitude);
}

/// Copy assignment
Magnitude& Magnitude::operator=(const Magnitude &magnitude)
{
    if (&magnitude == this){return *this;}
    pImpl = std::make_unique<MagnitudeImpl> (*magnitude.pImpl);
    return *this;
}

/// Move assignment
Magnitude& Magnitude::operator=(Magnitude &&magnitude) noexcept
{
    if (&magnitude == this){return *this;}
    pImpl = std::move(magnitude.pImpl);
    return *this;
}

/// Reset class
void Magnitude::clear() noexcept
{
    pImpl = std::make_unique<MagnitudeImpl> ();
}

/// Destructor
Magnitude::~Magnitude() = default;

/// Magnitude
void Magnitude::setValue(const double value) noexcept
{
    pImpl->mValue = value;
    pImpl->mHaveValue = true;
}

double Magnitude::getValue() const
{
    if (!haveValue()){throw std::runtime_error("Value not set");}
    return pImpl->mValue;
}

bool Magnitude::haveValue() const noexcept
{
    return pImpl->mHaveValue;
}

/// Magnitude type
void Magnitude::setType(const std::string &type)
{
    if (type.empty()){throw std::invalid_argument("Type is empty");}
    pImpl->mType = type;
}

std::string Magnitude::getType() const
{
    if (!haveType()){throw std::runtime_error("Magnitude type not set");}
    return pImpl->mType;
}

bool Magnitude::haveType() const noexcept
{
    return !pImpl->mType.empty();
}

/// Magnitude ID
void Magnitude::setIdentifier(const int64_t identifier) noexcept
{
    pImpl->mIdentifier = identifier;
    pImpl->mHaveIdentifier = true;
}

int64_t Magnitude::getIdentifier() const
{
    if (!haveIdentifier()){throw std::runtime_error("Identifier not set");}
    return pImpl->mIdentifier;
}

bool Magnitude::haveIdentifier() const noexcept
{
    return pImpl->mHaveIdentifier;
}

///--------------------------------------------------------------------------///
///                              Equality Checks                             ///
///--------------------------------------------------------------------------///
/// Check if magnitudes are equal
bool QPhase::Database::Internal::operator==(const Magnitude &lhs,
                                            const Magnitude &rhs)
{
    if (lhs.haveIdentifier() && rhs.haveIdentifier())
    {
        if (lhs.getIdentifier() != rhs.getIdentifier()){return false;}
    }
    else
    {
        if (lhs.haveIdentifier() != rhs.haveIdentifier()){return false;}
    }

    if (lhs.haveType() && rhs.haveType())
    {
        if (lhs.getType() != rhs.getType()){return false;}
    }
    else
    {
        if (lhs.haveType() != rhs.haveType()){return false;}
    }

    if (lhs.haveValue() && rhs.haveValue())
    {
        if (std::abs(lhs.getValue() - rhs.getValue()) >
            std::numeric_limits<double>::epsilon()*100)
        {
            return false;
        }
    }
    else
    {
        if (lhs.haveValue() != rhs.haveValue()){return false;}
    }

    return true;
}

bool QPhase::Database::Internal::operator!=(const Magnitude &lhs,
                                            const Magnitude &rhs)
{
    return !(lhs == rhs);
}
