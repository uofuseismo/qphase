#include <iostream>
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
#include "load.hpp"
#include "utilities.hpp"
#include "qphase/database/internal/arrival.hpp"
#include "qphase/database/internal/arrivalTable.hpp"
#include "qphase/database/internal/event.hpp"
#include "qphase/database/internal/eventTable.hpp"
#include "qphase/database/internal/origin.hpp"
#include "qphase/database/internal/stationData.hpp"
#include "qphase/database/internal/stationDataTable.hpp"
#include "qphase/database/internal/waveform.hpp"
#include "qphase/database/internal/waveformTable.hpp"
#include "qphase/widgets/tableViews/eventTableView.hpp"
#include "qphase/widgets/tableViews/eventTableModel.hpp"
#include "qphase/widgets/waveforms/stationView.hpp"
#include "qphase/widgets/waveforms/postProcessing/traceView.hpp"
#include "qphase/waveforms/station.hpp"
#include "private/haveMap.hpp"
#if QPHASE_HAVE_QGVIEW == 1
#include "qphase/widgets/map/mainWindow.hpp"
#include "qphase/widgets/map/map.hpp"
#include "qphase/widgets/map/station.hpp"
#include "qphase/widgets/map/event.hpp"
#endif

using namespace QPhase::QNode;

namespace
{
/*
#include <sff/sac/waveform.hpp>
std::vector<QPhase::Waveforms::Station<double>> load(
    const std::vector<std::string> &fileNames,
    const std::pair<std::chrono::microseconds,
                    std::chrono::microseconds> &times)
{
    std::vector<QPhase::Waveforms::Station<double>> stations;
    for (const auto &fileName : fileNames)
    {
        SFF::SAC::Waveform sac;
        sac.read(fileName);
    }
    return stations;
}
*/
}

/// C'tor
MainWindow::MainWindow(std::shared_ptr<Topics> &topics, QWidget *parent) :
    QMainWindow(parent),
    mTopics(topics)
{
    resize(1250, 600);
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
    mEventTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    mStationView = new QPhase::Widgets::Waveforms::StationView();//PostProcessing::TraceView();

    mainLayout->addWidget(mMainToolBar);
    eventAndTraceViewSplitter->addWidget(mEventTableView);
    eventAndTraceViewSplitter->addWidget(mStationView); //TraceView);
    mainLayout->addWidget(eventAndTraceViewSplitter);
    mainLayout->addWidget(mStatusBar); 

#if QPHASE_HAVE_QGVIEW == 1
    mMap = new QPhase::Widgets::Map::MainWindow (this);
#endif

    createSlots();

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

/// Destructor
MainWindow::~MainWindow() = default;

/// Hook up slots
void MainWindow::createSlots()
{
    // When an entry in the event table view is double clicked this will query
    // the database, load waveforms, and redraw the map
    connect(mEventTableView, &QTableView::doubleClicked,
            this, [=]()
            {
                auto selectedEvents = mEventTableView->getSelectedEvents();
                if (!selectedEvents.empty())
                {
                    if (selectedEvents.size() > 1)
                    {
                        qWarning() << "Multiple selections.  Processing first.";
                    }
                    QString message{"Processing event: "};
                    auto eventIdentifier = selectedEvents.at(0).getIdentifier();
                    auto originTime = selectedEvents.at(0).getOrigin().getTime();
                    auto plotTime0 = originTime
                                   - std::chrono::microseconds {30*1000000};
                    auto plotTime1 = originTime
                                   + std::chrono::microseconds {5*60*1000000};
                    message = message + QString::number(eventIdentifier);
                    mStatusBar->showMessage(message);
                    if (mTopics->mInternalDatabaseConnection != nullptr)
                    {
                        // Arrivals
                        std::vector<Database::Internal::Arrival> arrivals;
                        QPhase::Database::Internal::ArrivalTable arrivalTable;
                        try
                        {
                            arrivalTable.setConnection(
                               mTopics->mInternalDatabaseConnection);
                            arrivalTable.query(
                               selectedEvents.at(0).getOrigin().getIdentifier());
                            arrivals = arrivalTable.getArrivals();
#if QPHASE_HAVE_QGVIEW == 1
                            try
                            {
                                if (mMap)
                                {
                                    QPhase::Widgets::Map::Event
                                        mapEvent(selectedEvents.at(0));
                                    mMap->getMapPointer()->updateEvents(
                                        std::vector<QPhase::Widgets::Map::Event>
                                                   {mapEvent});
                                }
                            }
                            catch (const std::exception &e)
                            {
                                qCritical() << "Failed to make map: " << e.what();
                            }
#endif
                        }
                        catch (const std::exception &e)
                        {
                            qCritical() << e.what();
                        }
                        // Waveforms
                        std::vector<Database::Internal::Waveform> waveformsInTable;
                        QPhase::Database::Internal::WaveformTable waveformTable;
                        try
                        {
                            waveformTable.setConnection(
                               mTopics->mInternalDatabaseConnection);
                            waveformTable.query(eventIdentifier);
                            waveformsInTable = waveformTable.getWaveforms();
                        }
                        catch (const std::exception &e)
                        {
                            qCritical() << e.what();
                        }
                        // Read in the waveforms
                        std::vector<std::string> sacFileNames;
                        for (const auto &w : waveformsInTable)
                        {
                            sacFileNames.push_back(w.getFileName());
                        }
                        auto stations
                           = std::make_shared<std::vector<QPhase::Waveforms::Station<double>>>
                                     (loadSACFiles<double>(sacFileNames,
                                                             plotTime0,
                                                             plotTime1));
                        qDebug() << "Read" << stations->size() << "stations";
                        mStationView->setStations(stations);
                    } // End check on database connection
                    // Start doing plotting
                    mStationView->setTimeLimits(std::pair(plotTime0, plotTime1));
qDebug() << "set it";
                    mStationView->setEvent(selectedEvents.at(0));
qDebug() << "set";
                }
                else // No events
                {
                    qCritical() << "Could not find event";
                }
            });

}

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

#ifdef QPHASE_HAVE_QGVIEW
    // Map icon
    const QIcon mapIcon(":/images/map_icon.png");
    auto mainMapAction = new QAction(mapIcon, tr("Map"));
    mainMapAction->setToolTip("Map view of stations and events");
    mMainToolBar->addAction(mainMapAction);
    connect(mainMapAction, &QAction::triggered,
            [=]()
            {
                if (!mMap)
                {
                    mMap = new QPhase::Widgets::Map::MainWindow (this);
                    mMap->show();
                }
                else
                {
                    mMap->show();
                }
            });
#endif
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

    if (mMap)
    {
        QPhase::Database::Internal::StationDataTable stationTable;
        stationTable.setConnection(mTopics->mInternalDatabaseConnection);
        stationTable.queryAll();
        auto stations = stationTable.getStations();
        std::vector<QPhase::Widgets::Map::Station> mapStations;
        for (const auto &station : stations)
        {
            try
            {
                QPhase::Widgets::Map::Station mapStation(station);
                mapStations.push_back(mapStation);
            }
            catch (const std::exception &e)
            {
                qWarning() << e.what();
            }
        }
        mMap->getMapPointer()->updateStations(mapStations);
    }
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

