#include <string>
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/magnitude.hpp"

using namespace QPhase::Database::Internal;

class Event::EventImpl
{
public:
    Origin mOrigin;
    Magnitude mMagnitude;
    int64_t mIdentifier;
    Event::ReviewStatus mReviewStatus = Event::ReviewStatus::Automatic;
    Event::Type mType = Event::Type::Unknown;
    bool mHaveIdentifier{false};
    bool mHaveOrigin{false};
    bool mHaveMagnitude{false};
};

/// C'tor
Event::Event() :
    pImpl(std::make_unique<EventImpl> ())
{
}

/// Copy c'tor
Event::Event(const Event &event)
{
    *this = event;
}

/// Move c'tor
Event::Event(Event &&event) noexcept
{
    *this = std::move(event);
}

/// Copy assignment
Event& Event::operator=(const Event &event)
{
    if (&event == this){return *this;}
    pImpl = std::make_unique<EventImpl> (*event.pImpl);
    return *this;
}

/// Move assignment
Event& Event::operator=(Event &&event) noexcept
{
    if (&event == this){return *this;}
    pImpl = std::move(event.pImpl);
    return *this;
}

/// Reset class
void Event::clear() noexcept
{
    pImpl = std::make_unique<EventImpl> ();
}

/// Destructor
Event::~Event() = default;

/// Event type
void Event::setType(const Event::Type type) noexcept
{
    pImpl->mType = type;
}

Event::Type Event::getType() const noexcept
{
    return pImpl->mType;
}

/// Identifier
void Event::setIdentifier(const int64_t id) noexcept
{
    pImpl->mIdentifier = id;
    pImpl->mHaveIdentifier = true;
}

int64_t Event::getIdentifier() const
{
    if (!haveIdentifier()){return false;}
    return pImpl->mIdentifier;
}

bool Event::haveIdentifier() const noexcept
{
    return pImpl->mHaveIdentifier;
}

/// Sets the magnitude
void Event::setMagnitude(const Magnitude &magnitude)
{
    if (!magnitude.haveType()){throw std::invalid_argument("Type not set");}
    if (!magnitude.haveValue()){throw std::invalid_argument("Value not set");}
    pImpl->mMagnitude = magnitude;
    pImpl->mHaveMagnitude = true;
}

Magnitude Event::getMagnitude() const
{
    if (!haveMagnitude()){throw std::runtime_error("Magnitude not set");}
    return pImpl->mMagnitude;
}

bool Event::haveMagnitude() const noexcept
{
    return pImpl->mHaveMagnitude;
}

/// Sets the origin
void Event::setOrigin(const Origin &origin)
{
    if (!origin.haveTime()){throw std::invalid_argument("Time not set");}
    if (!origin.haveDepth()){throw std::invalid_argument("Depth not set");}
    if (!origin.haveLatitude())
    {
        throw std::invalid_argument("Latitude not set");
    }
    if (!origin.haveLongitude())
    {
        throw std::invalid_argument("Longitude not set");
    }
    pImpl->mOrigin = origin;
    pImpl->mHaveOrigin = true;
}

Origin Event::getOrigin() const
{
    if (!haveOrigin()){throw std::runtime_error("Origin not set");}
    return pImpl->mOrigin;
}

bool Event::haveOrigin() const noexcept
{
    return pImpl->mHaveOrigin;
}

/// Review status
void Event::setReviewStatus(const Event::ReviewStatus status) noexcept
{
    pImpl->mReviewStatus = status;
}

Event::ReviewStatus Event::getReviewStatus() const noexcept
{
    return pImpl->mReviewStatus;
}

///--------------------------------------------------------------------------///
///                                Operators                                 ///
///--------------------------------------------------------------------------///
bool QPhase::Database::Internal::operator==(const Event &lhs, const Event &rhs)
{
    if (lhs.getReviewStatus() != rhs.getReviewStatus()){return false;}
    if (lhs.getType() != rhs.getType()){return false;}

    if (lhs.haveOrigin() != rhs.haveOrigin()){return false;}
    if (lhs.haveOrigin())
    {
        if (lhs.getOrigin() != rhs.getOrigin()){return false;}
    }

    if (lhs.haveMagnitude() != rhs.haveMagnitude()){return false;}
    if (lhs.haveMagnitude())
    {
        if (lhs.getMagnitude() != rhs.getMagnitude()){return false;}
    }
    return true;
}

bool QPhase::Database::Internal::operator!=(const Event &lhs, const Event &rhs)
{
    return !(lhs == rhs);
}
