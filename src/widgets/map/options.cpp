#include <iostream>
#include <string>
#include <filesystem>
#include <QGeoView/QGVGlobal.h>
#include "qphase/widgets/map/options.hpp"
#include "private/paths.hpp"
#include "private/shiftLongitude.hpp"

using namespace QPhase::Widgets::Map;

namespace
{

void checkLatitude(const double latitude)
{
    if (latitude < -90 || latitude > 90)
    {
        throw std::invalid_argument("Latitude = " + std::to_string(latitude)
                                  + " must be in range [-90,90]");
    }
}

}

class Options::OptionsImpl
{
public:
    OptionsImpl()
    {
        // Map center
        setCenter(std::pair{40.89, -111}); // LHUT looks good for UUSS
        // Utah
        std::vector<std::pair<double, double>> utah(5);
        utah[0] = std::pair(36.75, -114.25);
        utah[1] = std::pair(36.75, -108.75);
        utah[2] = std::pair(42.5,  -108.75);
        utah[3] = std::pair(42.5,  -114.25);
        utah[4] = std::pair(36.75, -114.25);
        addBoundary(utah);
        // Yellowstone National Park
        std::vector<std::pair<double, double>> ynp(5);
        ynp[0] = std::pair(44.,    -111.333);
        ynp[1] = std::pair(44.,    -109.75);
        ynp[2] = std::pair(45.167, -109.75);
        ynp[3] = std::pair(45.167, -111.333);
        ynp[4] = std::pair(44.,    -111.333);
        addBoundary(ynp);
        // FORGE
        std::vector<std::pair<double, double>> forge(9);
        forge[0] = std::pair(38.50766251464152, -112.9172670480094);
        forge[1] = std::pair(38.50765095807106, -112.8984475721061);
        forge[2] = std::pair(38.48464129404871, -112.8986038105227);
        forge[3] = std::pair(38.48473681463327, -112.889197233939);
        forge[4] = std::pair(38.50007663264921, -112.8891782562262);
        forge[5] = std::pair(38.50006934569299, -112.8800311838714);
        forge[6] = std::pair(38.51489823672957, -112.8801413665903);
        forge[7] = std::pair(38.51492363342575, -112.9169998098031);
        forge[8] = std::pair(38.50766251464152, -112.9172670480094);
        addBoundary(forge); 
        // Create tile cache directory
        std::string directory = mTileCacheDirectory;
        setTileCacheDirectory(directory);
    }
    /// Sets the map center
    void setCenter(const std::pair<double, double> &center)
    {
        auto lat = center.first;
        checkLatitude(lat);
        auto lon = shiftLongitude(center.second);
        mCenter.first = lat;
        mCenter.second = lon;
    }
    /// Adds a boundary
    void addBoundary(const std::vector<std::pair<double, double>> &boundary)
    {
        if (boundary.size() < 2)
        {
            throw std::invalid_argument("Boundary must have 2 points");
        }
        std::vector<std::pair<double, double>> newBoundary;
        newBoundary.reserve(boundary.size());
        for (const auto &p : boundary)
        {
            auto lat = p.first;
            checkLatitude(lat);
            auto lon = shiftLongitude(p.second);
            newBoundary.push_back(std::pair(lat, lon));
        }
        mBoundaries.push_back(newBoundary);
    }
    /// Sets the tile cache directory
    void setTileCacheDirectory(const std::string &directory)
    {
        std::filesystem::path cacheDirectory(directory);;
        if (!std::filesystem::exists(cacheDirectory))
        {
            if (!std::filesystem::create_directories(cacheDirectory))
            {
                throw std::runtime_error("Failed to make tile cache: "
                                       + directory);
            }
        }
        mTileCacheDirectory = directory;
    }
///private:
    std::pair<double, double> mCenter;
    std::vector<std::vector<std::pair<double, double>>> mBoundaries;
    std::filesystem::path mTileCacheDirectory
        = std::filesystem::path{CACHE_PATH} /
          std::filesystem::path{"mapTiles/"};
    double mScale = 1/3000.; // Inverse of kilometers
};

/// C'tor
Options::Options() :
    pImpl(std::make_unique<OptionsImpl> ())
{
}

/// Copy c'tor
Options::Options(const Options &options)
{
    *this = options;
}

/// Move c'tor
Options::Options(Options &&options) noexcept
{
    *this = std::move(options);
}
            
/// Copy assignment
Options& Options::operator=(const Options &options)
{
    if (&options == this){return *this;}
    pImpl = std::make_unique<OptionsImpl> (*options.pImpl);
    return *this;
}

/// Move assignment
Options& Options::operator=(Options &&options) noexcept
{
    if (&options == this){return *this;}
    pImpl = std::move(options.pImpl);
    return *this;
}

/// Reset class
void Options::clear() noexcept
{
    pImpl = std::make_unique<OptionsImpl> ();
}

/// Destructor
Options::~Options() = default;

/// Map center
void Options::setCenter(const std::pair<double, double> &center)
{
    pImpl->setCenter(center);
}

std::pair<double, double> Options::getCenter() const noexcept
{
    return pImpl->mCenter;
}

/// Map boundaries
void Options::addBoundary(
    const std::vector<std::pair<double, double>> &boundary)
{
    pImpl->addBoundary(boundary);
}

std::vector<std::vector<std::pair<double, double>>>
    Options::getBoundaries() const noexcept
{
    return pImpl->mBoundaries;
}

/// Map scale
void Options::setScale(const double scale)
{
    if (scale <= 0){throw std::invalid_argument("Scale must be positive");}
    pImpl->mScale = scale;
}

double Options::getScale() const noexcept
{
    return pImpl->mScale;
}

/// Map cache directory
void Options::setTileCacheDirectory(const std::string &directory)
{
    pImpl->setTileCacheDirectory(directory);
}

std::string Options::getTileCacheDirectory() const noexcept
{
    return pImpl->mTileCacheDirectory;
}
