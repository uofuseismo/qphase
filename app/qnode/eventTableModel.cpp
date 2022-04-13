#include <vector>
#include <QColor>
#include <QDateTime>
#include <QList>
#include <QSize>
#include <QString>
#include <QString>
#include <QVariant>
#include "eventTableModel.hpp"
#include "qphase/database/internal/event.hpp"

//using namespace QPhase::Widgets::AbstractTableModel::EventTableModel
using namespace QPhase::QNode;

class EventTableModel::EventTableModelImpl
{
public:
    std::vector<QPhase::Database::Internal::Event> mEvents;
    std::vector<QString> mColumnNames{ "ID", "Time (UTC)", "Magnitude"};
};

/// C'tor
EventTableModel::EventTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    pImpl(std::make_unique<EventTableModelImpl> ()) 
{
}

/// Destructor
EventTableModel::~EventTableModel() = default;

/*
/// Set the data
void EventTableModel::populateData(
    const std::vector<Qphase::WebServices::Comcat::Event> &events)
{
    pImpl->mEvents = events;
}
*/

/// Table rows
int EventTableModel::rowCount(const QModelIndex &) const
{
    return static_cast<int> (pImpl->mEvents.size());
}

/// Table columns 
int EventTableModel::columnCount(const QModelIndex &) const
{
    //Q_UNUSED(parent);
    return static_cast<int> (pImpl->mColumnNames.size());
}

/// Put the event information in a format Qt can show in a table
QVariant EventTableModel::data(
    const QModelIndex &index, const int role) const
{
    if (!index.isValid()){return QVariant();}

    return QVariant();
}

/// Header
QVariant EventTableModel::headerData(
    const int section, const Qt::Orientation orientation, const int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return pImpl->mColumnNames.at(section);
        }
    }
    return QVariant();
}
