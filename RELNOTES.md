GNSSTk-APPs 14.1.0 Release Notes
========================

 * This release includes the following:
   * Fixing WhereSat.cpp to no longer filter out almanac subframes.
   * Fixing expected time offset output in gnsstk data submodule repo.
   * Co-installation of major versions of debian packages.

Updates since v14.0.0
---------------------

**Build System and Test Suite**
  * Update create debian/ubuntu major version coinstallable package 

**Gitlab CI**
  * Fix deb11 optional pipeline jobs

Fixes since v14.0.0
--------------------
  * Fix expected time offset output.
  * Fix WhereSat.cpp to no longer filter out almanac subframes when loading data allowing the use-alm flag to function properly