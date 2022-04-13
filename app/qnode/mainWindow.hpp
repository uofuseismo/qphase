#ifndef QNODE_MAINWINDOW_HPP
#define QNODE_MAINWINDOW_HPP
#include <memory>
#include <QMainWindow>
namespace QPhase::QNode
{
class Topics;
}
QT_BEGIN_NAMESPACE
class QStatusBar;
class QToolBar;

class QGraphicsView;
//class QListView;
class QTableView;
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
private:
    void createMenus();
    void createMainToolBar();
    void createStatusBar();
    void loadDatabase(const std::string &fileName);
private slots:
    //void aboutQt();
private:
    std::shared_ptr<QPhase::QNode::Topics> mTopics{nullptr};
    QGraphicsView *mTraceView{nullptr};
    QTableView *mEventTableView{nullptr};
    QStatusBar *mStatusBar{nullptr};
    QToolBar *mMainToolBar{nullptr};
};
}
#endif
