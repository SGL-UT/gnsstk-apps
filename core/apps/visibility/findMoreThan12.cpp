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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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
 * \page findMoreThan12 findMoreThan12 - Look for More than 12 SVs in View
 * \tableofcontents
 *
 * \section findMoreThan12_name NAME
 * findMoreThan12 - Look for More than 12 SVs in View
 *
 * \section findMoreThan12_synopsis SYNOPSIS
 * <b>findMoreThan12</b>  <b>-h</b> <br/>
 * <b>findMoreThan12</b> <b>-e</b>&nbsp;\argarg{ARG} <b>-p</b>&nbsp;\argarg{POSITION} <b>-m</b>&nbsp;\argarg{NUM} <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-T</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-E</b>&nbsp;\argarg{TIME}<b>]</b>
 *
 * \section findMoreThan12_description DESCRIPTION
 * This application finds when there are simultaneously more than 12
 * SVs above a given elevation.  This is generally meant to be used
 * for 12-channel receivers to determine when such a receiver will not
 * be able to track all satellites in view.
 *
 * \dictionary
 * \dicterm{-e, \--eph-files=\argarg{ARG}}
 * \dicdef{Ephemeris source file(s). Can be RINEX nav, SP3.}
 * \dicterm{-p, \--position=\argarg{POSITION}}
 * \dicdef{Antenna position in ECEF meters (x y z)}
 * \dicterm{-m, \--min-elev=\argarg{NUM}}
 * \dicdef{Give an integer for the elevation (degrees) above which you want to find more than 12 SVs at a given time.}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-T, \--time=\argarg{TIME}}
 * \dicdef{start time of simulation (YYYY DOY SOD)}
 * \dicterm{-E, \--end-time=\argarg{TIME}}
 * \dicdef{end time of simulation (YYYY DOY SOD)}
 * \enddictionary
 *
 * \section findMoreThan12_examples EXAMPLES
 *
 * \cmdex{findMoreThan12 -e data/glob200a.15n -p "1275746.600 -6252216.754 -15441.113" -m 7}
 *
 * Will output lists of epochs (times) at which there are more than 12
 * SVs are above 7 degrees elevation at the given position.  The
 * output table includes the time stamp, PRN, and the elevation and
 * azimuth of the satellite.
 *
 * \section findMoreThan12_exit_status EXIT STATUS
 * Abort/failure error codes given on return:
 * \dictable
 * \dictentry{0,No errors occurred}
 * \dictentry{1,A C++ exception occurred}
 * \enddictable
 *
 * \section findMoreThan12_see_also SEE ALSO
 * \ref compSatVis, \ref compStaVis, \ref wheresat
 */

#include <iostream>
#include <iomanip>

#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/CommonTime.hpp>
#include <gnsstk/CommandOptionWithPositionArg.hpp>
#include <gnsstk/CommandOptionWithCommonTimeArg.hpp>
#include <gnsstk/CivilTime.hpp>
#include <gnsstk/TimeString.hpp>
#include <gnsstk/NavLibrary.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>
#include "NewNavInc.h"

using namespace std;
using namespace gnsstk;

class FindMoreThan12 : public BasicFramework
{
public:
   FindMoreThan12(const std::string& applName);

   virtual bool initialize( int argc,
                            char *argv[],
                            bool pretty = true )
      throw();

      /// load ephemeris data
   virtual void additionalSetup();

   virtual void process();

      /// Specify the location(s) of ephemeris data files
   CommandOptionWithAnyArg ephFiles;
      /// Specify the reference antenna position
   CommandOptionWithPositionArg antennaPosition;
      /// Cut-off elevation at which point the user cares about >12 SVs in view
   CommandOptionWithNumberArg minElev;
      /// Allow the user to specify a time to start processing
   CommandOptionWithCommonTimeArg startTime;
      /// Allow the user to specify a time to stop processing
   CommandOptionWithCommonTimeArg endTime;

      /// User's requested elevation cut-off.
   int minEl;
      /// High level nav store interface.
   NavLibrary navLib;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
      /// Start and end times of processing
   CommonTime tstart, tend;
};


FindMoreThan12 ::
FindMoreThan12(const std::string& applName)
      : BasicFramework(applName, "Find when there are simultaneously more"
                       " than 12 SVs above a given elevation."),
        ephFiles('e', "eph-files",
                 "If you see this, we failed.",
                 true),
        antennaPosition('p', "position", "%x %y %z",
                        "Antenna position in ECEF meters (x y z)",
                        true),
        minElev('m', "min-elev",
                "Give an integer for the elevation (degrees) above which you"
                " want to find more than 12 SVs at a given time.",
                true),
        startTime('T', "time", "%Y %j %s", "start time of simulation (YYYY DOY"
                  " SOD)"),
        endTime('E', "end-time", "%Y %j %s", "end time of simulation (YYYY DOY"
                " SOD)")
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   ephFiles.setDescription("Ephemeris source file(s). Can be " +
                           ndfp->getFactoryFormats() + ".");
   antennaPosition.setMaxCount(1);
   minElev.setMaxCount(1);
   startTime.setMaxCount(1);
   endTime.setMaxCount(1);
}


bool FindMoreThan12 ::
initialize( int argc,
            char *argv[],
            bool pretty )
   throw()
{
   if (!BasicFramework::initialize(argc, argv, pretty))
      return false;

      // get the minimum elevation
   minEl = gnsstk::StringUtils::asInt((minElev.getValue())[0]);
   if (minEl < 0)
   {
      cerr << "Please enter a positive elevation." << endl;
      return false;
   }

   navLib.addFactory(ndfp);
      // without clock, SP3 doesn't work.
   navLib.setTypeFilter({NavMessageType::Ephemeris, NavMessageType::Clock});

   return true;
}


void FindMoreThan12 ::
additionalSetup()
{
      // get the ephemeris source(s)
   vector<string> names = ephFiles.getValue();
   for (size_t i=0; i<names.size(); i++)
   {
      if (!ndfp->addDataSource(names[i]))
      {
         cerr << "Unable to load \"" << names[i] << "\"" << endl;
         exitCode = BasicFramework::EXIST_ERROR;            
      }
   }

   if (startTime.getCount())
   {
      tstart = startTime.getTime()[0];
      tstart.setTimeSystem(TimeSystem::Any);
   }
   else
   {
      tstart = navLib.getInitialTime();
   }
   if (endTime.getCount())
   {
      tend = endTime.getTime()[0];
      tend.setTimeSystem(TimeSystem::Any);
   }
   else
   {
      tend = navLib.getFinalTime();
   }
}


void FindMoreThan12 ::
process()
{
   CommonTime t = tstart;

   cout << "Start Time: " << printTime(tstart, "%02m/%02d/%04Y %02H:%02M:%02S")
        << " End Time: "  << printTime(tend,   "%02m/%02d/%04Y %02H:%02M:%02S")
        << endl;

   Position antXYZ = antennaPosition.getPosition()[0];

   while (t < tend)
   {
      short numSVsAboveElv = 0;
      for (int prn=1; prn <= gnsstk::MAX_PRN; prn++)
      {
         try
         {
            gnsstk::Xvt peXVT;
            if (navLib.getXvt(
                   NavSatelliteID(SatID(prn,SatelliteSystem::GPS)),t,peXVT,
                   false, SVHealth::Any, NavValidityType::ValidOnly,
                   NavSearchOrder::Nearest))
            {
               double elvAngle = antXYZ.elvAngle(peXVT.x);
               if ( elvAngle > minEl )
                  numSVsAboveElv++;
            }
         }
         catch(gnsstk::Exception& e)
         {
            if (verboseLevel)
               cout << e << endl;
         }
      }

      if (numSVsAboveElv > 12)
      {
         cout << "Found " << numSVsAboveElv << " SVs above " << minEl
              << " degrees at "
              << printTime(t, "%02m/%02d/%04Y %02H:%02M:%02S") << endl;

         for (int prn = 1; prn <= gnsstk::MAX_PRN; prn++)
         {
            try
            {
               gnsstk::Xvt peXVT;
               if (navLib.getXvt(
                      NavSatelliteID(SatID(prn,SatelliteSystem::GPS)),t,peXVT,
                      false, SVHealth::Any, NavValidityType::ValidOnly,
                      NavSearchOrder::Nearest))
               {
                  double elvAngle = antXYZ.elvAngle(peXVT.x);
                  if ( elvAngle > 0 )
                  {
                     double azAngle = antXYZ.azAngle(peXVT.x);
                     cout << printTime(t, "%02m/%02d/%04Y %02H:%02M:%02S")
                          << "  PRN " << setw(2) << prn
                          << " : elev: " << elvAngle
                          << "  azim: " << azAngle
                          << " degrees" << endl;
                  }
               }
            }
            catch(gnsstk::Exception& e)
            {
               if (verboseLevel)
                  cout << e << endl;
            }
         }
      }
      t += 10;
   }
}


int main(int argc, char* argv[])
{
   try
   {
#include "NewNavInit.h"
      FindMoreThan12 app(argv[0]);

      if (!app.initialize(argc, argv))
         return app.exitCode;

      if(!app.run())
         return app.exitCode;

      return app.exitCode;
   }
   catch(Exception& e)
   {
      cout << e << endl;
   }
   catch(std::exception& e)
   {
      cout << e.what() << endl;
   }
   catch(...)
   {
      cout << "Caught an unknown exception." << endl;
   }
      // only reach this point if an exception was caught
   return BasicFramework::EXCEPTION_ERROR;
}
