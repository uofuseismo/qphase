# Already in cache, be silent
if (SFF_INCLUDE_DIR AND SFF_LIBRARY)
   set (SFF_FIND_QUIETLY TRUE)
endif()

find_path(SFF_INCLUDE_DIR
          NAMES sff
          HINTS $ENV{SFF_ROOT}/include
                $ENV{SFF_INCLUDE_DIR}
                /usr/local/include
                /usr/include)
find_library(SFF_LIBRARY
             NAMES sff
             PATHS $ENV{SFF_ROOT}/lib
                   $ENV{SFF_ROOT}/lib64
                   /usr/local/lib
                   /usr/local/lib64)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FindSFF DEFAULT_MSG SFF_INCLUDE_DIR SFF_LIBRARY)
mark_as_advanced(SFF_INCLUDE_DIR SFF_LIBRARY)
