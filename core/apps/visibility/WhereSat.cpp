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
 * \page wheresat wheresat - Display a Table of Satellite ECEF Positions
 * \tableofcontents
 *
 * \section wheresat_name NAME
 * wheresat - Display a Table of Satellite ECEF Positions
 *
 * \section wheresat_synopsis SYNOPSIS
 * <b>wheresat</b>  <b>-h</b> <br/>
 * <b>wheresat</b> <b>-e</b>&nbsp;\argarg{ARG} <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-i</b><b>]</b> <b>[-V</b><b>]</b> <b>[-u</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--start</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--end</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-f</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-s</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-p</b>&nbsp;\argarg{NUM}<b>]</b> <b>[-t</b>&nbsp;\argarg{NUM}<b>]</b>
 *
 * \section wheresat_description DESCRIPTION
 * This application uses input ephemeris to compute the predicted
 * location of a satellite. The Earth-centered, Earth-fixed (ECEF)
 * position of the satellite is reported. Optionally, the topocentric
 * coordinates--azimuth, elevation, and range--can be generated. The
 * user can specify the time interval between successive
 * predictions. Also the output can be generated in a format easily
 * imported into numerical packages.
 *
 * \dictionary
 * \dicterm{-e, \--eph-files=\argarg{ARG}}
 * \dicdef{Ephemeris source file(s). Can be RINEX nav, SP3, RawNavCSV, or FIC.}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-i, \--noHealth}
 * \dicdef{Ignore bad SV health.}
 * \dicterm{-V, \--velocity}
 * \dicdef{Display velocity in addition to position}
 * \dicterm{-u, \--position=\argarg{ARG}}
 * \dicdef{Antenna position (m) in ECEF (x,y,z) coordinates.  Format as a string: "X Y Z". Used to give user-centered data (SV range, azimuth & elevation) when SV is in view.}
 * \dicterm{\--start=\argarg{ARG}}
 * \dicdef{Ignore data before this time. Format as string: "MO/DD/YYYY HH:MM:SS".}
 * \dicterm{\--end=\argarg{ARG}}
 * \dicdef{Ignore data after this time. Format as string: "MO/DD/YYYY HH:MM:SS".}
 * \dicterm{-f, \--time-format=\argarg{ARG}}
 * \dicdef{CommonTime format specifier used for times in the output. The default is "%02m/%02d/%4Y %02H:%02M:%04.1f".}
 * \dicterm{-s, \--sat=\argarg{ARG}}
 * \dicdef{Which SVs to analyze (by RINEX SatID.  e.g., G01 E30). Repeat option for multiple satellites. If this option is not specified, all ephemeris data will be processed.}
 * \dicterm{-p, \--prn=\argarg{NUM}}
 * \dicdef{Which SVs to analyze (by PRN). Repeat option for multiple satellites. If this option is not specified, all ephemeris data will be processed.}
 * \dicterm{-t, \--time=\argarg{NUM}}
 * \dicdef{Time increment for ephemeris calculation. Enter increment in seconds. Default is 900 (15 min).}
 * \enddictionary
 *
 * \section wheresat_examples EXAMPLES
 *
 * \code{.sh}
 * > wheresat -e ../../data/glob200a.15n  | head -15
 * Scanning over prnSet.
 * Scan complete, # of PRNs on command line = 0
 * Set timeFormat to %02m/%02d/%04Y %02H:%02M:%04.1f
 * positionOpt has count = 0
 * # time, PRN, X(m), Y(m), Z(m), Clock Correction(ms), Azimuth(deg), Elevation(deg), Range(m)
 * File read by NavLibrary.
 * Number of satellites for which positions will be computed = 31
 * 07/19/2015 00:00:00 Any
 * 07/20/2015 02:00:00 Any
 * 07/19/2015 00:00:00.0  G02    3708172.649  -15380790.820   21695248.673   0.579090        -        -               -
 * 07/19/2015 00:00:00.0  G03   13652480.299   20791847.447    9285059.991   0.008500        -        -               -
 * 07/19/2015 00:00:00.0  G04   10011698.237   16084389.489  -19050243.469  -0.002465        -        -               -
 * 07/19/2015 00:00:00.0  G05    -939523.230  -24616625.984    9624905.222  -0.216443        -        -               -
 * 07/19/2015 00:00:00.0  G06   17807400.194   -8812349.264   17624772.330   0.019302        -        -               -
 * 07/19/2015 00:00:00.0  G07   26053971.886    6163905.693      -3859.401   0.475214        -        -               -
 * ...
 * \endcode
 *
 * \section wheresat_exit_status EXIT STATUS
 * Abort/failure error codes given on return:
 * \dictable
 * \dictentry{0,No errors occurred}
 * \dictentry{1,A C++ exception or other error occurred}
 * \enddictable
 *
 * \section wheresat_see_also SEE ALSO
 * \ref compSatVis, \ref compStaVis, \ref svvis
 */

/** @todo Use BasicFramework to get proper handling of help and
 * similar command-line options. */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>

#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/CommonTime.hpp>
#include <gnsstk/CommandOption.hpp>
#include <gnsstk/CommandOptionParser.hpp>
#include <gnsstk/WGS84Ellipsoid.hpp>
#include <gnsstk/NavLibrary.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>
#include <gnsstk/RinexSatID.hpp>
#include <gnsstk/CivilTime.hpp>
#include <gnsstk/TimeString.hpp>
#include <gnsstk/Xvt.hpp>
#include "NewNavInc.h"

using namespace std;
using namespace gnsstk;
using namespace gnsstk::StringUtils;


class WhereSat : public BasicFramework
{
public:
   WhereSat(const string& applName);

   void process() override;

   CommandOptionNoArg ignoreHealthOpt;
   CommandOptionNoArg velOpt;
   CommandOptionWithAnyArg ephFiles;
   CommandOptionWithAnyArg positionOpt;
   CommandOptionWithAnyArg startTimeOpt;
   CommandOptionWithAnyArg endTimeOpt;
   CommandOptionWithAnyArg formatOpt;
   CommandOptionWithAnyArg sidOpt;
   CommandOptionWithNumberArg prnOpt;
   CommandOptionWithNumberArg incrementOpt;
      /// High level nav store interface.
   NavLibrary navLib;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
};


WhereSat ::
WhereSat(const string& applName)
      : BasicFramework(applName, "Computes ephemeris data."),
        ignoreHealthOpt('i',"noHealth","Ignore bad SV health."),
        velOpt('V',"velocity","Display velocity in addition to position"),
        ephFiles('e',"eph-files","Ephemeris source file(s). Can be RINEX nav,"
                 " SP3, RawNavCSV, or FIC.",true),
        positionOpt('u',"position","Antenna position (m) in ECEF (x,y,z)"
                    " coordinates.  Format as a string: \"X Y Z\". Used to give"
                    " user-centered data (SV range, azimuth & elevation) when"
                    " SV is in view."),
        startTimeOpt('\0',"start","Ignore data before this time. Format as"
                     " string: \"MO/DD/YYYY HH:MM:SS\"."),
        endTimeOpt('\0',"end", "Ignore data after this time. Format as string: "
                   "\"MO/DD/YYYY HH:MM:SS\"."),
        formatOpt('f',"time-format","CommonTime format specifier used for times"
                  " in the output. The default is \"%02m/%02d/%4Y"
                  " %02H:%02M:%04.1f\"."),
        sidOpt('s',"sat","Which SVs to analyze (by RINEX SatID.  e.g., G01"
               " E30). Repeat option for multiple satellites. If this option is"
               " not specified, all ephemeris data will be processed."),
        prnOpt('p',"prn","Which SVs to analyze (by PRN). Repeat option for"
               " multiple satellites. If this option is not specified, all"
               " ephemeris data will be processed."),
        incrementOpt('t',"time","Time increment for ephemeris calculation. "
                     "Enter increment in seconds. Default is 900 (15 min).")
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   ephFiles.setDescription("Ephemeris source file(s). Can be " +
                           ndfp->getFactoryFormats() + ".");
}


void WhereSat ::
process()
{
      // prnSet is a set of int that represents the PRN numbers
      // requested on the command line (if any).  It is system
      // agnostic.  That is to say, the GNSS will be determined by the
      // data set that is loaded.
   cout << "Scanning over prnSet." << endl;
   set<SatID> prnSet;
   size_t index;
   for (index = 0; index < prnOpt.getCount(); index++)
   {
      int prn = asInt(prnOpt.getValue()[index]);
      SatID sid(prn,SatelliteSystem::GPS);
      prnSet.insert(sid);
   }

   bool failCount = false;
   for (index=0;index<sidOpt.getCount(); index++)
   {
      try
      {
         RinexSatID rsid = RinexSatID(sidOpt.getValue()[index]);
         SatID sid = SatID(rsid.id,rsid.system);
         prnSet.insert(sid);
      }
      catch (Exception)
      {
         cout << "Invalid SV " << sidOpt.getValue()[index];
         failCount = true;
      }
   }
   if (failCount)
   {
      cout << "One of more invalid SVs specified.  wheresat will terminate."
           << endl;
      exitCode = 1;
      return;
   }
   cout << "Scan complete, # of PRNs on command line = " << prnSet.size()
        << endl;

   std::string timeFormat;
   if (formatOpt.getCount())
      timeFormat = formatOpt.getValue()[0];
   else
//    timeFormat = "%4Y %3j %02H:%02M:%04.1f";
      timeFormat = "%02m/%02d/%04Y %02H:%02M:%04.1f";
   cout << "Set timeFormat to " << timeFormat << endl;

   cout << "positionOpt has count = " << positionOpt.getCount() << endl;

   cout << "# time, PRN, X(m), Y(m), Z(m), Clock Correction(ms), Azimuth(deg),"
        << " Elevation(deg), Range(m)";
   if (velOpt)
   {
      cout << ", Vx(m/s), Vy(m/s), Vz(m/s)";
   }
   cout << endl;

   Xvt antXvt;
   if (positionOpt.getCount())
   {
      double x, y, z;
      sscanf(positionOpt.getValue().front().c_str(),"%lf %lf %lf", &x, &y, &z);
      antXvt.x[0] = x; antXvt.x[1] = y; antXvt.x[2] = z;
   }

   int incr=900;
   if (incrementOpt.getCount())
      incr = asInt(incrementOpt.getValue()[0]);

      // get the ephemeris source(s)
      // without clock, SP3 doesn't work.
   navLib.addFactory(ndfp); // must be done before setTypeFilter
   navLib.setTypeFilter({NavMessageType::Ephemeris, NavMessageType::Clock});
   if (debugLevel)
   {
      cout << "Attempting to read ephemeris data." << endl;
   }
   vector<string> names = ephFiles.getValue();
   for (size_t i=0; i<names.size(); i++)
   {
      if (!ndfp->addDataSource(names[i]))
      {
         cerr << "Unable to load \"" << names[i] << "\"" << endl;
         exitCode = BasicFramework::EXIST_ERROR;            
      }
      else
      {
         cout << "File read by NavLibrary." << endl;
      }
   }

      // Get the list of SatIDs that are available in the navigation message
      // store.  Convert this to a set that will be used later.
   if (debugLevel)
   {
      cout << "Figuring out set of PRNs for which calculations will be"
           << " performed." << endl;
   }
   bool loadPrnSet = false;
   if (prnSet.size()==0)
      loadPrnSet = true;
   std::set<gnsstk::SatID> storedSats = navLib.getIndexSet(
      CommonTime::BEGINNING_OF_TIME, CommonTime::END_OF_TIME);

      // This will hold a list of the SVs for which positions are to
      // be calculated
   std::set<gnsstk::SatID> satSet;

      // Check command line entered PRNs against the satellite IDs
      // available in the set.  Make a list of all the satellites for
      // which posotions are to be computed.  iF no PRNs were enetered
      // on the command line, copy all of satList into satSet.
   std::set<gnsstk::SatID>::const_iterator cit;
   if (prnSet.size()==0)
   {
      for (cit=storedSats.begin();cit!=storedSats.end();cit++)
      {
         SatID nextID = *cit;
         satSet.insert(nextID);
      }
   }
      // If PRNs were entered on the commmand line, see if they exist
      // in the list of satellites in the store.  If they do NOT, then
      // warn the user.
   else
   {
      std::set<SatID>::const_iterator cit2;
      for (cit2=prnSet.begin();cit2!=prnSet.end();cit2++)
      {
         bool found = false;
         const SatID& testID = *cit2;
         cit = storedSats.find(testID);
         if (cit!=storedSats.end())
            satSet.insert(testID);
         else
         {
            cout << "PRN " << testID << " entered on command line, but not"
                 << " found in the set of orbit elements." << endl;
         }
      }
   }
   cout << "Number of satellites for which positions will be computed = "
        << satSet.size() << endl;

   CivilTime tS, tE;
   CommonTime tStart,tEnd;
   if (startTimeOpt.getCount())
   {
      double ss;
      int mm,dd,yy,hh,minu;
      sscanf(startTimeOpt.getValue().front().c_str(),
             "%d/%d/%d %d:%d:%lf",&mm,&dd,&yy,&hh,&minu,&ss);
      tS=CivilTime((short)yy, (short)mm, (short)dd, (short)hh,
                   (short)minu, (double)ss, TimeSystem::Any);
      tStart = tS.convertToCommonTime();
      cout << tS << endl;
   }
   else
   {
         //extra code b/c sscanf reads in int's but CivilTime needs shorts
         //cout << navLib.getInitialTime() << endl;
      CommonTime tFile(navLib.getInitialTime());
      short year = static_cast<CivilTime>(tFile).year;
      short month = static_cast<CivilTime>(tFile).month;
      short day = static_cast<CivilTime>(tFile).day;
      short hour = static_cast<CivilTime>(tFile).hour;
      short minute = static_cast<CivilTime>(tFile).minute;
      double ss = static_cast<CivilTime>(tFile).second;
      tS = CivilTime(year,month,day,hour,minute,ss, TimeSystem::Any);
      tStart = tS.convertToCommonTime();
      cout << tS << endl;
   }

   if (endTimeOpt.getCount())
   {
      double ss;
      int mm,dd,yy,hh,minu;
      sscanf(endTimeOpt.getValue().front().c_str(),
             "%d/%d/%d %d:%d:%lf",&mm,&dd,&yy,&hh,&minu,&ss);
      tE = CivilTime((short)yy, (short)mm, (short)dd, (short)hh, (short)minu,
                     (double)ss, TimeSystem::Any);
      tEnd = tE.convertToCommonTime();
      cout << tE << endl;
   }
   else
   {
         //extra code b/c sscanf reads in int's but CivilTime needs shorts
      CommonTime tFile(navLib.getFinalTime());
      short year = static_cast<CivilTime>(tFile).year;
      short month = static_cast<CivilTime>(tFile).month;
      short day = static_cast<CivilTime>(tFile).day;
      short hour = static_cast<CivilTime>(tFile).hour;
      short minute = static_cast<CivilTime>(tFile).minute;
      double ss = static_cast<CivilTime>(tFile).second;
      tE = CivilTime(year,month,day,hour,minute,ss, TimeSystem::Any);
      tEnd = tE.convertToCommonTime();
      cout << tE << endl;
   }

   CommonTime t = tStart;
   while (t <= tEnd)
   {
      for(set<SatID>::iterator i = satSet.begin(); i != satSet.end(); i++ )
      {
         SatID thisSat = *i;
         RinexSatID rsid(thisSat);   // Used only for output formatting
         Xvt xvt;
         if (navLib.getXvt(
                NavSatelliteID(thisSat),t,xvt,false,
                (ignoreHealthOpt ? SVHealth::Any : SVHealth::Healthy),
                NavValidityType::ValidOnly, NavSearchOrder::Nearest))
         {
            cout << printTime(t,timeFormat)
                 << right << fixed  << setprecision(3)
                 << " " << setw(4)  <<  rsid
                 << " " << setw(14) << xvt.x[0]
                 << " " << setw(14) << xvt.x[1]
                 << " " << setw(14) << xvt.x[2]
                 << " " << setprecision(6) << setw(10)
                 << ((xvt.clkbias + xvt.relcorr)*1000);

            WGS84Ellipsoid ellipsoid;
            double correction = 0;

            if ( abs(antXvt.x[0]) < 1 || antXvt.x.elvAngle(xvt.x) < 0 )
            {
               cout << right
                    << " "  << setw(8) << "-"
                    << " "  << setw(8) << "-"
                    << " "  << setw(15) << "-";
            }
            else
            {
               cout << right << fixed << setprecision(3)
                    << " "  << setw(8) << antXvt.x.azAngle(xvt.x)
                    << " "  << setw(8) << antXvt.x.elvAngle(xvt.x)
                    << " "  << setw(15)
                    << xvt.preciseRho(antXvt.x, ellipsoid, correction);
            }
            if (velOpt)
            {
               cout << right << fixed << setprecision(3)
                    << " " << setw(9) << xvt.v[0]
                    << " " << setw(9) << xvt.v[1]
                    << " " << setw(9) << xvt.v[2];
            }
            cout << endl;
         }
      }

      t += incr;
   }
}


int main(int argc, char *argv[])
{
   try
   {
#include "NewNavInit.h"
      WhereSat app(argv[0]);

      if (app.initialize(argc, argv))
      {
         app.run();
      }
      return app.exitCode;
   }
   catch(Exception& e)
   {
      cout << e << endl;
   }
   catch(std::exception& e)
   {
      cout << "Caught std::exception " << e.what() << endl;
   }
   catch(...)
   {
      cout << "Caught unknown exception" << endl;
   }
      // only reach this point if an exception was caught
   return BasicFramework::EXCEPTION_ERROR;
}
