#include <QGraphicsView>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QToolBar>
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
 
    auto eventAndTraceViewBox = new QHBoxLayout();

    mTraceView = new QGraphicsView();

    mainLayout->addWidget(mMainToolBar);
    eventAndTraceViewBox->addWidget(mTraceView);
    mainLayout->addLayout(eventAndTraceViewBox);
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
}

/// Creates the status bar
void MainWindow::createStatusBar()
{
    mStatusBar = new QStatusBar();
    mStatusBar->showMessage(QString("Welcome to QNode"));
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
