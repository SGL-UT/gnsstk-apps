\page INSTALL INSTALL

INSTALL
-------

Contents:
---------

* Introduction
* Dependencies
* UNIX-like Environment: Automated Build and Install
* UNIX-like Environment: Manual Build and Install
* Windows and Microsoft Visual Studio: Building with CMake
* Linking Against the GNSSTk Library

Introduction:
-------------

The most up-to-date project information can be found at the project website,
including build and install instructions, the latest project news, a support
forum, IRC, and contact information:

    http://www.gnsstk.org/

The easiest way to build an install GNSSTk is to use the shell script provided
with the source distribution. See below for more details on use of the script,
details on building on POSIX platforms, and where to get instructions for
building with CMake, both with Xcode on OSX and Visual Studio on Windows.


Dependencies:
-------------

This project uses CMake for all of its configuration, build, install & testing tasks.

    http://www.cmake.org/

This project (optionally) uses doxygen to dynamically generate API documentation.

    http://www.doxygen.org/


UNIX-like Environment: Automatic build and install
-------------------------------------------

This section describes build and installation under the following environments:

   - Linux, OSX, Solaris

The following procedure will build and install the GNSSTk.

   1. Ensure that prerequisites such as cmake and swig have been installed.

   2. Obtain the GNSSTk source distribution.

   3. If needed, extract the GNSSTk compressed archive.
      For example, using GNU tar and gunzip:

         $ tar -xvzf gnsstk.tgz

   4. Navigate to the root directory of the extract GNSSTk file tree, herein
      called $gnsstk_root:

         $ cd $gnsstk_root

    5. Execute the build script to build and install to the user's ~/.local dir

        $ ./build.sh -eu


UNIX-like Environment: Manual Build, Install, and Package:
----------------------------------------------------------

If you instead prefer or need to run commands one at a time, run the following
commands instead of build.sh:

   1. If you want documentation of the C++ classes:

         $ cd $gnsstk_root

         $ doxygen

   2. Create a directory for your build:

         $ cd $gnsstk_root

         $ mkdir $gnsstk_root/build

   3. Change into the $gnsstk_root/build directory, this will be where all of the
      build files will reside.

   4. If you prefer to build and install GNSSTk as a system library in /usr/local,
      execute the following commands:

         $ cd $gnsstk_root/build

         $ cmake $gnsstk_root

         $ make

         $ make install

   5. If you prefer to build and install GNSSTk as a user library in $HOME/.local,
      skip the previous step, and instead execute the following commands:

         $ cd $gnsstk_root/build

         $ cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local/gnsstk -DBUILD_EXT=1 $gnsstk_root

         $ export LD_LIBRARY_PATH=$HOME/.local/gnsstk/lib:$LD_LIBRARY_PATH

         $ make

         $ make install

         $ export PATH=$HOME/.local/gnsstk/bin:$PATH

   6. If you want to package the binaries into a tarball, execute the following commands:

         $ cd $gnsstk_root/build

         $ cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local/gnsstk $gnsstk_root

         $ export LD_LIBRARY_PATH=$HOME/.local/gnsstk/lib:$LD_LIBRARY_PATH

         $ make

         $ make package



Windows and Microsoft Visual Studio: Building with CMake:
---------------------------------------------------------

Step-by-step procedure with pictures on building, installing, and testing the GNSSTk under Visual Studio
2012 can be found at gnsstk.org:

    http://www.gnsstk.org/bin/view/Documentation/BuildingGNSSTkUnderWindows


Linking Against the GNSSTk Library:
----------------------------------

If you are developing against your install of GNSSTk, and have added
the relevant GNSSTk library \#include statements in you implementation file,
then the simplest example of building your application in a Bash script
would look like this:

    INPUT=Your_App.cpp
    OUTPUT=Your_App.exe
    GNSSTK_INSTALL=$HOME/.local/gnsstk
    COMPILER="gcc -Wall"

    COMMAND_STRING="$COMPILER -I$GNSSTK_INSTALL/include -L$GNSSTK_INSTALL/lib -lgnsstk -o $OUTPUT $INPUT"
    echo "$COMMAND_STRING"
    eval $COMMAND_STRING
