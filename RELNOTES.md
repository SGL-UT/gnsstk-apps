GNSSTk-APPs 13.6.0 Release Notes
========================

 * This release includes the following:
    * Adding navdump enhancements in support of GLONASS work.
    * Adding Merge UsingWhereSat.md into the WhereSat.cpp doxygen documentation.
    * Splitting test data into a separate repo with submodule configuration.
 * Additionally, it contains build tests and CI updates.

Updates since v13.5.0
---------------------

**Build System and Test Suite**
  * Fix ddbase tests that were writing to the same output file
  * Update Changed the minimum cmake version

**Gitlab CI**
  * Fix pipeline push artifacts

**Library Changes**
  * Add navdump support for internal time system conversion
  * Add navdump support for more precise time stamps
  * Add navdump detail to xvt output
  * Add navdump option to request XVTs using a text file
  * Add navdump option for printing satellite positions in geodetic coordinates
  * Add Merge UsingWhereSat.md into the WhereSat.cpp doxygen documentation.
  * Update Split test data into a separate repo with submodule configuration
  * Update exception specifications from throw() to noexcept
