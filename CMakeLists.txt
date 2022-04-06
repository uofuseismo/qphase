cmake_minimum_required(VERSION 3.16)
project(QPhase VERSION 0.1.0 LANGUAGES CXX)
enable_testing()

include(CheckCXXCompilerFlag)

set(ORGANIZATION "UUSS" CACHE STRING "Name of your organization.")
set(HOME_PATH $ENV{HOME})
set(DATA_PATH   ${HOME_PATH}/.local/share/${ORGANIZATION}/qphase CACHE STRING "Default data path.")
set(CACHE_PATH  ${HOME_PATH}/.cache/${ORGANIZATION}/qphase CACHE STRING "Default cache path.")
set(CONFIG_PATH ${HOME_PATH}/.config/${ORGANIZATION} CACHE STRING "Deafult config path.")

configure_file(${CMAKE_SOURCE_DIR}/include/private/version.hpp.in
               ${CMAKE_SOURCE_DIR}/include/qphase/version.hpp)
configure_file(${CMAKE_SOURCE_DIR}/include/private/paths.hpp.in
               ${CMAKE_SOURCE_DIR}/include/private/paths.hpp)
configure_file(${CMAKE_SOURCE_DIR}/include/private/organization.hpp.in
               ${CMAKE_SOURCE_DIR}/include/private/organization.hpp)

# Qt
find_package(Qt6 COMPONENTS Core Widgets Network REQUIRED)
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON) 
# Other packages
find_package(SOCI REQUIRED)
find_package(Boost COMPONENTS program_options date_time REQUIRED)
find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include)
set(PUBLIC_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/urts)
set(PRIVATE_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private)


##########################################################################################
#                                        Libraries                                       #
##########################################################################################
set(DB_SRC
    src/database/internal/arrival.cpp
    src/database/internal/origin.cpp)

set(BUILD_SHARED_LIBS YES)
add_library(qphase_core ${DB_SRC} src/version.cpp)
set_target_properties(qphase_core PROPERTIES
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(qphase_core
#                      PUBLIC #${TIME_LIBRARY}
                      PRIVATE Boost::date_time SOCI::soci_sqlite3)# SOCI::soci_postgresql)
target_include_directories(qphase_core
                           PRIVATE SOCI::soci_sqlite3
                           PUBLIC $<BUILD_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>
                           PUBLIC $<INSTALL_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>)


##########################################################################################
#                                      Applications                                      #
##########################################################################################
qt6_add_resources(QNODE_RESOURCES
                 ${CMAKE_SOURCE_DIR}/resources/qnode.qrc)
add_executable(qnode
               app/qnode/qnode.cpp
               app/qnode/mainWindow.cpp ${QNODE_RESOURCES})
set_target_properties(qnode PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(qnode
                      PRIVATE qphase_core Qt6::Core Qt6::Widgets Qt6::Network)
##########################################################################################
#                                       Unit Tests                                       #
##########################################################################################
set(TEST_SRC
    testing/main.cpp
    testing/database/internal.cpp)
add_executable(unitTests ${TEST_SRC})
set_target_properties(unitTests PROPERTIES
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(unitTests
                      PRIVATE qphase_core ${GTEST_BOTH_LIBRARIES})
target_include_directories(unitTests
                           PRIVATE ${GTEST_INCLUDE_DIRS}
                                   ${PRIVATE_HEADER_DIRECTORIES}
                           PUBLIC $<BUILD_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>)

##########################################################################################
#                                      Installation                                      #
##########################################################################################
include(GNUInstallDirs)
install(TARGETS qnode qphase_core
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/qphase
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(DIRECTORY ${PUBLIC_HEADER_DIRECTORIES}/qphase
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

##########################################################################################
#                                     CPack Packaging                                    #
##########################################################################################
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "UUSS")
set(CPACK_PACKAGE_CONTACT "ben.baker@utah.edu")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A passive-source seismology user interface.")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_COMPONENTS_ALL libraries headers)
if (WIN32)
    set(CPACK_GENERATOR ZIP WIX)
elseif (APPLE)
    set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ RPM)
else()
    set(CPACK_GENERATOR TGZ)
endif()
set(CPACK_SOURCE_IGNORE_FILES
  /\\.git/
  \\.swp
  \\.orig
  /CMakeLists\\.txt\\.user
  /private/
)
include(CPack) # Put this last!
