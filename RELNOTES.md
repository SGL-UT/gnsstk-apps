GNSSTk-APPs 13.4.0 Release Notes
========================

 * This release includes the following:
   * Adding `rinexelvstrip`, `wheresat` and `findMoreThan12` tools
   * Removing `examples` code that already exists in gnsstk library
   * Adding support for deploying to Debian 11.
 * Additionally, it contains bug fixes and CI updates.


Updates since v13.3.0
---------------------

**Build System and Test Suite**
  * Add support for deploying to Debian 11

**Gitlab CI**
  * Fix CI Build Job Requiring LIBHDF5.
  * Update conda recipe and CI pipeline to release from stable branch

**Library Changes**
  * Add output to stdout including filenames, number of NavMsg, and time span for RinEditNav
  * Add rinexelvstrip, wheresat and findMoreThan12 tools
  * Update timeconvert and navdump to indicate missing/bad file/data errors
  * Delete example code which already exists in gnsstk library

Fixes since v13.3.0
--------------------
  * Fix rineditnav so it doesn't ignore files with the same start time

Removed Code due to Deprecation
-------------------------------
     examples/CommandOption1.cpp
     examples/CommandOption2.cpp
     examples/CommandOption3.cpp
     examples/CommandOption4.cpp
     examples/CommandOption5.cpp
     examples/example1.cpp
     examples/example12.cpp
     examples/example15.cpp
     examples/example2.cpp
     examples/example3.cpp
     examples/example4.cpp
     examples/navfilterex-pseudo-container.cpp
     examples/navfilterex-pseudo-inherit.cpp
     examples/navfilterex-pseudo.cpp
     examples/navfilterex.cpp

New Modules
-------------------------------
     core/apps/Rinextools/RinexElvStrip.cpp
     core/apps/visibility/WhereSat.cpp
     core/apps/visibility/findMoreThan12.cpp