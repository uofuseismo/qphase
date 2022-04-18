#include <QAction>
#include <QMainWindow>
#include <QMenuBar>
#include <QVBoxLayout>
#include "qphase/widgets/map/mainWindow.hpp"
#include "qphase/widgets/map/map.hpp"
#include "private/haveMap.hpp"

using namespace QPhase::Widgets::Map;

/// C'tor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::Window |
                        Qt::CustomizeWindowHint |
                        Qt::WindowTitleHint |
                        Qt::WindowMaximizeButtonHint |
                        Qt::WindowCloseButtonHint)
{
    setWindowTitle("Map Viewer");
    setMouseTracking(true);
    resize(900, 600);

    // Create a container to hold widgets
    auto mapBox = new QVBoxLayout();
    // Create the menus
    createMenus();

    // Create a new map
    mMap = new Map(this);

    // Set it as the central widget 
    mapBox->addWidget(mMap);
    auto centralWidget = new QWidget();
    centralWidget->setLayout(mapBox);
    setCentralWidget(centralWidget);
}

/// Create menus
void MainWindow::createMenus()
{
    //----------------------------------File----------------------------------//
    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    auto exitIcon = QIcon::fromTheme("application-exit");
    auto exitAction = fileMenu->addAction(exitIcon, tr("E&xit"),
                                          this, &QWidget::close);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the map viewer."));

    //---------------------------------Layers---------------------------------//
    auto optionsMenu = menuBar()->addMenu(tr("Map Layers"));
    auto backgroundMenu = optionsMenu->addMenu(tr("Background"));
    auto bingSatelliteAction = new QAction("Bing Satellite");
    auto osmTopoAction = new QAction("OSM Topography");
#if QPHASE_HAVE_MAPBOX == 1
    auto mapBoxSatelliteStreetsAction = new QAction("MapBox Satellite-Streets");
#endif
    backgroundMenu->addAction(bingSatelliteAction);
    backgroundMenu->addAction(osmTopoAction);
#if QPHASE_HAVE_MAPBOX == 1
    backgroundMenu->addAction(mapBoxSatelliteStreetsAction);
#endif
    connect(bingSatelliteAction, &QAction::triggered,
            this, [this]()
            {
                qDebug("Switching to Bing satellite...");
                this->mMap->selectBackground(Background::Layer::BING_SATELLITE);
            });
    connect(osmTopoAction, &QAction::triggered,
            this, [this]()
            {
                qDebug("Switching to OSM topo...");
                this->mMap->selectBackground(Background::Layer::OSM);
            });
#if QPHASE_HAVE_MAPBOX == 1
    connect(mapBoxSatelliteStreetsAction, &QAction::triggered,
            this, [this]()
            {
                qDebug("Switching to MapBox SatelliteStreets...");
                this->mMap->selectBackground(
                    Background::Layer::MAPBOX_SATELLITE_STREETS);
            });
#endif
}

/// Destructor
MainWindow::~MainWindow() = default;

/// Get pointer to underlying map 
Map *MainWindow::getMapPointer()
{
    return mMap;
}

