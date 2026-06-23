GNSSTk-APPs 15.1.1 Release Notes
========================

 * This release removes the non-existent ctest `-v` flag and updates CI as listed below

Updates since v15.1.0
---------------------

**Build System and Test Suite**
  * Fix build.sh to remove non-existent ctest `-v` flag. 

**Gitlab CI**
  * Update gnsstk-data submodule ref in gitlab pipeline
  * Fix conda pkg pipeline job by pinning version of conda glibc to 2.28
  * Use reduced pkgbuild image in gitlab pipeline
