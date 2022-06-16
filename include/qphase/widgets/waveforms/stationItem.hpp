#ifndef QPHASE_WIDGETS_WAVEFORMS_STATIONITEM_HPP
#define QPHASE_WIDGETS_WAVEFORMS_STATIONITEM_HPP
#include "qphase/widgets/waveforms/enums.hpp"
#include <QGraphicsRectItem>
#include <memory>
QT_BEGIN_NAMESPACE
 class QPainter;
 class QPainterPath;
 class QStyleOptionGraphicsItem;
 class QWidget;
QT_END_NAMESPACE
namespace QPhase
{
 namespace Waveforms
 {
  template<class T> class Station;
 }
 namespace Database::Internal
 {
  class Arrival;
 }
 namespace Widgets::Waveforms
 {
  class ChannelItem;
 }
}
namespace QPhase::Widgets::Waveforms
{
/// @class StationItem "stationItem.hpp" "qphase/widgets/waveforms/stationItem.hpp"
/// @brief The graphics item that holds the channels for a given station.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class StationItem : public QGraphicsRectItem
{
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    explicit StationItem(const QRectF &globalShape,
                         QGraphicsItem *parent = nullptr);
    /// @brief Constructor with a station.
    template<typename U>
    StationItem(const QPhase::Waveforms::Station<U> &stationWaveforms,
                const QRectF &globalShape,
                QGraphicsItem *parent = nullptr);
    /// @}

    /// @brief Sets the station data to plot.
    template<typename U>
    void setWaveforms(const QPhase::Waveforms::Station<U> &stationWaveforms); 

    /// @result The name of the station.
    [[nodiscard]] QString getName() const noexcept;
 
    /// @result The number of channels.
    [[nodiscard]] int getNumberOfChannels() const noexcept;

    /// @brief Sets the time limits.
    /// @param[in] plotLimits  plotLimits.first is the start time (UTC) in
    ///                        microseconds since the epoch of the plot while
    ///                        plotLimits.second is the end time (UTC) in
    ///                        microseconds since the epoch of the plot.
    /// @throws std::invalid_argument if plotLimits.first > plotLimits.second.
    void setAbsoluteTimeLimits(const std::pair<std::chrono::microseconds, 
                                               std::chrono::microseconds> &plotLimits);


    /// @name Base Class Overrides
    /// @{

    /// @result The item's bounding rectangle in global coordinates.
    [[nodiscard]] QRectF boundingRect() const override;
    /// @result The item's shape in local coordinates.
    [[nodiscard]] QPainterPath shape() const override;
    /// @brief Plots a trace.
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
    //           QWidget *widget = nullptr) override;
    /// @}


    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~StationItem() override;
    /// @}

    StationItem(const StationItem &) = delete;
    StationItem(StationItem &&) noexcept = delete;
    StationItem& operator=(const StationItem &) = delete;
    StationItem& operator=(StationItem &&) noexcept = delete;
private:
    class StationItemImpl;
    std::unique_ptr<StationItemImpl> pImpl;
};
}
#endif
