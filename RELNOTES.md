GNSSTk-APPs 13.2.0 Release Notes
========================

 * This release adds the following:
   * GNSSTk-APPs compiled with upstream windows GNSSTk DLL package.
   * Navdump support for matching individual messages and getting satellite xvt.
   * Code style guide updates
 * Additionally, it contains a bug fix and removal of duplicate unit tests (see Deprecation list below)

Updates since v13.1.0
---------------------

**Build System and Test Suite**
  * Remove tests that are duplicates from gnsstk (library)

**Library Changes**
  * Add navdump support for matching individual messages and getting satellite xvt.
  * Update apps and tests so windows builds can use dynamic linking
  * Update per style guide

Fixes since v13.1.0
--------------------
  * Fix rowdiff failure when file 2 contained only a header.

Removed Code due to Deprecation
-------------------------------
     ext/tests/geomatics/KalmanFilter_T.cpp
     ext/tests/geomatics/Rinex3ObsLoader_T.cpp
     ext/tests/geomatics/StatsFilter_T.cpp
     ext/tests/geomatics/test_EO_SOFA.cpp
     ext/tests/geomatics/test_tides.cpp