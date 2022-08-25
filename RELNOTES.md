GNSSTk-APPs 13.5.0 Release Notes
========================

 * This release includes the following:
    * Updating `WhereSat` to add almanac support.
    * Updating `sats.csv` to match finalized format
 * Additionally, it contains build tests and CI updates.

Updates since v13.4.2
---------------------

**Build System and Test Suite**
  * Fix rineditnav tests so they don't clobber each other's output for random failures. 

**Gitlab CI**
  * Add sonar scan job to pipeline
  * Update replaced sgl_ci docker images with sgl equivalent

**Library Changes**
  * Update WhereSat to add almanac support
  * Update sats.csv to match finalized format