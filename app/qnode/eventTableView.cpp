#include <array>
#include <algorithm>
#include "eventTableView.hpp"
#include "eventTableModel.hpp"

using namespace QPhase::QNode;

class EventTableView::EventTableViewImpl
{
public:
    EventTableModel *mTableModel{nullptr}; 
    std::array<int, 3> mTableWidths{95, 160, 95};
    int mTableWidth = std::accumulate(mTableWidths.begin(),
                                      mTableWidths.end(), 0) + 3;
};

/// C'tor
EventTableView::EventTableView(QWidget *parent) :
    QTableView(parent),
    pImpl(std::make_unique<EventTableViewImpl> ())
{
    pImpl->mTableModel = new EventTableModel();

    setModel(pImpl->mTableModel);
 
    setStyleSheet("QTableView::item:selected{background-color: rgb(204,0,0);}");
    setSelectionBehavior(QAbstractItemView::SelectRows);
/*
    // Make the event model and connect to the tableview
    //mEventModel = new ComcatEventModel(this);
    auto events = pImpl->mEventList.getEvents();
    mEventModel->populateData(events);
    mTableView->setModel(mEventModel);
*/
    setMinimumWidth(pImpl->mTableWidth);
    setMaximumWidth(pImpl->mTableWidth);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnWidth(0, 95);
    setColumnWidth(1, 160);
    setColumnWidth(2, 95);

}

/// Destructor
EventTableView::~EventTableView() = default;
