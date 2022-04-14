#include <QGraphicsView>
#include <QHBoxLayout>
#include <QIcon>
#include <QTableView>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QToolBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <filesystem>
#include <qphase/version.hpp>
#include "mainWindow.hpp"
#include "topics.hpp"
#include "utilities.hpp"
#include "qphase/database/internal/eventTable.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/widgets/tableViews/eventTableView.hpp"
#include "qphase/widgets/tableViews/eventTableModel.hpp"

using namespace QPhase::QNode;

/// C'tor
MainWindow::MainWindow(std::shared_ptr<Topics> &topics, QWidget *parent) :
    QMainWindow(parent),
    mTopics(topics)
{
    resize(1200, 600);
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
 
    auto eventAndTraceViewSplitter = new QSplitter(Qt::Horizontal);

    mEventTableModel = new QPhase::Widgets::TableViews::EventTableModel ();
    mEventTableView
        = new QPhase::Widgets::TableViews::EventTableView();
    mEventTableView->setModel(mEventTableModel);

    mTraceView = new QGraphicsView();

    mainLayout->addWidget(mMainToolBar);
    eventAndTraceViewSplitter->addWidget(mEventTableView);
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
    openAction->setToolTip(tr("Open catalog"));
    connect(openAction, &QAction::triggered,
            [=]()
            {
                const QString defaultDirectory{"defaultFileDirectory"}; 
                QSettings settings;
                QString currentDirectory("");
                if (settings.contains(defaultDirectory))
                {
                     currentDirectory
                         = settings.value(defaultDirectory).toString();
                }
                QString fileName
                    = QFileDialog::getOpenFileName(
                         this,
                         tr("Select catalog to open"), // Open
                         currentDirectory,
                         tr("Catalog Files (*.sqlite *.sqlite3);; All Files (*,*)"), // Filters
                         nullptr, // Selected filter
                         QFileDialog::ReadOnly); // Disable making read only
                bool loadedFile = false;
                if (!fileName.isEmpty())
                {
                    qDebug() << "Attempting to load: " << fileName;
                    loadedFile = true;
                    std::filesystem::path path{fileName.toStdString()};
                    if (path.extension() == ".sqlite" || path.extension() == ".sqlite3")
                    {
                        auto directory = path.parent_path().string();
                        settings.setValue(defaultDirectory,
                                          QString {directory.c_str()});
                    }
                    try
                    {
                        loadDatabase(fileName.toStdString());
                        loadedFile = true;
                    }
                    catch (const std::exception &e)
                    {
                        qCritical() << e.what();
                        loadedFile = false;
                    }
                }
                if (loadedFile){qDebug() << "Loaded catalog";}
            });
    mMainToolBar->addAction(openAction);

    // Map icon
    const QIcon mapIcon(":/images/map_icon.png");
    auto mainMapAction = new QAction(mapIcon, tr("Map"));
    mainMapAction->setToolTip("Map view of stations and events");
    mMainToolBar->addAction(mainMapAction);
}

/// Loads the database
void MainWindow::loadDatabase(const std::string &fileName)
{
    constexpr bool readOnly = false;
    if (mTopics->mInternalDatabaseConnection != nullptr)
    {
        qInfo() << "Closing previous database";
    }
    mTopics->mInternalDatabaseConnection
        = createSQLite3Connection(fileName, readOnly);
    QPhase::Database::Internal::EventTable eventTable;
    eventTable.setConnection(mTopics->mInternalDatabaseConnection);
    eventTable.queryAll();
    auto eventTableModel = new QPhase::Widgets::TableViews::EventTableModel();
    eventTableModel->populateData(eventTable.getEvents());
    mEventTableModel = std::move(eventTableModel);
    refreshEventList();
}

/// Refreshes the event list
void MainWindow::refreshEventList()
{
    if (mEventTableView != nullptr)
    {
        mEventTableView->setModel(mEventTableModel);
    }
    else
    {
        qWarning() << "Event table model view is NULL";
    }
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
    exitAction->setToolTip(tr("Exit the application."));
    //-----------------------------------Help---------------------------------//
    auto helpMenu = menuBar()->addMenu(tr("&Help"));
    QIcon applicationIcon(":/images/BlockU_redweb.png");
    auto aboutAction   = new QAction(applicationIcon, tr("About QNode"));
    aboutAction->setToolTip(tr("More about QNode"));
    connect(aboutAction,   &QAction::triggered,
            [=]()
            {
               QMessageBox::about(this, tr("About QNode"),
                  tr("<b>QNode</b> helps in generating picks on nodal datasets.  "
                     "It is driven by QPhase v" QPHASE_VERSION
                     "<p>Copyright (C) University of Utah Seismograph "
                     "Stations, 2022."));
            });
    QIcon qtIcon(":/images/qt_extended_32x32.png");
    auto aboutQtAction = new QAction(qtIcon, tr("About Qt"));
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

