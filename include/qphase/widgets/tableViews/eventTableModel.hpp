#ifndef QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEMODEL_HPP
#define QPHASE_WIDGETS_TABLEVIEWS_EVENTTABLEMODEL_HPP
#include <QAbstractTableModel>
namespace QPhase::Database::Internal
{
class Event;
}

namespace QPhase::Widgets::TableViews
{
class EventTableModel : public QAbstractTableModel
{
//    Q_OBJECT
public:
    /// @brief Constructor.
    EventTableModel(QObject *parent = nullptr);

    /// @brief Sets the events.
    void populateData(std::vector<QPhase::Database::Internal::Event> &&events) noexcept;
    void populateData(const std::vector<QPhase::Database::Internal::Event> &events);
    /// @result The number of rows under the given parent.
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /// @result The number of columns under the given parent.
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    /// @result The data stored under the given role for the item referred
    ///         to by the index.
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;
    /// @result The data for the given role and section in the header with the
    ///         specified orientation.
    [[nodiscard]] QVariant headerData(int section,
                                      Qt::Orientation orientation,
                                      int role = Qt::DisplayRole) const override;

    /// @brief Destructor.
    ~EventTableModel() override;

    EventTableModel(const EventTableModel &) = delete;
    EventTableModel(EventTableModel &&) noexcept = delete;
    EventTableModel& operator=(const EventTableModel &) = delete;
    EventTableModel& operator=(EventTableModel &&) noexcept = delete;
private:
    class EventTableModelImpl;
    std::unique_ptr<EventTableModelImpl> pImpl;
};
}
#endif
