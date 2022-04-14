#ifndef QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEVIEW_HPP
#define QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEVIEW_HPP
#include <memory>
#include <QTableView>
namespace QPhase::Widgets::TableViews
{
//class EventTableModel;
}

namespace QPhase::Widgets::TableViews
{
/// @class EventTableView "eventTableView.hpp" "qphase/widgets/tableView/eventTableView.hpp"
/// @brief Defines an event table view.  This is table with columns:
///        --------------------------
///        |Event ID|Time |Magnitude|
///        |x       |x    |x        |
///        |.       |.    |.        |
///        |.       |.    |.        |
///        |.       |.    |.        |
///        |x       |x    |x        |
///        --------------------------
/// @copyright Ben Baker (University of Utah) distributed under the MIT License.
class EventTableView : public QTableView
{
public:
    /// @name Constructors
    /// @{

    EventTableView(QWidget *parent = nullptr);
    /// @}

    /// @name Destructors
    /// @{
    ~EventTableView() override;
    /// @}

    //void setModel(QAbstractTableModel *model);
    //EventTableModel *getEventTableModelPointer();

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
