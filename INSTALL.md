# Overview

This provides a quick walk-through on building and installing the software.  There are two main components of QPhase.  First, there are the libraries.  These are decomposed into libraries that use Qt and libraries that do not.  The second component are the binaries.

# Prerequisites

The following prerequisites are required

   1. A C++20 compatible compiler.
   2. [CMake](cmake.org) v.3.16 or greater.
   3. [Qt](https://www.qt.io/) 6.2 or greater. 
   4. [Boost](boost.org) v.1.70 or greater.
   5. [SFF](https://github.com/uofuseismo/sff) for loading seismic files of various formats.
   6. [SOCI](http://soci.sourceforge.net/) with, at least, [SQLite3](https://www.sqlite.org/index.html) support.

The following prerequisites are optional

   1. [QGeoView](https://github.com/AmonRaNet/QGeoView) for mapping.
   2. A [MapBox](https://www.mapbox.com/) API token.

# Configuring

The simplest strategy to work with CMake is in your root directory create a script.  For example,

    #!/bin/bash
    export CXX=g++
    export ORGANIZATION=UUSS
    export QPHASE_CACHE_DIR=${HOME}/.cache/${ORGANIZATION}/qphase
    export QPHASE_DATA_DIR=${HOME}/.local/share/${ORGANIZATION}/qphase
    export QPHASE_CONFIG_DIR=${HOME}/.config/${ORGANIZATION}
    export CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${HOME}/Qt/6.2.2/gcc_64/lib/cmake
    export BUILD_DIR=clang_build
    if [ -d ${BUILD_DIR} ]; then
       rm -rf ${BUILD_DIR}
    fi
    mkdir ${BUILD_DIR}
    cd ${BUILD_DIR}
    cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_CXX_FLAGS="-Wall -O2" \
    -DSFF_INCLUDE_DIR=${HOME}/Codes/sff/include \
    -DSFF_LIBRARY=${HOME}/Codes/sff/clang_build/libsff.so \
    -DUSE_MAPBOX=ON \
    -DMAPBOX_TOKEN=${QPHASE_MAPBOX_TOKEN}

If SFF is installed in a sensible spot, say /usr/local/include and /usr/local/lib, then it is unnecessary to specify the path to the library.  Here, the MapBox token is an environment variable that I set in my bashrc.

# Building

After successfully compiling just type make.  

    make

# Installing

To install the libraries, includes, and binaries use

    make install

This may require sudo.  Note, the binaries will, by default, be install in /usr/local/bin/qphase/executable so you may want to add that to your path.


