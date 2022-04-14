#include <iostream>
#include <vector>
#include <array>
#include <QColor>
#include <QDateTime>
#include <QList>
#include <QSize>
#include <QString>
#include <QString>
#include <QVariant>
#include "qphase/widgets/tableViews/eventTableModel.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/magnitude.hpp"

using namespace QPhase::Widgets::TableViews;

class EventTableModel::EventTableModelImpl
{
public:
    std::vector<QPhase::Database::Internal::Event> mEvents;
    std::array<QString, 3> mColumnNames{ "ID", "Time (UTC)", "Magnitude"};
};

/// C'tor
EventTableModel::EventTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    pImpl(std::make_unique<EventTableModelImpl> ()) 
{
}

/// Destructor
EventTableModel::~EventTableModel() = default;

/// Set the data
void EventTableModel::populateData(
    std::vector<QPhase::Database::Internal::Event> &&events) noexcept
{
    pImpl->mEvents = std::move(events);
}

void EventTableModel::populateData(
    const std::vector<QPhase::Database::Internal::Event> &events)
{
    pImpl->mEvents = events;
}

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
    if (role == Qt::BackgroundRole)
    {
        const QColor White = QColorConstants::White;
        const QColor LightGray = QColorConstants::LightGray;
        if (index.row()%2 == 0)
        {
            return White;
        }
        else
        {
            return LightGray;
        }
    }
    if (role != Qt::DisplayRole){return QVariant();}
    if (index.column() == 0)
    {
        if (pImpl->mEvents[index.row()].haveIdentifier())
        {
            auto evid = static_cast<qlonglong>
                        (pImpl->mEvents[index.row()].getIdentifier());
            return QVariant(evid);
        }
        return QVariant();
    }
    else if (index.column() == 1)
    {
        if (pImpl->mEvents[index.row()].haveOrigin())
        {
            auto origin = pImpl->mEvents[index.row()].getOrigin();
            if (origin.haveTime())
            {
                auto originTimeMicroSeconds = origin.getTime().count()*1.e-3;
                auto mSecsSinceEpoch
                    = static_cast<qint64> (std::round(originTimeMicroSeconds));
                QDateTime originTime;
                originTime.setMSecsSinceEpoch(mSecsSinceEpoch);
                return QVariant(originTime);
            }
        }
        return QVariant();
    }
    else if (index.column() == 2)
    {
        if (pImpl->mEvents[index.row()].haveMagnitude())
        {
            auto magnitude = pImpl->mEvents[index.row()].getMagnitude();
            if (magnitude.haveValue())
            {
                auto value = magnitude.getValue();
                auto sMagnitude = QString::number(value);
                if (magnitude.haveType())
                {
                    auto magnitudeType = magnitude.getType();
                    auto sMagnitudeType =
                       QString::fromStdString(" " + magnitudeType);
                    sMagnitude = sMagnitude + sMagnitudeType;
                }
                return QVariant(sMagnitude);
            }
        }
        return QVariant();
    }
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
