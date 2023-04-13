#ifndef QPHASE_WIDGETS_WAVEFORMS_REAL_TIME_CHANNEL_VIEW_HPP
#define QPHASE_WIDGETS_WAVEFORMS_REAL_TIME_CHANNEL_VIEW_HPP
#include <memory>
#include <chrono>
#include <QGraphicsView>
#include "qphase/widgets/waveforms/enums.hpp"
QT_BEGIN_NAMESPACE
 class QResizeEvent;
QT_END_NAMESPACE
namespace QPhase
{
 namespace Database::Connection
 {
  class IConnection;
 }
 namespace Database::Internal
 {
  class Event;
 }
 namespace Waveforms
 {
  template<class T> class Channel;
 }
 namespace Widgets::Waveforms
 {
  class ChannelItem;
 }
}
namespace QPhase::Widgets::Waveforms::RealTime
{
/// @class ChannelView "channelView.hpp" "qphase/widgets/realTime/channelView.hpp"
/// @brief This is a QGraphicView that manages a ChannelScene.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class ChannelView : public QGraphicsView
{
    Q_OBJECT;
public:
    /// @brief Constructor with a given parent.
    explicit ChannelView(QWidget *parent = nullptr);

    //void setDatabaseConnection(std::shared_ptr<QPhase::Database::Connection::IConnection> &connection);

    /// @brief Sets the plot's time conventions.
    void setTimeConvention(TimeConvention convention) noexcept;
    /// @brief Sets the plot's temporal limits.
    /// @param[in] plotLimits  plotLimits.first is the start time of the plot
    ///                        while plotLimits.second is the end time of the
    ///                        plot.  
    void setTimeLimits(const std::pair<std::chrono::microseconds, std::chrono::microseconds> &plotLimits);
    /// @brief Sets the channels to plot.
    void setChannels(std::shared_ptr<std::vector<QPhase::Waveforms::Channel<double>>> &channel);

    /// @brief Sets the event information.
    /// @param[in] event  The event information that is currently being processed.
    void setEvent(const QPhase::Database::Internal::Event &event);
    /// @result The event being processed.
    [[nodiscard]] QPhase::Database::Internal::Event getEvent() const;
    /// @brief Clears the event.
    void clearEvent() noexcept; 

    /// @brief Forces the scene to be redrawn.
    void redrawScene();
    /// @brief Destructor.
    ~ChannelView() override;
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    class ChannelViewImpl;
    std::unique_ptr<ChannelViewImpl> pImpl;
};
}
#endif
