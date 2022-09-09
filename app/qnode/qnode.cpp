#include <iostream>
#include <filesystem>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStandardPaths>
#include "private/paths.hpp"
#include "private/organization.hpp"
#include "qphase/database/connection/sqlite3.hpp"
#include "private/database/utilities.hpp"
#include "mainWindow.hpp"
#include "topics.hpp"

std::shared_ptr<QPhase::Database::Connection::SQLite3>
    createScratchDatabase(const std::filesystem::path &fileName)
{
    auto connection
        = std::make_shared<QPhase::Database::Connection::SQLite3> ();
    if (std::filesystem::exists(fileName))
    {
        std::filesystem::remove(fileName);
    }
    connection->setFileName(fileName);
    connection->setReadWrite();
    connection->connect();
    createTable(*connection->getSession());
    deleteTable(*connection->getSession());
    return connection;
}

void myMessageOutput(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    if (type == QtDebugMsg)
    {
        fprintf(stderr, "\033[0;36m"); 
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr, "Debug: %s\n", localMsg.constData());
        fprintf(stderr, "\033[0m");
    }
    else if (type == QtInfoMsg)
    {
        fprintf(stdout, "\033[0;32m");
        fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stdout, "\033[0m");
    }
    else if (type == QtWarningMsg)
    {
        fprintf(stderr, "\033[0;33m");
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr, "\033[0m");
    }
    else if (type == QtCriticalMsg)
    {
        fprintf(stderr, "\033[0;31m");
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr, "\033[0m");
    }
    else if (type == QtFatalMsg)
    {
        fprintf(stderr, "\033[0;31m");
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr, "\033[0m");
        abort();
    }
}

int main(int argc, char *argv[])
{
    auto topics = std::make_shared<QPhase::QNode::Topics> ();

    Q_INIT_RESOURCE(qnode);
    qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);
    QIcon::setThemeName("Yaru"); // TODO get this from CMake
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setApplicationName("QNode");
    QCoreApplication::setOrganizationDomain("quake.utah.edu");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    std::string defaultDataPath(DATA_PATH);
    std::string defaultCachePath(CACHE_PATH);
    std::string defaultConfigPath(CONFIG_PATH);
    std::string defaultUser(std::getenv("USER"));

    try
    {
        auto scratchDatabase = std::filesystem::path{defaultDataPath}
                              /std::filesystem::path{"scratch.sqlite3"};
        topics->mScratchDatabaseConnection
            = createScratchDatabase(scratchDatabase);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create scratch database: "
                  << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Create the main application
    QPhase::QNode::MainWindow mainWindow(topics);
    mainWindow.show();
    auto error = QApplication::exec();
    if (error != 0){qCritical("Errors detected during execution");}
    return error;
}
