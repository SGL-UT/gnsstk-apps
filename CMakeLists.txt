#============================================================
# Name    = $NAVSAT_TOOLS/CMakeLists.txt
# Purpose = Generator of build framework (e.g. Makefiles) for Navsat-Tools
# Notes   = This is the top-level CMake input file
#           Depends on $NAVSAT_TOOLS/BuildSetup.cmake
#           Is dependend on by $NAVSAT_TOOLS/build.sh
#============================================================

cmake_minimum_required( VERSION 2.8.4 )
cmake_policy( VERSION 2.8.4 )

project( NAVSAT_TOOLS )
set( NAVSAT_TOOLS_VERSION_MAJOR "13" )
set( NAVSAT_TOOLS_VERSION_MINOR "0" )
set( NAVSAT_TOOLS_VERSION_PATCH "0" )
set( NAVSAT_TOOLS_VERSION "${NAVSAT_TOOLS_VERSION_MAJOR}.${NAVSAT_TOOLS_VERSION_MINOR}.${NAVSAT_TOOLS_VERSION_PATCH}" )
set( CPACK_PACKAGE_VERSION_MAJOR ${NAVSAT_TOOLS_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${NAVSAT_TOOLS_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${NAVSAT_TOOLS_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION ${NAVSAT_TOOLS_VERSION} )

# This sets up variables contining GNU standard installation locations.
include( GNUInstallDirs )

# always link gnsstk
if ( ${PROFILER} )
  list( APPEND SUPPORTLIBS gnsstkStatic )
else()
  list( APPEND SUPPORTLIBS gnsstk )
endif()

# Include any optional desired linkage
include( ExtLinkage.cmake OPTIONAL )

# Set a filename for collecting exported targets.
set( EXPORT_TARGETS_FILENAME "GNSSTKTargets" )


option( DEBUG_SWITCH "HELP: DEBUG_SWITCH: Default = OFF, print some CMake variable values to stdout." OFF )
option( DEBUG_VERBOSE "HELP: DEBUG_VERBOSE: Default = OFF, print all CMake variable values." OFF )
option( BUILD_EXT "HELP: BUILD_EXT: SWITCH, Default = OFF, Build the ext library, in addition to the core library." OFF )
option( TEST_SWITCH "HELP: TEST_SWITCH: SWITCH, Default = OFF, Turn on test mode." OFF )
option( COVERAGE_SWITCH "HELP: COVERAGE_SWITCH: SWITCH, Default = OFF, Turn on coverage instrumentation." OFF )
option( USE_RPATH "HELP: USE_RPATH: SWITCH, Default= ON, Set RPATH in libraries and binaries." ON )
option( VERSIONED_HEADER_INSTALL "HELP: VERSIONED_HEADER_INSTALL: SWITCH, Default= OFF, Install header files into maj/min versioned directory." OFF )

# GNSSTk header file install target (whether it is version dependent changes based on user flag)
if( VERSIONED_HEADER_INSTALL )
    set( GNSSTK_INCLUDE_WRAPPER_DIR "gnsstk_${GNSSTK_VERSION_MAJOR}_${GNSSTK_VERSION_MINOR}" )
    set( GNSSTK_INCLUDE_TGT "${GNSSTK_INCLUDE_WRAPPER_DIR}/gnsstk" )
    set( GNSSTK_INCLUDE_PARENT "${CMAKE_INSTALL_INCLUDEDIR}/${GNSSTK_INCLUDE_WRAPPER_DIR}" )
    set( CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${GNSSTK_INCLUDE_TGT}" )
    message( WARNING "Setting GNSSTK_INCLUDE_TGT_DIR = ${CMAKE_INSTALL_INCLUDEDIR}" )
else()
    set( GNSSTK_INCLUDE_PARENT "${CMAKE_INSTALL_INCLUDEDIR}" )
    set( CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/gnsstk" )
endif()



include( BuildSetup.cmake )

#----------------------------------------
# Find needed packages
# Note that since these are brought in here, the sub dirs do not need to
# re-find these or include them.
#----------------------------------------
find_package( GNSSTK REQUIRED CONFIG )

include_directories( ${GNSSTK_INCLUDE_DIRS} )
link_directories( ${GNSSTK_LIBRARY_DIRS} )


#============================================================
# Testing
#============================================================

if( TEST_SWITCH )
  enable_testing()

  # Test data file source directory
  set( GNSSTK_APPS_TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/data )
  # Test output directory
  set( GNSSTK_APPS_TEST_OUTPUT_DIR ${PROJECT_BINARY_DIR}/Testing/Temporary )
  # Some cmake scripts used with testing
  set(df_diff ${GNSSTK_BINDIR}/df_diff)
  set(difftest ${PROJECT_SOURCE_DIR}/tests/difftest.cmake)
  set(testsuccexp ${PROJECT_SOURCE_DIR}/tests/testsuccexp.cmake)
  set(testhelp ${PROJECT_SOURCE_DIR}/tests/testhelp.cmake)
endif( )

#============================================================
# Coverage
#============================================================

if( COVERAGE_SWITCH )
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"
    OR ((${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.9.0" ) AND CMAKE_COMPILER_IS_GNUCXX))
    message(STATUS "Enabling address sanitizer for debug build")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage" )
  endif()
endif( )


#----------------------------------------
# Add sub-directories
#----------------------------------------

add_subdirectory( core )

if( BUILD_EXT )
   add_subdirectory( ext )
endif()

#----------------------------------------
# Export the project import cmake files.
#----------------------------------------
include( CMakePackageConfigHelpers )
set( INSTALL_CONFIG_DIR ${CMAKE_INSTALL_DATADIR}/cmake/GNSSTK-Apps )
configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/GNSSTK-AppsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIG_DIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_BINDIR CMAKE_INSTALL_PREFIX INSTALL_CONFIG_DIR )
write_basic_package_version_file( ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfigVersion.cmake
    VERSION ${GNSSTK_VERSION}
    COMPATIBILITY ExactVersion )
install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfigVersion.cmake
    DESTINATION
    ${INSTALL_CONFIG_DIR} )
