GNSSTk-APPs 13.1.0 Release Notes
========================

 * This release introduces the ability to do windows builds with optional toolkit libraries.
 * It also includes fixes for windows support related issues.

Updates since v13.0.0
---------------------

**Build System and Test Suite**
  * Fix build.sh for ExtLinkage.cmake find_package and removed protected term hit.

**Gitlab CI**
  * Fix Windows CI issues 

**Library Changes**
  * Update rineditnav output arguments to use @ instead of : for Windows compatibility.
  * Update cmake linkage so that optional libs work under windows.