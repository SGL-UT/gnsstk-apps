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
set( NAVSAT_TOOLS_VERSION_MINOR "3" )
set( NAVSAT_TOOLS_VERSION_PATCH "0" )
set( NAVSAT_TOOLS_VERSION "${NAVSAT_TOOLS_VERSION_MAJOR}.${NAVSAT_TOOLS_VERSION_MINOR}.${NAVSAT_TOOLS_VERSION_PATCH}" )
set( CPACK_PACKAGE_VERSION_MAJOR ${NAVSAT_TOOLS_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${NAVSAT_TOOLS_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${NAVSAT_TOOLS_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION ${NAVSAT_TOOLS_VERSION} )


option( BUILD_SHARED_LIBS "Build using shared libraries" OFF )
option( DEBUG_SWITCH "HELP: DEBUG_SWITCH: Default = OFF, print some CMake variable values to stdout." OFF )
option( DEBUG_VERBOSE "HELP: DEBUG_VERBOSE: Default = OFF, print all CMake variable values." OFF )
option( BUILD_EXT "HELP: BUILD_EXT: SWITCH, Default = OFF, Build the ext library, in addition to the core library." OFF )
option( TEST_SWITCH "HELP: TEST_SWITCH: SWITCH, Default = OFF, Turn on test mode." OFF )
option( COVERAGE_SWITCH "HELP: COVERAGE_SWITCH: SWITCH, Default = OFF, Turn on coverage instrumentation." OFF )
option( USE_RPATH "HELP: USE_RPATH: SWITCH, Default= ON, Set RPATH in libraries and binaries." ON )
option( VERSIONED_HEADER_INSTALL "HELP: VERSIONED_HEADER_INSTALL: SWITCH, Default= OFF, Install header files into maj/min versioned directory." OFF )

# This sets up variables contining GNU standard installation locations.
include( GNUInstallDirs )

# always link gnsstk
if ( ${PROFILER} )
  list( APPEND SUPPORTLIBS gnsstkStatic )
else()
  list( APPEND SUPPORTLIBS gnsstk )
endif()

# do all the needed linking in a way that works for all supported platforms
function(linkum target_name)
  if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    string(JOIN " /WHOLEARCHIVE:" WINLIBFLAGS ${SUPPORTLIBS})
    SET_TARGET_PROPERTIES(${target_name} PROPERTIES LINK_FLAGS_DEBUG "/WHOLEARCHIVE:${WINLIBFLAGS}")
    SET_TARGET_PROPERTIES(${target_name} PROPERTIES LINK_FLAGS_RELEASE "/WHOLEARCHIVE:${WINLIBFLAGS}")
  endif()
  target_link_libraries(${target_name} ${SUPPORTLIBS} ${ARGN})
endfunction()

# Include any optional desired linkage
include( ExtLinkage.cmake OPTIONAL )

# Remove gnsstk linkage from Windows builds if SUPPORTLIBS contains
# anything else.  This is because SUPPORTLIBS is used to define
# extensions to NewNav, and Windows links libraries in weird ways that
# cause symbols to be multiply defined, and the extensions are
# presumed to link to gnsstk, which defines the base classes for
# NewNav.
list(LENGTH SUPPORTLIBS SLL)
if (BUILD_SHARED_LIBS AND WIN32 AND (${SLL} GREATER 1))
   message(STATUS "Removing gnsstk from SUPPORTLIBS to avoid symbol duplication")
   list(REMOVE_ITEM SUPPORTLIBS gnsstk)
   list(REMOVE_ITEM SUPPORTLIBS gnsstkStatic)
endif()

# Add each include path from SUPPORTINCPATH to the include directories
foreach (incpath ${SUPPORTINCPATH})
   include_directories(${incpath})
endforeach()

# Set a filename for collecting exported targets.
set( EXPORT_TARGETS_FILENAME "GNSSTKTargets" )

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
include(SupportFuncs.cmake)

#----------------------------------------
# Find needed packages
# Note that since these are brought in here, the sub dirs do not need to
# re-find these or include them.
#----------------------------------------
find_package( GNSSTK REQUIRED CONFIG )

include_directories( ${GNSSTK_INCLUDE_DIRS} )
link_directories( ${GNSSTK_LIBRARY_DIRS} )

if ( WIN32 )
   # @todo use cmake_path once we've dropped support for everything
   # below cmake 3.20
   string(REPLACE "/" "\\" WINDOWS_GNSSTK_LIBRARY_DIRS "${GNSSTK_LIBRARY_DIRS}")
   set(EXTPATH "${WINDOWS_GNSSTK_LIBRARY_DIRS}")
   # Put EXTPATH first so that our built libraries take precedence
   # over anything installed.
   set(TESTPATH "${EXTPATH};$ENV{PATH}")
   # Windows uses ; to separate paths, cmake uses it to separate list
   # items, so we have to do this replacement to make things behave as
   # they should.
   string(REPLACE ";" "\\;" TESTPATH "${TESTPATH}")
else ( WIN32 )
   # This disables the Windows-specific export things for non-Windows platforms.
   add_definitions(-DGNSSTK_STATIC_DEFINE)
endif ( WIN32 )

GenerateNewNavIncludes(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

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
    COMPATIBILITY SameMajorVersion )
install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTK-AppsConfigVersion.cmake
    DESTINATION
    ${INSTALL_CONFIG_DIR} )
