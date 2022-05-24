#ifndef QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEITEM_HPP
#define QPHASE_WIDGETS_WAVEFORMS_POSTPROCESSING_TRACEITEM_HPP
#include <QGraphicsRectItem>
#include <memory>
#include <set>
QT_BEGIN_NAMESPACE
 class QFont;
 class QPainter;
 class QPainterPath;
 class QPen;
 class QStyleOptionGraphicsItem;
 class QWidget;
QT_END_NAMESPACE
namespace QPhase
{
 namespace Waveforms
 {
  template<class T> class Waveform;
 }
 namespace Database::Internal
 {
  class Arrival;
 }
}
namespace QPhase::Widgets::Waveforms::PostProcessing
{
/// @class TraceItem "traceItem.hpp" "qphase/widgets/waveforms/postProcessing/traceItem.hpp"
/// @brief The graphics item that draws the trace and auxiliary items (e.g.,
///        picks, characteristic functions, etc.)
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class TraceItem : public QGraphicsRectItem
{
public:
    /// @brief Constructor.
    explicit TraceItem(const QRectF &globalShape, QGraphicsItem *parent = nullptr);

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
   
    /// @name Waveform
    /// @{

    /// @brief Sets the waveform to plot.  
    /// @param[in] waveform  The waveform to plot.
    template<typename U>
    void setWaveform(const QPhase::Waveforms::Waveform<U> &waveform);
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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    /// @}

    /// @name Destructors
    /// @{

    /// @brief Destructor.
    ~TraceItem() override;
    /// @}

    TraceItem(const TraceItem &) = delete; 
    TraceItem(TraceItem &&) noexcept = delete;
    TraceItem& operator=(const TraceItem &) = delete;
    TraceItem& operator=(TraceItem &&) noexcept = delete;

private:
    void drawName(QPainter *painter);
private:
    class TraceItemImpl;
    std::unique_ptr<TraceItemImpl> pImpl;
};
}
#endif
