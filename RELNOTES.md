GNSSTk-APPs 14.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit apps.
 * It includes the following:
   * Updating cmake package from using a hyphen to using an underscore.
   * Adding PRN/SVN mapping support to navdump.
   * Adding Ubuntu20.04 (focal) support.

Updates since v13.6.0
---------------------

**Build System and Test Suite**
  * Updated the clean build parameter

**Gitlab CI**
  * Add ubuntu 20.04 pipeline jobs

**Library Changes**
  * Update replaced hypen with underscore from cmake package name
  * Add PRN/SVN mapping support to navdump