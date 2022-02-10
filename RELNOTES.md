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
 * Please see Known Issues list below regarding latest update

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

Known Issues
--------------------
1.  The following unit tests are failing in windows environment only and have been tracked for further investigation.
```
      116 - RinEditNav_merge_1 (Failed)
      117 - RinEditNav_merge_2 (Failed)
      118 - RinEditNav_merge_3 (Failed)
      119 - RinEditNav_merge_4 (Failed)
      120 - RinEditNav_outputs_1 (Failed)
      121 - RinEditNav_exclude_1 (Failed)
      122 - RinEditNav_exclude_2 (Failed)
      123 - RinEditNav_time_1 (Failed)
      124 - RinEditNav_time_2 (Failed)
      125 - RinEditNav_time_3 (Failed)
```