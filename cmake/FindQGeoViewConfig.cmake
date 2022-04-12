# Already in cache, be silent
if (QGEOVIEW_INCLUDE_DIR AND QGEOVIEW_LIBRARY)
   set (QGEOVIEW_FIND_QUIETLY TRUE)
endif()

find_path(QGEOVIEW_INCLUDE_DIR
          NAMES QGeoView
          HINTS $ENV{QGEOVIEW_INCLUDE_DIR}/include
                /usr/local/include
                /usr/include)
find_library(QGEOVIEW_LIBRARY
             NAMES qgeoview
             PATHS $ENV{QGEOVIEW_ROOT}/lib
                   $ENV{QGEOVIEW_ROOT}/lib64
                   /usr/local/lib
                   /usr/local/lib64)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FindQGeoView DEFAULT_MSG QGEOVIEW_INCLUDE_DIR QGEOVIEW_LIBRARY)
mark_as_advanced(QGEOVIEW_INCLUDE_DIR QGEOVIEW_LIBRARY)
