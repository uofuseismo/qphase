#include <QGraphicsView>
#include <QHBoxLayout>
#include <QIcon>
#include <QListView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QToolBar>
#include <QSplitter>
#include <QVBoxLayout>
#include "mainWindow.hpp"
#include <qphase/version.hpp>

using namespace QPhase::QNode;

/// C'tor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(1100, 600);
    setWindowIcon(QIcon(":images/BlockU_redweb.png"));
    setWindowTitle(tr("QNode"));
    // Create the menus, main toolbar, graphics view, etc.
    createMenus();
    createMainToolBar();
    createStatusBar();
    // Create containers for widgets
    // -----------------------------------------------------|
    // | Toolbar                                            |
    // -----------------------------------------------------|
    // | Events          |          Trace Gather            |
    // |----------------------------------------------------|
    // |Status Bar                                          |
    // -----------------------------------------------------|
    auto centralWidget = new QWidget();
    auto mainLayout = new QVBoxLayout();
    //mainLayout->addWidget();
 
    //auto eventAndTraceViewBox = new QHBoxLayout();
    auto eventAndTraceViewSplitter = new QSplitter(Qt::Horizontal);

    mEventListView = new QListView();
    mEventListView->setMaximumWidth(280);
    mTraceView = new QGraphicsView();

    mainLayout->addWidget(mMainToolBar);
    eventAndTraceViewSplitter->addWidget(mEventListView);
    eventAndTraceViewSplitter->addWidget(mTraceView);
    mainLayout->addWidget(eventAndTraceViewSplitter);
    mainLayout->addWidget(mStatusBar); 

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

/// Destructor
MainWindow::~MainWindow() = default;

/// Creates the main toolbar
void MainWindow::createMainToolBar()
{
    mMainToolBar = new QToolBar();
   
    // Open icon
    const auto openIcon
        = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    auto openAction = new QAction(openIcon, tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open waveforms"));
    mMainToolBar->addAction(openAction);

    // Map icon
    const QIcon mapIcon(":/images/map_icon.png");
    auto mainMapAction = new QAction(mapIcon, tr("Map"));
    mainMapAction->setStatusTip("Map view of stations and events");
    mMainToolBar->addAction(mainMapAction);
}

/// Creates the status bar
void MainWindow::createStatusBar()
{
    mStatusBar = new QStatusBar();
    mStatusBar->setStyleSheet("background-color : rgb(255,255,255);");
    mStatusBar->setFixedHeight(20);
    mStatusBar->showMessage(tr("Welcome to QNode"));
}

/// Creates the menus
void MainWindow::createMenus()
{
    //----------------------------------File----------------------------------//
    auto fileMenu = menuBar()->addMenu(tr("&File"));

    auto exitIcon = QIcon::fromTheme("application-exit");
    fileMenu->addSeparator();
    auto exitAction = fileMenu->addAction(exitIcon, tr("E&xit"),
                                          this, &QWidget::close);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application."));
    //-----------------------------------Help---------------------------------//
    auto helpMenu = menuBar()->addMenu(tr("&Help"));
    auto aboutAction   = new QAction(tr("About QNode"), this);
    aboutAction->setStatusTip(tr("More about QNode"));
    connect(aboutAction,   &QAction::triggered,
            [=]()
            {
               QMessageBox::about(this, tr("About QNode"),
                  tr("<b>QNode</b> helps in generating picks on nodal datasets.  "
                     "It is driven by QPhase v" QPHASE_VERSION
                     "<p>Copyright (C) University of Utah Seismograph "
                     "Stations, 2022."));
            });
    auto aboutQtAction = new QAction(tr("About Qt"), this);
    aboutQtAction->setStatusTip(tr("More about Qt"));
    connect(aboutQtAction, &QAction::triggered,
            [=]()
            {
                QMessageBox::aboutQt(this, tr("About Qt"));
            });
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    //---------------------------------Mac Nonsense---------------------------//
    setUnifiedTitleAndToolBarOnMac(true);
}

