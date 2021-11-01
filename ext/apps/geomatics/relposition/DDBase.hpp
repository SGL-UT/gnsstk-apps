//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

/**
 * @file DDBase.hpp
 * Main include file for DDBase - the ARL:UT network precise position estimation
 * using double differenced GPS carrier phase.
 */

//------------------------------------------------------------------------------------
#ifndef CLASS_DDBASE_PROCESSOR_INCLUDE
#define CLASS_DDBASE_PROCESSOR_INCLUDE

//------------------------------------------------------------------------------------
// includes
// system
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

// GNSSTk
//#define RANGECHECK // if defined, Vector and Matrix will throw on invalid index.
#include <gnsstk/Exception.hpp>
#include <gnsstk/StringUtils.hpp>
#include <gnsstk/CommonTime.hpp>
#include <gnsstk/TropModel.hpp>
#include <gnsstk/SP3EphemerisStore.hpp>
#include <gnsstk/GPSEphemerisStore.hpp>
#include <gnsstk/EarthOrientation.hpp>
#include <gnsstk/EOPStore.hpp>
#include <gnsstk/Position.hpp>
#include <gnsstk/RinexObsBase.hpp>
#include <gnsstk/RinexObsData.hpp>
#include <gnsstk/RinexObsHeader.hpp>
#include <gnsstk/RinexObsStream.hpp>
#include <gnsstk/Epoch.hpp>
#include <gnsstk/Matrix.hpp>

// DDBase
#include <gnsstk/GSatID.hpp>             // this one first
#include "DataStructures.hpp"
#include "DDid.hpp"
#include "constants.hpp"
#include <gnsstk/format.hpp>

//------------------------------------------------------------------------------------
extern clock_t totaltime;        // for timing tests

//------------------------------------------------------------------------------------
// prototypes
/**
 * @throw Exception
 */
int Configure(int which);        // Configure.cpp
/**
 * @throw Exception
 */
int ReadAndProcessRawData(void); // ReadRawData.cpp
/**
 * @throw Exception
 */
int EditRawDataBuffers(void);    // EditRawDataBuffers.cpp
/**
 * @throw Exception
 */
int OutputRawDataBuffers(void);  // EditRawDataBuffers.cpp
/**
 * @throw Exception
 */
int EphemerisImprovement(void);  // EphemerisImprovement.cpp
/**
 * @throw Exception
 */
int ClockModel(void);            // ClockModel.cpp
/**
 * @throw Exception
 */
int Synchronization(void);       // Synchronization.cpp
/**
 * @throw Exception
 */
int RecomputeFromEphemeris(void);// Synchronization.cpp
/**
 * @throw Exception
 */
int Timetable(void);             // Timetable.cpp
/**
 * @throw Exception
 */
int QueryTimeTable(gnsstk::SDid& sdid, gnsstk::CommonTime& tt);
/**
 * @throw Exception
 */
int QueryTimeTable(std::string bl, int& beg, int& end);
/**
 * @throw Exception
 */
int DoubleDifference(void);      // DoubleDifference.cpp
/**
 * @throw Exception
 */
int EditDDs(void);               // EditDDs.cpp
/**
 * @throw Exception
 */
int Estimation(void);            // Estimation.cpp
/**
 * @throw Exception
 */
gnsstk::Matrix<double> SingleAxisRotation(double angle, const int axis);
   // DDBase.cpp

//------------------------------------------------------------------------------------
// Global data -- see DDBase.cpp where these are declared and documented
extern std::string Title;
extern std::string PrgmDesc;
extern std::ofstream oflog;
//extern CommandInput CI;   put in CommandInput.hpp
extern std::vector<std::string> Baselines;

extern gnsstk::CommonTime SolutionEpoch;
extern gnsstk::CommonTime FirstEpoch,LastEpoch,MedianEpoch;
extern int Count;
extern int maxCount;
extern double wave;

extern gnsstk::XvtStore<gnsstk::SatID> *pEph;
extern gnsstk::EOPStore EOPList;
extern gnsstk::EarthOrientation eorient;

extern std::map<std::string,Station> Stations;
extern std::vector<ObsFile> ObsFileList;

extern std::map<gnsstk::DDid,DDData> DDDataMap;

#endif

// include the 'command line input configuration' structure
#include "CommandInput.hpp"

// nothing below this
