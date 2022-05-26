#include <cmath>
#include "qphase/waveforms/simpleResponse.hpp"

using namespace QPhase::Waveforms;

class SimpleResponse::SimpleResponseImpl
{
public:
    std::chrono::microseconds mStartTime{-2208988800*1000000};   // Year 1900 
    std::chrono::microseconds mEndTime{ 4102444800*1000000}; // Year 2100
    double mScalar = 0;
    Units mInputUnits;
    Units mOutputUnits;
    bool mHaveInputUnits = false;
    bool mHaveOutputUnits = false;
    bool mHaveStartAndEndTime = false;
};

/// C'tor
SimpleResponse::SimpleResponse() :
    pImpl(std::make_unique<SimpleResponseImpl> ())
{
}

/// Copy c'tor
SimpleResponse::SimpleResponse(const SimpleResponse &response)
{
    *this = response;
}

/// Move c'tor
SimpleResponse::SimpleResponse(SimpleResponse &&response) noexcept
{
    *this = std::move(response);
}

/// Copy assignment 
SimpleResponse& SimpleResponse::operator=(const SimpleResponse &response)
{
    if (&response == this){return *this;}
    pImpl = std::make_unique<SimpleResponseImpl> (*response.pImpl);
    return *this;
}

/// Move assignment
SimpleResponse& SimpleResponse::operator=(SimpleResponse &&response) noexcept
{
    if (&response == this){return *this;}
    pImpl = std::move(response.pImpl);
    return *this;
}

/// Reset class
void SimpleResponse::clear() noexcept
{
    pImpl = std::make_unique<SimpleResponseImpl> ();
}

/// Destructor
SimpleResponse::~SimpleResponse() = default;

/// Scalar
void SimpleResponse::setScalar(const double scalar)
{
    if (scalar == 0){throw std::invalid_argument("Scalar cannot be zero");}
    pImpl->mScalar = scalar;
}

double SimpleResponse::getScalar() const
{
    if (!haveScalar()){throw std::runtime_error("Scalar not set");}
    return pImpl->mScalar;
}

bool SimpleResponse::haveScalar() const noexcept
{
    return (pImpl->mScalar != 0);
}

/// Input units
void SimpleResponse::setInputUnits(const Units units) noexcept
{
    pImpl->mInputUnits = units;
    pImpl->mHaveInputUnits = true;
}

Units SimpleResponse::getInputUnits() const
{
    if (!haveInputUnits()){throw std::runtime_error("Input units not set");}
    return pImpl->mInputUnits;
}

bool SimpleResponse::haveInputUnits() const noexcept
{
    return pImpl->mHaveInputUnits;
}

/// Output units
void SimpleResponse::setOutputUnits(const Units units) noexcept
{
    pImpl->mOutputUnits = units;
    pImpl->mHaveOutputUnits = true;
}

Units SimpleResponse::getOutputUnits() const
{
    if (!haveOutputUnits()){throw std::runtime_error("Output units not set");}
    return pImpl->mOutputUnits;
}

bool SimpleResponse::haveOutputUnits() const noexcept
{
    return pImpl->mHaveOutputUnits;
}

/// Start / end time
void SimpleResponse::setStartAndEndTime(
    const std::pair<std::chrono::microseconds,
                    std::chrono::microseconds> &startEndTime)
{
    if (startEndTime.first >= startEndTime.second)
    {
        throw std::invalid_argument("Start time must be less than end time");
    }
    pImpl->mStartTime = startEndTime.first;
    pImpl->mEndTime = startEndTime.second;
    pImpl->mHaveStartAndEndTime = true; 
}

std::chrono::microseconds SimpleResponse::getStartTime() const
{
    if (!haveStartAndEndTime()){throw std::runtime_error("Start time not set");}
    return pImpl->mStartTime;
}

std::chrono::microseconds SimpleResponse::getEndTime() const
{
    if (!haveStartAndEndTime()){throw std::runtime_error("End time not set");}
    return pImpl->mEndTime;
}

bool SimpleResponse::haveStartAndEndTime() const noexcept
{
    return pImpl->mHaveStartAndEndTime;
}
