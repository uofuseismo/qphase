#ifndef QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEVIEW_HPP
#define QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEVIEW_HPP
#include <memory>
#include <QTableView>
namespace QPhase::QNode
{
class EventTableModel;
}

// namespace QPhase::Widgets::TableViews::EventTableModel
namespace QPhase::QNode
{
class EventTableView : public QTableView
{
    //Q_OBJECT
public:
    EventTableView(QWidget *parent = nullptr);
    ~EventTableView() override;

    EventTableView(const EventTableView &) = delete;
    EventTableView(EventTableView &&) noexcept = delete;
    EventTableView& operator=(const EventTableView &) = delete;
    EventTableView& operator=(EventTableView &&) noexcept = delete;
private:
    class EventTableViewImpl;
    std::unique_ptr<EventTableViewImpl> pImpl; 
};
}
#endif
