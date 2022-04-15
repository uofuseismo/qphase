#include <array>
#include <algorithm>
#include <QScrollBar>
#include "qphase/widgets/tableViews/eventTableView.hpp"
#include "qphase/widgets/tableViews/eventTableModel.hpp"
#include "qphase/database/internal/event.hpp"

using namespace QPhase::Widgets::TableViews;

class EventTableView::EventTableViewImpl
{
public:
    //EventTableModel *mTableModel{nullptr}; 
    std::array<int, 3> mTableWidths{90, 170, 85};
    int mTableWidth = std::accumulate(mTableWidths.begin(),
                                      mTableWidths.end(), 0);// + 3;
};

/// C'tor
EventTableView::EventTableView(QWidget *parent) :
    QTableView(parent),
    pImpl(std::make_unique<EventTableViewImpl> ())
{
    auto tempModel = new EventTableModel();
    setModel(tempModel);
 
    setStyleSheet("QTableView::item:selected{background-color: rgb(204,0,0);}");
    this->verticalScrollBar()->setFixedWidth(13);

    setSelectionBehavior(QAbstractItemView::SelectRows);
/*
    // Make the event model and connect to the tableview
    //mEventModel = new ComcatEventModel(this);
    auto events = pImpl->mEventList.getEvents();
    mEventModel->populateData(events);
    mTableView->setModel(mEventModel);
*/
//    setMinimumWidth(pImpl->mTableWidth);
//    setMaximumWidth(pImpl->mTableWidth);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedWidth(pImpl->mTableWidth
                + this->verticalScrollBar()->width() + 5);

    setColumnWidth(0, pImpl->mTableWidths.at(0));
    setColumnWidth(1, pImpl->mTableWidths.at(1));
    setColumnWidth(2, pImpl->mTableWidths.at(2));
}

/*
void EventTableView::setModel(QAbstractTableModel *model)
{
    //pImpl->mTableModel = model;
    //::setModel(pImpl->mTableModel);
}
*/

/// Destructor
EventTableView::~EventTableView() = default;

/// Get the selections
std::vector<QPhase::Database::Internal::Event> EventTableView::getSelectedEvents() const
{
    std::vector<QPhase::Database::Internal::Event> result;
    auto selections = this->selectionModel();
    if (selections->hasSelection())
    {
        auto eventModel = reinterpret_cast<EventTableModel *> (this->model());
        auto rows = selections->selectedRows();
        result.reserve(rows.size());
        for (const auto &row : rows)
        {
            auto data = eventModel->data(row);
            auto eventIdentifier = static_cast<int64_t> (data.toLongLong());
            try
            {
                result.push_back(eventModel->getEvent(eventIdentifier));
            }
            catch (const std::exception &e)
            {
                qCritical() << e.what();
            }
        }
    }
    return result;
}

/// Pointer to the table model
/*
EventTableModel *EventTableView::getEventTableModelPointer()
{
    return pImpl->mTableModel;
}
*/
