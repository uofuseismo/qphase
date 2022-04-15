#ifndef QNODE_MAINWINDOW_HPP
#define QNODE_MAINWINDOW_HPP
#include <memory>
#include <QMainWindow>
namespace QPhase
{
 namespace QNode
 {
  class Topics;
 }
 namespace Widgets::TableViews
 {
  class EventTableView;
  class EventTableModel;
 }
}

QT_BEGIN_NAMESPACE
class QStatusBar;
class QToolBar;

class QGraphicsView;
//class QListView;
//class QTableView;
QT_END_NAMESPACE

namespace QPhase::QNode
{
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(std::shared_ptr<Topics> &topics,
                        QWidget *parent = nullptr); //std::shared_ptr<Topics> &topics,
//                        QWidget *parent = nullptr);
    virtual ~MainWindow();
public slots:
    void refreshEventList();
    //void eventTableViewDoubleClicked();
private:
    void createMenus();
    void createMainToolBar();
    void createStatusBar();
    void createSlots();
    void loadDatabase(const std::string &fileName);
private slots:
    //void aboutQt();
private:
    std::shared_ptr<QPhase::QNode::Topics> mTopics{nullptr};
    QPhase::Widgets::TableViews::EventTableModel *mEventTableModel{nullptr};
    QPhase::Widgets::TableViews::EventTableView *mEventTableView{nullptr}; 
    QGraphicsView *mTraceView{nullptr};
    QStatusBar *mStatusBar{nullptr};
    QToolBar *mMainToolBar{nullptr};
};
}
#endif
