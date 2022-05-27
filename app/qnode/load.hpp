#ifndef QNODE_LOAD_HPP
#define QNODE_LOAD_HPP
#include <vector>
#include <chrono>
#include "qphase/waveforms/station.hpp"
namespace QPhase::QNode
{
    constexpr std::chrono::microseconds t0{-2208988800*1000000}; // Year 1900 
    constexpr std::chrono::microseconds t1{ 4102444800*1000000}; // Year 2100

template<typename T>
std::vector<QPhase::Waveforms::Station<T>>
    loadSACFiles(const std::vector<std::string> &fileNames,
                 const std::chrono::microseconds &t0 = std::chrono::microseconds{-2208988800*1000000},
                 const std::chrono::microseconds &t1 = std::chrono::microseconds{ 4102444800*1000000});

}
#endif
