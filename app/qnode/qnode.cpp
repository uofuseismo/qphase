#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStandardPaths>
#include "private/paths.hpp"
#include "private/organization.hpp"
#include "mainWindow.hpp"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qnode);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setApplicationName("QNode");
    QCoreApplication::setOrganizationDomain("quake.utah.edu");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    std::string defaultDataPath(DATA_PATH);
    std::string defaultCachePath(CACHE_PATH);
    std::string defaultConfigPath(CONFIG_PATH);
    std::string defaultUser(std::getenv("USER"));

    // Create the main application
    QPhase::QNode::MainWindow mainWindow;//topics);
    mainWindow.show();
    auto error = QApplication::exec();
    if (error != 0){qCritical("Errors detected during execution");}
    return error;
}
