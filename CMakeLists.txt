cmake_minimum_required(VERSION 3.16)
project(QPhase VERSION 0.1.0 LANGUAGES CXX)
enable_testing()

include(CheckCXXCompilerFlag)

set(ORGANIZATION "UUSS" CACHE STRING "Name of your organization.")
set(HOME_PATH $ENV{HOME})
set(DATA_PATH   ${HOME_PATH}/.local/share/${ORGANIZATION}/qphase CACHE STRING "Default data path.")
set(CACHE_PATH  ${HOME_PATH}/.cache/${ORGANIZATION}/qphase CACHE STRING "Default cache path.")
set(CONFIG_PATH ${HOME_PATH}/.config/${ORGANIZATION} CACHE STRING "Default config path.")

option(USE_MAPBOX "Enable MapBox mapping for maps" OFF)
set(MAPBOX_TOKEN "" CACHE STRING "MapBox token for MapBox maps")

configure_file(${CMAKE_SOURCE_DIR}/include/private/version.hpp.in
               ${CMAKE_SOURCE_DIR}/include/qphase/version.hpp)
configure_file(${CMAKE_SOURCE_DIR}/include/private/paths.hpp.in
               ${CMAKE_SOURCE_DIR}/include/private/paths.hpp)
configure_file(${CMAKE_SOURCE_DIR}/include/private/organization.hpp.in
               ${CMAKE_SOURCE_DIR}/include/private/organization.hpp)

# Qt
find_package(Qt6 COMPONENTS Core Widgets Quick Network REQUIRED)
qt6_standard_project_setup()
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON) 
# Other packages
find_package(SOCI REQUIRED)
find_package(Boost COMPONENTS program_options date_time REQUIRED)
find_package(GTest REQUIRED)
find_package(CURL)

set(FindQGeoView_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(FindSFF_DIR ${CMAKE_SOURCE_DIR}/cmake)
find_package(FindQGeoView)
find_package(FindSFF)

include_directories(
    ${CMAKE_SOURCE_DIR}/include)
set(PUBLIC_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qphase)
set(PRIVATE_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private)

if (${FindQGeoView_FOUND})
   set(QPHASE_HAVE_QGVIEW 1)
   if (${USE_MAPBOX})
      set(QPHASE_HAVE_MAPBOX 1)
   else()
      set(QPHASE_HAVE_MAPBOX 0)
   endif()
else()
   set(QPHASE_HAVE_QGVIEW 0)
   set(QPHASE_HAVE_MAPBOX 0)
endif()
configure_file(${CMAKE_SOURCE_DIR}/include/private/haveMap.hpp.in
               ${CMAKE_SOURCE_DIR}/include/private/haveMap.hpp)

##########################################################################################
#                                        Libraries                                       #
##########################################################################################
set(CORE_SRC
    src/observerPattern/subject.cpp
    #src/waveforms/multiChannelStation.cpp
    src/waveforms/channel.cpp
    src/waveforms/segment.cpp
    src/waveforms/simpleResponse.cpp
    src/waveforms/singleChannelSensor.cpp
    src/waveforms/singleChannelVerticalSensor.cpp
    src/waveforms/station.cpp
    src/waveforms/threeChannelSensor.cpp
    src/waveforms/waveform.cpp
    #src/waveforms/sac.cpp
    )
set(DB_SRC
    src/database/connection/sqlite3.cpp
    src/database/internal/arrival.cpp
    src/database/internal/arrivalTable.cpp
    src/database/internal/event.cpp
    src/database/internal/eventTable.cpp
    src/database/internal/magnitude.cpp
    src/database/internal/origin.cpp
    src/database/internal/stationData.cpp
    src/database/internal/stationDataTable.cpp
    src/database/internal/waveform.cpp
    src/database/internal/waveformTable.cpp)
set(WS_SRC
    src/webServices/comcat/event.cpp)
if (${CURL_FOUND})

endif()

set(UI_SRC
    src/widgets/tableViews/eventTableModel.cpp
    src/widgets/tableViews/eventTableView.cpp
    src/widgets/waveforms/channelItem.cpp
    src/widgets/waveforms/stationItem.cpp
    src/widgets/waveforms/stationScene.cpp
    src/widgets/waveforms/stationView.cpp
    include/qphase/widgets/waveforms/stationView.hpp
    #src/widgets/waveforms/postProcessing/traceItem.cpp
    #src/widgets/waveforms/postProcessing/traceScene.cpp
    #include/qphase/widgets/waveforms/postProcessing/traceView.hpp
    #src/widgets/waveforms/postProcessing/traceView.cpp
    )

if (${FindQGeoView_FOUND})
    set(MAP_SRC
        src/widgets/map/background.cpp
        src/widgets/map/boundary.cpp 
        src/widgets/map/event.cpp
        src/widgets/map/map.cpp
        include/qphase/widgets/map/mainWindow.hpp
        src/widgets/map/mainWindow.cpp
        src/widgets/map/options.cpp
        src/widgets/map/station.cpp)
    if (${USE_MAPBOX})
        set(MAP_SRC ${MAP_SRC} src/widgets/map/QGVMapBox.cpp
                               include/qphase/widgets/map/QGVMapBox.hpp)
    endif()
endif()
#qt6_add_qml_module(qphaseMap
#                   URI qphaseMaps
#                   QML_FILES src/widgets/map/map.qml
#                   #RESOURCES
#                   #    img/world.png
#                   )

set(BUILD_SHARED_LIBS YES)
add_library(qphase_core ${CORE_SRC} ${DB_SRC} ${WS_SRC} src/version.cpp)
set_target_properties(qphase_core PROPERTIES
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(qphase_core
#                      PUBLIC #${TIME_LIBRARY}
                      PRIVATE Boost::date_time SOCI::soci_core SOCI::soci_sqlite3
                      PRIVATE ${SFF_LIBRARY}
                      PRIVATE ${CURL_LIBRARIES})# SOCI::soci_postgresql)
target_include_directories(qphase_core
                           PRIVATE SOCI::soci_sqlite3 ${CURL_INCLUDE_DIRS} ${SFF_INCLUDE_DIR}
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                           PUBLIC  $<INSTALL_INTERFACE:include>)


add_library(qphase_widgets ${UI_SRC} ${MAP_SRC})
set_target_properties(qphase_widgets PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(qphase_widgets
                      PUBLIC qphase_core
                      PRIVATE ${QGEOVIEW_LIBRARY} Qt6::Core Qt6::Widgets 
                                                  Qt6::Quick Qt6::Network)
target_include_directories(qphase_widgets
                           PRIVATE ${Qt6Widgets_INCLUDE_DIRS} ${QGEOVIEW_INCLUDE_DIR}
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                           PUBLIC  $<INSTALL_INTERFACE:include>)

##########################################################################################
#                                      Applications                                      #
##########################################################################################
qt6_add_resources(QNODE_RESOURCES
                 ${CMAKE_SOURCE_DIR}/resources/qnode.qrc)
add_executable(qnode
               app/qnode/load.cpp
               app/qnode/qnode.cpp
               app/qnode/mainWindow.cpp ${QNODE_RESOURCES})
set_target_properties(qnode PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_include_directories(qnode PRIVATE ${SFF_INCLUDE_DIR} PUBLIC ${QGEOVIEW_INCLUDE_DIR})
target_link_libraries(qnode
                      PRIVATE qphase_core qphase_widgets Qt6::Core Qt6::Widgets Qt6::Network ${SFF_LIBRARY} SOCI::soci_core)
##########################################################################################
#                                       Unit Tests                                       #
##########################################################################################
set(TEST_SRC
    testing/main.cpp
    testing/database/internal.cpp
    testing/waveforms/waveform.cpp
    testing/webServices/comcat.cpp)
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
# Add the tests
add_test(NAME unitTests
         COMMAND unitTests)
##########################################################################################
#                                      Installation                                      #
##########################################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/QPhaseConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS qphase_core qphase_widgets
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT Runtime)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/qphase
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

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
