#include <iostream>
#include <string>
#include <filesystem>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "qphase/database/connection/sqlite3.hpp"

using namespace QPhase::Database::Connection;

class SQLite3::SQLite3Impl
{
public:
    soci::session mSession;
    std::string mFileName;
    bool mReadOnly = false;
};

/// C'tor
SQLite3::SQLite3() :
    pImpl(std::make_unique<SQLite3Impl> ())
{
}

/// Move c'tor
SQLite3::SQLite3(SQLite3 &&database) noexcept
{
    *this = std::move(database);
}

/// Move assignment
SQLite3& SQLite3::operator=(SQLite3 &&database) noexcept
{
    if (&database == this){return *this;}
    pImpl = std::move(database.pImpl);
    return *this;
}

/// Destructor
SQLite3::~SQLite3() = default;

/// Database driver
std::string SQLite3::getDriver() const noexcept
{
    return "sqlite3";
} 

/// File name
void SQLite3::setFileName(const std::string &fileName)
{
    if (!std::filesystem::exists(fileName))
    {
        auto parentPath = std::filesystem::path(fileName).parent_path();
        if (!parentPath.empty() && !std::filesystem::exists(parentPath))
        {
            if (!std::filesystem::create_directories(parentPath))
            {
                throw std::runtime_error("Failed to create directory: "
                                        + std::string(parentPath));
            }
        }
    }
    pImpl->mFileName = fileName;
}

std::string SQLite3::getFileName() const
{
    if (!haveFileName()){throw std::runtime_error("Filename not set");}
    return pImpl->mFileName;
}

bool SQLite3::haveFileName() const noexcept
{
    return !pImpl->mFileName.empty();
}

/// Connect to the database
void SQLite3::connect()
{
    auto fileName = getFileName(); // Throws
    auto connectionString = "db=" + fileName;
    if (isReadOnly())
    {
        if (!std::filesystem::exists(fileName))
        {
            throw std::runtime_error("sqlite3 database " + fileName
                                   + " does not exist");
        }
        connectionString = connectionString + " readonly";
    }
    try
    {
        pImpl->mSession.open(soci::sqlite3, connectionString);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to connect to sqlite3 with error:\n"
                               + std::string{e.what()});
    }
}    

/// Disconnect
void SQLite3::close()
{
    if (isConnected()){pImpl->mSession.close();}
}

/// Sets read only
void SQLite3::setReadOnly() noexcept
{
    pImpl->mReadOnly = true;
}

void SQLite3::setReadWrite() noexcept
{
    pImpl->mReadOnly = false;
}

bool SQLite3::isReadOnly() const noexcept
{
    return pImpl->mReadOnly;
}

/// Connected?
bool SQLite3::isConnected() const noexcept
{
    return pImpl->mSession.is_connected();
}
/// Get session
soci::session *SQLite3::getSession()
{
    if (!isConnected()){throw std::runtime_error("Not connected");}
    return &pImpl->mSession;
}

/// Load the connection information from an initialization file
void SQLite3::parseInitializationFile(const std::string &fileName,
                                      const std::string &section)
{
    if (!std::filesystem::exists(fileName))
    {
        throw std::runtime_error("Initialization file: " + fileName
                               + " does not exist");
    }
    boost::property_tree::ptree propertyTree;
    boost::property_tree::ini_parser::read_ini(fileName, propertyTree);
    auto sqlite3FileName
        = propertyTree.get<std::string> (section + ".fileName", "");
    auto lReadOnly
        = propertyTree.get<bool> (section + ".readonly", isReadOnly());
    // Set information
    setFileName(sqlite3FileName);
    if (lReadOnly)
    {
        setReadOnly();
    }
    else
    {
        setReadWrite();
    }
   
}

