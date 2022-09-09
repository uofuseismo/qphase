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
    explicit EventTableModel(QObject *parent = nullptr);

    /// @brief Sets the events.
    void populateData(std::vector<QPhase::Database::Internal::Event> &&events) noexcept;
    void populateData(const std::vector<QPhase::Database::Internal::Event> &events);
    /// @result The number of rows under the given parent.
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    /// @result The number of columns under the given parent.
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    /// @result The event corresponding to the given event identifier.
    /// @throws std::invalid_argument if no events match the specified
    ///         event identifier.
    [[nodiscard]] QPhase::Database::Internal::Event getEvent(int64_t eventIdentifier) const;
    /// @result The data stored under the given role for the item referred
    ///         to by the index.
    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;
    /// @result The data for the given role and section in the header with the
    ///         specified orientation.
    [[nodiscard]] QVariant headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const override;

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
