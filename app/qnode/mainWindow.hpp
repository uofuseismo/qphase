#ifndef QNODE_MAINWINDOW_HPP
#define QNODE_MAINWINDOW_HPP
#include <memory>
#include <QMainWindow>
/*
namespace QURTS
{
class Topics;
}
*/
QT_BEGIN_NAMESPACE
class QStatusBar;
class QGraphicsView;
class QToolBar;
QT_END_NAMESPACE
namespace QPhase::QNode
{
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr); //std::shared_ptr<Topics> &topics,
//                        QWidget *parent = nullptr);
    virtual ~MainWindow();
private:
    void createMenus();
    void createMainToolBar();
    void createStatusBar();
private slots:
    //void aboutQt();
private:
//    std::shared_ptr<QURTS::Topics> mTopics{nullptr};
    QGraphicsView *mTraceView{nullptr};
    QStatusBar *mStatusBar{nullptr};
    QToolBar *mMainToolBar{nullptr};
};
}
#endif
