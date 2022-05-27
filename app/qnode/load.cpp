#include <iostream>
#include <QString>
#include <string>
#include <set>
#include <filesystem>
#include <sff/sac/waveform.hpp>
#include <sff/utilities/time.hpp>
#include <QDebug>
#include <boost/algorithm/string.hpp>
#include "load.hpp"
#include "qphase/waveforms/station.hpp"
#include "qphase/waveforms/threeChannelSensor.hpp"
#include "qphase/waveforms/singleChannelSensor.hpp"
#include "qphase/waveforms/waveform.hpp"
#include "qphase/waveforms/channel.hpp"
#include "qphase/waveforms/segment.hpp"
#include "private/removeBlanksAndCapitalize.hpp"

namespace
{
template<class T>
struct WaveformHelper
{
    QPhase::Waveforms::Waveform<T> waveform; 
    std::string network;
    std::string station;
    std::string channel;
    std::string locationCode;
    double azimuth = 0;
    double inclination = 0;
    double latitude = 0;
    double longitude = 0;
    bool haveAzimuth = false;
    bool haveInclination = false;
    bool haveLatitude = false;
    bool haveLongitude = false;
};

template<typename T>
QPhase::Waveforms::Channel<T>
     waveformHelperToChannel(WaveformHelper<T> &&helper)
{
    QPhase::Waveforms::Channel<T> channel;
    channel.setWaveform(std::move(helper.waveform));
    channel.setChannelCode(helper.channel);
    if (helper.haveAzimuth){channel.setAzimuth(helper.azimuth);}
    if (helper.haveInclination){channel.setDip(helper.inclination);}
    return channel; 
}

}


template<typename T>
std::vector<QPhase::Waveforms::Station<T>>
    QPhase::QNode::loadSACFiles(const std::vector<std::string> &fileNames,
                                const std::chrono::microseconds &t0,
                                const std::chrono::microseconds &t1)
{
    int nRead = 0;
    for (const auto &fileName : fileNames) 
    {
        if (std::filesystem::exists(fileName)){nRead = nRead + 1;}
    }
    std::vector<QPhase::Waveforms::Station<T>> result;
    if (nRead < 1){return result;}
    std::vector<WaveformHelper<T>> workSpace;
    workSpace.reserve(nRead);
    std::set<std::string> stationNames;
    for (const auto &fileName : fileNames)
    {
        qDebug() << "Loading: " << QString::fromStdString(fileName);
        WaveformHelper<T> waveformHelper;
        SFF::SAC::Waveform sacWaveform;
        SFF::Utilities::Time startTime(t0.count()*1.e-6);
        SFF::Utilities::Time endTime(t1.count()*1.e-6);
        try
        {
            sacWaveform.read(fileName, startTime, endTime);
        }
        catch (const std::exception &e)
        {
            qWarning() << "Error loading file.  Failed with " << e.what();
            continue;
        }
        // Figure out some header information
        auto network = sacWaveform.getHeader(SFF::SAC::Character::KNETWK);
        auto station = sacWaveform.getHeader(SFF::SAC::Character::KSTNM);
        auto channel = sacWaveform.getHeader(SFF::SAC::Character::KCMPNM);
        auto locationCode = sacWaveform.getHeader(SFF::SAC::Character::KHOLE);
        auto azimuth = sacWaveform.getHeader(SFF::SAC::Double::CMPAZ);
        auto inclination = sacWaveform.getHeader(SFF::SAC::Double::CMPINC);
        auto latitude = sacWaveform.getHeader(SFF::SAC::Double::STLA);
        auto longitude = sacWaveform.getHeader(SFF::SAC::Double::STLO);
        network = removeBlanksAndCapitalize(network);
        station = removeBlanksAndCapitalize(station);
        channel = removeBlanksAndCapitalize(channel);
        locationCode = removeBlanksAndCapitalize(locationCode);
        if (network == "-12345")
        {
            qCritical() << "Unable to read network code";
            continue;
        }
        if (station == "-12345")
        {
            qCritical() << "Unable to read station code";
            continue;
        }
        if (channel == "-12345")
        {
            qCritical() << "Unable to read channel code";
            continue;
        }
        if (channel.size() != 3)
        {
            qCritical() << "Invalid channel code length"; continue;
        }
        if (locationCode == "-12345"){locationCode = "01";}
        // Copy some information 
        QPhase::Waveforms::Segment<T> segment; 
        auto traceStartTime = sacWaveform.getStartTime().getEpoch();
        auto nSamples = sacWaveform.getNumberOfSamples();
        auto dataPtr  = sacWaveform.getDataPointer();
        segment.setSamplingRate(sacWaveform.getSamplingRate());
        segment.setStartTime(traceStartTime);
        segment.setData(nSamples, dataPtr);
        // Add the segment
        waveformHelper.waveform.setSegments(std::move(segment));
        waveformHelper.network = network;
        waveformHelper.station = station;
        waveformHelper.channel = channel;
        waveformHelper.locationCode = locationCode; 
        if (std::abs(azimuth - -12345) > 1.e-8)
        {
            waveformHelper.azimuth = azimuth;
            waveformHelper.haveAzimuth = true;
        }
        if (std::abs(inclination - -12345) > 1.e-8)
        {
            inclination = inclination - 90;
            inclination = std::min(90., std::max(-90., inclination));
            waveformHelper.inclination = inclination;
            waveformHelper.haveInclination = true;
        }
        if (std::abs(latitude - -12345) > 1.e-8)
        {
            waveformHelper.latitude = latitude;
            waveformHelper.haveLatitude = true;
        }
        if (std::abs(longitude - -12345) > 1.e-8)
        {
            waveformHelper.longitude = longitude;
            waveformHelper.haveLongitude = true;
        }
        // Save it
        workSpace.push_back(std::move(waveformHelper));
        auto stationName = network + "." + station + "." + locationCode;
        if (stationNames.find(stationName) == stationNames.end())
        {
            stationNames.insert(stationName);
        }
    }
    // Define a heirarchy of sampling rates
    std::vector<std::string> sensorSamplings{"G", "H", "B", "E", "S"};
    // Define a heirarchy of high-gain vs. strong motion
    std::vector<std::string> instrumentTypes{"H", "N", "L"};
    // Define a heirarchy of channel codes
    std::vector<std::array<std::string, 3>> channelTriplets
    {
        std::array<std::string, 3> {"Z", "N", "E"},
        std::array<std::string, 3> {"Z", "1", "2"},
        std::array<std::string, 3> {"Z", "R", "T"},
        std::array<std::string, 3> {"L", "Q", "T"},
        std::array<std::string, 3> {"1", "2", "3"}
    };
    std::vector<std::array<std::string, 3>> protoTypeChannels;
    protoTypeChannels.reserve(sensorSamplings.size()
                             *instrumentTypes.size()
                             *channelTriplets.size());
    for (const auto &instrumentType : instrumentTypes)
    {
        for (const auto &sensorSampling : sensorSamplings)
        {
            for (const auto &channelTriplet : channelTriplets)
            {
                auto prefix = sensorSampling + instrumentType;
                auto c1 = prefix + channelTriplet[0];
                auto c2 = prefix + channelTriplet[1];
                auto c3 = prefix + channelTriplet[2];
                protoTypeChannels.push_back(
                   std::array<std::string, 3> {c1, c2, c3});
            }
        }
    }
    // Merge
    auto nStations = static_cast<int> (stationNames.size());
    nRead = static_cast<int> (workSpace.size());
    qDebug() << "Number of (stations,waveforms): ("
             << nStations << "," << nRead << ")";
    std::vector<bool> lDone(nRead, false);
    // For each station
    for (const auto &stationName : stationNames)
    {
        std::vector<std::string> nameToSplit; 
        boost::split(nameToSplit, stationName, boost::is_any_of("."));
        QPhase::Waveforms::Station<T> seismicStation;
        seismicStation.setNetworkCode(nameToSplit.at(0));
        seismicStation.setName(nameToSplit.at(1));
        // Find the 3C sensors - this is basically an iterative loop since
        // a station may have multiple 3C sensors
        for (int k = 0; k < nRead; ++k)
        {
            int iVertical =-1;
            int iNorth =-1;
            int iEast =-1;
            for (int i = 0; i < nRead; ++i)
            {
                if (lDone[i]){continue;}
                auto network = workSpace[i].network;
                auto station = workSpace[i].station;
                auto channel = workSpace[i].channel;
                auto locationCode = workSpace[i].locationCode;
                auto thisStationName = network + "." + station + "." + locationCode;
                // Station match
                if (thisStationName == stationName)
                {
                    // Process channel triplets
                    for (const auto &protoTypeChannel : protoTypeChannels)
                    {
                        if (channel == protoTypeChannel[0])
                        {
                            iVertical = i;
                            break;
                        }
                        else if (channel == protoTypeChannel[1])
                        {
                            iNorth = i;
                            break;
                        }
                        else if (channel == protoTypeChannel[2])
                        {
                            iEast = i;
                            break;
                        }
                    }
                }
            } // End loop on read waveforms
            if (iVertical >= 0 && iNorth >= 0 && iEast >= 0)
            {
                std::cout << "Pairing: " << iVertical << " " << iNorth << " " << iEast << std::endl; 
                lDone[iVertical] = true;
                lDone[iNorth] = true;
                lDone[iEast] = true;
                QPhase::Waveforms::ThreeChannelSensor<T> sensor;
                sensor.setLocationCode(workSpace.at(iVertical).locationCode);
                if (workSpace[iVertical].haveLatitude)
                {
                    sensor.setLatitude(workSpace[iVertical].latitude);
                }
                if (workSpace[iVertical].haveLongitude)
                {
                    sensor.setLatitude(workSpace[iVertical].longitude);
                }
                auto vertical = waveformHelperToChannel<T>(
                    std::move(workSpace[iVertical]));
                auto north =  waveformHelperToChannel<T>(
                    std::move(workSpace.at(iNorth)));
                auto east = waveformHelperToChannel<T>(
                    std::move(workSpace.at(iEast)));
                sensor.setVerticalChannel(std::move(vertical));
                sensor.setNorthChannel(std::move(north));
                sensor.setEastChannel(std::move(east));
                seismicStation.add(std::move(sensor));
            }
        } // End iterative loop
        // Now what remains are single channel stations
        for (int i = 0; i < nRead; ++i)
        {
            if (lDone[i]){continue;}
            auto network = workSpace[i].network;
            auto station = workSpace[i].station;
            auto channel = workSpace[i].channel;
            auto locationCode = workSpace[i].locationCode;
            auto thisStationName = network + "." + station + "." + locationCode;
            // Station match
            if (thisStationName == stationName)
            {
                QPhase::Waveforms::SingleChannelSensor<T> sensor;
                sensor.setLocationCode(workSpace.at(i).locationCode);
                if (workSpace[i].haveLatitude)
                {
                    sensor.setLatitude(workSpace[i].latitude);
                }
                if (workSpace[i].haveLongitude)
                {
                    sensor.setLatitude(workSpace[i].longitude);
                }
                auto channel
                    = waveformHelperToChannel<T>(std::move(workSpace[i]));
                sensor.setVerticalChannel(std::move(channel));
                seismicStation.add(std::move(sensor));
            }
        }
        if (seismicStation.getNumberOfChannels() > 0)
        {
            result.push_back(std::move(seismicStation));
        }
    }
    return result;
}

template std::vector<QPhase::Waveforms::Station<double>>
    QPhase::QNode::loadSACFiles(const std::vector<std::string> &fileNames,
                                const std::chrono::microseconds &t0,
                                const std::chrono::microseconds &t1);
