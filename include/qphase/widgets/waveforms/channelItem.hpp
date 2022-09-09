#ifndef QPHASE_WIDGETS_WAVEFORMS_CHANNELITEM_HPP
#define QPHASE_WIDGETS_WAVEFORMS_CHANNELITEM_HPP
#include <QGraphicsRectItem>
#include <memory>
#include <set>
#include "qphase/widgets/waveforms/enums.hpp"
QT_BEGIN_NAMESPACE
 class QFont;
 class QGraphicsSceneMouseEvent; 
 class QPainter;
 class QPainterPath;
 class QPen;
 class QStyleOptionGraphicsItem;
 class QString;
 class QWidget;
QT_END_NAMESPACE
namespace QPhase
{
 namespace Waveforms
 {
  template<class T> class Channel;
 }
 namespace Database::Internal
 {
  class Arrival;
 }
}
namespace QPhase::Widgets::Waveforms
{
/// @class ChannelItem "channelItem.hpp" "qphase/widgets/waveforms/channelItem.hpp"
/// @brief The graphics item that draws the waveform and auxiliary items (e.g.,
///        picks, characteristic functions, etc.)
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
template<class T = double>
class ChannelItem : public QGraphicsRectItem
{
public:
    /// @brief Constructor.
    explicit ChannelItem(const QRectF &globalShape, QGraphicsItem *parent = nullptr);
    /// @brief Constructor from a given channel.
    ChannelItem(const QPhase::Waveforms::Channel<T> &channel,
                const QRectF &globalShape,
                WaveformType waveformType = WaveformType::Seismogram,
                QGraphicsItem *parent = nullptr);
 
    /// @name Name
    /// @{

    /// @brief Sets the trace item's name.
    /// @param[in] name  The name of the trace.
    void setName(const std::string &name);
    /// @result The name of the trace item.
    [[nodiscard]] std::string getName() const noexcept;
    void setNamePen(const QPen &pen);
    void setNameFont(const QFont &font);
    /// @}

    /// @name Time Limits
    /// @{

    void setAbsoluteTimeLimits(const std::pair<std::chrono::microseconds, 
                                               std::chrono::microseconds> &plotLimits);
    /// @}
   
    /// @name Waveform
    /// @{

    /// @brief Sets the channel to plot.
    /// @param[in] channel       The channel to plot.
    /// @param[in] waveformType  The waveform to plot.
    void setWaveform(const QPhase::Waveforms::Channel<T> &channel,
                     WaveformType waveformType = WaveformType::Seismogram);
    /// @brief Sets the waveform's name.
    void setName(const QString &name);
    /// @}

    /// @name Arrival
    /// @{

    [[nodiscard]] std::set<QPhase::Database::Internal::Arrival> getArrivals() const;
    /// @}

    /// @name Base Class Overrides
    /// @{

    /// @result The item's bounding rectangle in global coordinates.
    [[nodiscard]] QRectF boundingRect() const override;
    /// @result The item's shape in local coordinates.
    [[nodiscard]] QPainterPath shape() const override;
    /// @brief Plots a trace.
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~ChannelItem() override;
    /// @}

    ChannelItem(const ChannelItem &) = delete; 
    ChannelItem(ChannelItem &&) noexcept = delete;
    ChannelItem& operator=(const ChannelItem &) = delete;
    ChannelItem& operator=(ChannelItem &&) noexcept = delete;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; 
private:
    void drawGrid(QPainter *painter);
    void drawName(QPainter *painter);
private:
    class ChannelItemImpl;
    std::unique_ptr<ChannelItemImpl> pImpl;
};
}
#endif
