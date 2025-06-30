GNSSTk-APPs 15.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit.
 * It includes the following:
   * Adding std C++17 support while deprecating std C++11 support. (**api breaking**)
   * Fixing wheresat application such that a usable default TimeSystem is used for the optional start and end times
* Additionally, it contains build and test CI/CD updates.

Updates since v14.1.4
---------------------

**Known Issues**
  * Some distributions (such as RHEL 8) may support different versions of gcc (such as gcc 8.x and 9.x but gcc 8.x is the default). A version of gcc (such as gcc 9 on RHEL 8) that supports C++17 and the C++17 ABI is stable must be chosen to build. Note that gcc 8.x on RHEL 8 may support some C++17 but the C++17 ABI is not stable and thus should not be used.
  * Support for MSVC14 (Microsoft Visual Studio 14) and older compilers is removed. Only Microsoft Visual Studio 19 and newer versions are supported.

**Build System and Test Suite**
  * Update cmake to set minimum compiler version on RHEL to be C++17 ABI compatible
  * Fix tests using df_diff from df_diff fixes in gnsstk!969

**Gitlab CI**
  * Update CI pipeline to use RKS Runner Tags
  * Update submodule ref to point to latest gnsstk-data merge
  * Add Fortify scan job
  * Add pipeline jobs for check LTS breaking changes
  * Fix fortify pipeline job to default to full scans

**Library Changes**
  * Add C++17 support for Toolkits

Fixes since v14.1.4
--------------------
  * Fix wheresat application such that a usable default TimeSystem is used for the optional start and end times; alternatively, allow a TimeSystem to be specified on the command-line.

Removed Code due to Deprecation
-------------------------------
    core/tests/df_diff.cpp
    
    No longer build and publish packages with std C++11