GNSSTk-APPs 13.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit apps.
 * It includes the following:
   * Refactoring due to deprecation of old C++ Nav classes in gnsstk (see deprecation list below)
     * PRSolve and DDBase to use gnsstk NewNav
     * Deprecate RinNav in favor of RinEditNav
     * Refactor uses of gnsstk/SunEarthSatGeometry in gnsstk-apps
   * Addition of new NavDump tool and RinEditNav class (see new modules list below)
 * Additionally, it contains minor library updates, bug fixes, and CI updates

Updates since v12.0.1
---------------------

**Build System and Test Suite**
  * Store CI artifacts in artifactory.
  * Add export of config cmake files
  * Update copyright header to year 2022

**Gitlab CI**
  * Update ci-pipelines to expect python3 bindings, instead of python2.
  * Update Remove debian 8 jobs
  * Add windows packaging job to pipeline

**Library Changes**
  * Add new navdump tool
  * Refactor PRSolve to use gnsstk NewNav
  * Add integrity status flag to GPS LNAV data structures
  * Update PRSolve and dfix truth data to account for small changes in WAVELENGTH constants
  * Update ddbase to use gnsstk NewNav and NOT use PRSolutionLegacy
  * Deprecate RinNav in favor of rineditnav
  * Refactor uses of gnsstk/SunEarthSatGeometry in gnsstk-apps

Fixes since v12.0.1
--------------------
  * Fix timeconvert tests truth data for time offset error

Removed Code due to Deprecation
-------------------------------
     core/apps/Rinextools/scanBrdcFile.cpp
     ext/apps/Rinextools/RinNav.cpp
     ext/apps/geomatics/relposition/EphemerisImprovement.cpp

New Modules
-------------------------------
     core/apps/Rinextools/RinEditNav.cpp
     core/apps/filetools/navdump.cpp