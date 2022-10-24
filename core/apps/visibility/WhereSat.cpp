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
 * <b>wheresat</b> <b>-e</b>&nbsp;\argarg{ARG} <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-i</b><b>]</b> <b>[-V</b><b>]</b> <b>[-u</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--start</b>&nbsp;\argarg{TIME}<b>]</b> <b>[\--end</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-f</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-s</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-p</b>&nbsp;\argarg{NUM}<b>]</b> <b>[-t</b>&nbsp;\argarg{NUM}<b>]</b> <b>[-A</b><b>]</b>
 *
 * \section wheresat_description DESCRIPTION
 * This application uses input ephemeris to compute the estimated
 * position and clock state of a satellite. The Earth-centered,
 * Earth-fixed (ECEF) position of the satellite is
 * reported. Optionally, given an antenna position, the topocentric
 * coordinates--azimuth, elevation, and range--can be generated. The
 * user can specify the time interval between successive
 * predictions. Also the output can be generated in a format easily
 * imported into numerical packages.
 *
 * The <i>ephemeris</i> of a GNSS satellite is essentially a set of
 * parametric equations that predict or describe its position and
 * clock offset over time. The ephemeris borrows its meaning from the
 * Greek term describing something that is brief and only lasts a
 * day. This is appropriate as the ephemeris of a satellite is
 * typically only valid for a few hours.
 *
 * Each satellite broadcasts its own ephemeris as part of the
 * navigation message. Those ephemerides are used within a receiver to
 * estimate the user position. It is also possible to extract the
 * ephemerides from a receiver in order to process satellite positions
 * on a computer. The wheresat application is written to support
 * numerous file formats used for storing ephemerides, as well as a
 * variety of GNSSes.
 *
 * Johannes Kepler (1571-1630) made a number of key discoveries about
 * the orbits of the planets, including the fact that those orbits are
 * essentially elliptic in shape. For this reason we refer to orbit
 * parameterizations that use the properties of conic sections as
 * <i>Keplerian</i>. Most GNSS ephemerides are essentially Keplerian
 * (GLONASS being the lone exception). For example, they contain
 * parameters such as eccentricity and semi-major axis. The process of
 * computing a satellite's position based on Keplerian orbit
 * parameters is treated in a number of texts. See references [3],
 * [4], and [5] for general treatments.
 *
 * These GNSS ephemerides do contain parameters (such as perturbations
 * and clock models) not found in classic Keplerian orbit
 * descriptions. For a thorough development of those parameters the
 * reader is referred to references [6] and [7]. The U.S. government,
 * which provides GPS free of charge to all users, also provides the
 * recommended algorithms for using the system for positioning. Those
 * algorithms are defined (but not developed) in the IS-GPS-200
 * (formerly known as the ICD-GPS-200), reference [8]. Other systems
 * have their own ICDs and algorithm details, though they are
 * generally identical.
 *
 * \dictionary
 * \dicterm{-e, \--eph-files=\argarg{ARG}}
 * \dicdef{Ephemeris source file(s). Can be SP3a, SP3b, SP3c, SP3d, RINEX2, RINEX3, Yuma, SEM, RawNavCSV, NovAtel, MDP, FIC, MDH.}
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
 * \dicterm{\--start=\argarg{TIME}}
 * \dicdef{Ignore data before this time. Format as string: "MO/DD/YYYY HH:MM:SS".}
 * \dicterm{\--end=\argarg{TIME}}
 * \dicdef{Ignore data after this time. Format as string: "MO/DD/YYYY HH:MM:SS".}
 * \dicterm{-f, \--time-format=\argarg{ARG}}
 * \dicdef{CommonTime format specifier used for times in the output. The default is "%02m/%02d/%4Y %02H:%02M:%04.1f".}
 * \dicterm{-s, \--sat=\argarg{ARG}}
 * \dicdef{Which SVs to analyze (by RINEX SatID.  e.g., G01 E30). Repeat option for multiple satellites. If this option is not specified, all ephemeris data will be processed.}
 * \dicterm{-p, \--prn=\argarg{NUM}}
 * \dicdef{Which SVs to analyze (by PRN). Repeat option for multiple satellites. If this option is not specified, all ephemeris data will be processed.}
 * \dicterm{-t, \--time=\argarg{NUM}}
 * \dicdef{Time increment for ephemeris calculation. Enter increment in seconds. Default is 900 (15 min).}
 * \dicterm{-A, \--use-alm}
 * \dicdef{Use almanac to compute positions (default=ephemeris)}
 * \enddictionary
 *
 * \section wheresat_examples EXAMPLES
 *
 * The output for wheresat is of the following form:
 * \code{.sh}
 * Date       Time        Sat    ECEF-X(m)    ECEF-Y(m)      ECEF-Z(m)      clock(ms)  az(deg)   el(deg)   Range-to-SV(m)
 * 07/19/2015 00:00:00.0  G13    9094017.832  -20832137.536  -13973496.670  -0.133344  151.603    7.742    25105556.964
 * \endcode
 *
 * Note that the azimuth, elevation and range will be replaced with "-
 * - -" when either no antenna position is specified, or the satellite
 * is below the horizon (elevation less than 0).
 *
 * \subsection wheresat_sat_pos_clock_cor Generating Satellite Position and Clock Correction
 *
 * With only a navigation file and PRN number, wheresat will generate
 * the satellite position and clock correction, beginning with the
 * initial data timestamp given in the file and incrementing by
 * fifteen minutes for the length of the file.  The following sample
 * output demonstrates the most basic processing provided by wheresat.
 *
 * \code{.sh}
 * > wheresat -e ../../data/glob200a.15n -p 13 | head -15
 * Scanning over prnSet.
 * Scan complete, # of PRNs on command line = 1
 * Set timeFormat to %02m/%02d/%04Y %02H:%02M:%04.1f
 * positionOpt has count = 0
 * # time, PRN, X(m), Y(m), Z(m), Clock Correction(ms), Azimuth(deg), Elevation(deg), Range(m)
 * File read by NavLibrary.
 * Number of satellites for which positions will be computed = 1
 * 07/19/2015 00:00:00 GPS
 * 07/20/2015 02:00:00 GPS
 * 07/19/2015 00:00:00.0  G13    9094017.832  -20832137.536  -13973496.670  -0.133344        -        -               -
 * 07/19/2015 00:15:00.0  G13   10251699.424  -21703752.603  -11636968.247  -0.133346        -        -               -
 * 07/19/2015 00:30:00.0  G13   11173672.448  -22436241.683   -9102744.166  -0.133348        -        -               -
 * 07/19/2015 00:45:00.0  G13   11868947.071  -22989512.624   -6413721.728  -0.133349        -        -               -
 * 07/19/2015 01:00:00.0  G13   12354364.901  -23327352.752   -3615500.365  -0.133351        -        -               -
 * 07/19/2015 01:15:00.0  G13   12653789.995  -23418881.468    -755642.293  -0.133354        -        -               -
 * \endcode
 *
 * \subsection wheresat_const_pos_clock_cor Generating Full Constellation Satellite Position and Clock Correction
 *
 * By default, if no PRN/satellite is specified, the complete (known)
 * constellation positions are output.
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
 * 07/19/2015 00:00:00 GPS
 * 07/20/2015 02:00:00 GPS
 * 07/19/2015 00:00:00.0  G02    3708172.649  -15380790.820   21695248.673   0.579090        -        -               -
 * 07/19/2015 00:00:00.0  G03   13652480.299   20791847.447    9285059.991   0.008500        -        -               -
 * 07/19/2015 00:00:00.0  G04   10011698.237   16084389.489  -19050243.469  -0.002465        -        -               -
 * 07/19/2015 00:00:00.0  G05    -939523.230  -24616625.984    9624905.222  -0.216443        -        -               -
 * 07/19/2015 00:00:00.0  G06   17807400.194   -8812349.264   17624772.330   0.019302        -        -               -
 * 07/19/2015 00:00:00.0  G07   26053971.886    6163905.693      -3859.401   0.475214        -        -               -
 * \endcode
 *
 * \subsection wheresat_antenna Generating Satellite Azimuth, Elevation, and Range
 *
 * Additionally, the user may specify the antenna position in
 * Earth-centered, Earth-fixed (ECEF) XYZ coordinates and wheresat will
 * generate the azimuth, elevation, and distance of the satellite from
 * the user, in addition to the satellite ECEF coordinates and clock
 * offset. These three additional quantities form a local spherical coordinate system
 * that locates the satellite relative to the receiver. Azimuth is
 * defined as degrees clockwise from due north, ranging from 0° to
 * 360°.  Elevation is defined as degrees above the horizon, ranging
 * from 0° to 90°.  The navigation message contains orbital data of
 * the satellites for some arbitrary epoch.  Thus, if the satellite is
 * rising or setting during the epoch, the application will continue
 * propagating data values for the satellite until the end of the
 * epoch, even if the satellite is below the horizon. This results in
 * elevation angles below zero degrees. When the satellite is below
 * the horizon, it is typically not trackable by a user on the Earth's
 * surface, and in this case the spherical coordinates will be marked as
 * "- - -".
 *
 * Note that with the -t argument, the time increment was specified as
 * 7200 seconds, or 2 hours, instead of the default setting of 15
 * minutes. Also note that the units of the ECEF user position
 * as well as the SV range are in meters, while the azimuth and
 * elevation are in degrees.
 *
 * \code{.sh}
 * > wheresat -e ../../data/glob200a.15n -p 13 -u "-740289.807 -5457071.744 3207245.649" -t 7200
 * Scanning over prnSet.
 * Scan complete, # of PRNs on command line = 1
 * Set timeFormat to %02m/%02d/%04Y %02H:%02M:%04.1f
 * positionOpt has count = 1
 * # time, PRN, X(m), Y(m), Z(m), Clock Correction(ms), Azimuth(deg), Elevation(deg), Range(m)
 * File read by NavLibrary.
 * Number of satellites for which positions will be computed = 1
 * 07/19/2015 00:00:00 GPS
 * 07/20/2015 02:00:00 GPS
 * 07/19/2015 00:00:00.0  G13    9094017.832  -20832137.536  -13973496.670  -0.133344  151.603    7.742    25105556.964
 * 07/19/2015 02:00:00.0  G13   12755996.264  -22011763.983    7705231.282  -0.133361  102.491   42.947    21867452.631
 * 07/19/2015 04:00:00.0  G13   13513830.032   -7123978.524   21603931.171  -0.133389   41.832   22.769    23372276.645
 * 07/19/2015 06:00:00.0  G13   20722245.547    9238086.196   13575247.825  -0.133424        -        -               -
 * 07/19/2015 08:00:00.0  G13   21654262.184   12927837.705   -8271428.199  -0.133455        -        -               -
 * 07/19/2015 10:00:00.0  G13    6702633.004   13748562.842  -21838270.174  -0.133475        -        -               -
 * 07/19/2015 12:00:00.0  G13   -9265602.882   20957731.520  -13667853.557  -0.133487        -        -               -
 * 07/19/2015 14:00:00.0  G13  -12743178.272   21884826.027    8071233.674  -0.133505        -        -               -
 * 07/19/2015 16:00:00.0  G13  -13589463.267    6791967.081   21661600.961  -0.133533  325.835    0.652    25646758.286
 * 07/19/2015 18:00:00.0  G13  -20846538.843   -9411799.960   13263948.451  -0.133568  285.445   28.190    22866334.160
 * 07/19/2015 20:00:00.0  G13  -21522411.518  -12915209.073   -8634911.153  -0.133599  231.905    6.510    25095158.879
 * 07/19/2015 22:00:00.0  G13   -6375493.973  -13825016.952  -21889591.386  -0.133619        -        -               -
 * 07/20/2015 00:00:00.0  G13    9432680.607  -21081294.751  -13358178.204  -0.133631  150.372    8.904    24980352.821
 * 07/20/2015 02:00:00.0  G13   12729578.198  -21752544.263    8434380.734  -0.133648   99.851   43.603    21818537.796
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
 *
 *   1. The GNSS Toolkit, GNSSTk.  Website: http://www.gnsstk.org/
 *   2. RINEX: The Receiver Independent Exchange Format Version 2.10.  Available on the web at http://www.ngs.noaa.gov/CORS/Rinex2.html
 *   3. Roger Bate, Donald Mueller, and Jerry White. <i>Fundamentals of Astrodynamics</i>. Dover. New York, NY. 1971.
 *   4. A. E. Roy. <i>Orbital Motion, 2nd Edition</i>. Adam Hilger Ltd. Bristol, Great Britain. 1982.
 *   5. Battin, Richard. <i>An Introduction to the Mathematics and Methods of Astrodynamics</i>, Revised Editino. AIAA Press. Reston, Virginia, 1999.
 *   6. B. Hoffman-Wellenhof, H. Lichtenegger, and J. Collins. GPS: Theory and Practice, 5th edition. Springer Verlag.  New York, NY. 2001
 *   7. Misra, Pratap and Enge, Per. <i>Global Positioning System: Signals, Measurements and Performance</i>.  Ganga-Jamuna Press. 2004.
 *   8. ICD-GPS-200, now more recently renamed to IS-GPS-200. Website: http://www.navcen.uscg.gov/ftp/policy/icd200/ICD200Cw1234.pdf
 *
 * \section wheresat_authors AUTHORS
 * wheresat was written by Tracie Conn and other employees of ARL:UT.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>

#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/CommonTime.hpp>
#include <gnsstk/CommandOption.hpp>
#include <gnsstk/CommandOptionWithCommonTimeArg.hpp>
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
   CommandOptionWithCommonTimeArg startTimeOpt;
   CommandOptionWithCommonTimeArg endTimeOpt;
   CommandOptionWithAnyArg formatOpt;
   CommandOptionWithAnyArg sidOpt;
   CommandOptionWithNumberArg prnOpt;
   CommandOptionWithNumberArg incrementOpt;
   CommandOptionNoArg almOpt;
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
        startTimeOpt('\0',"start","%m/%d/%Y %H:%M:%S",
                     "Ignore data before this time. Format as"
                     " string: \"MO/DD/YYYY HH:MM:SS\"."),
        endTimeOpt('\0',"end","%m/%d/%Y %H:%M:%S",
                   "Ignore data after this time. Format as string: "
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
                     "Enter increment in seconds. Default is 900 (15 min)."),
        almOpt('A', "use-alm", "Use almanac to compute positions"
               " (default=ephemeris)")
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
      tStart = startTimeOpt.getTime()[0];
      tStart.setTimeSystem(gnsstk::TimeSystem::Any);
   }
   else
   {
      tStart = navLib.getInitialTime();
   }
   tS = tStart;
   cout << tS << endl;

   if (endTimeOpt.getCount())
   {
      tEnd = endTimeOpt.getTime()[0];
      tEnd.setTimeSystem(gnsstk::TimeSystem::Any);
   }
   else
   {
      tEnd = navLib.getFinalTime();
   }
   tE = tEnd;
   cout << tE << endl;

   CommonTime t = tStart;
   if (debugLevel)
   {
      cerr << "dump:" << endl;
      navLib.dump(std::cerr, gnsstk::DumpDetail::OneLine);
   }
   while (t <= tEnd)
   {
      for(set<SatID>::iterator i = satSet.begin(); i != satSet.end(); i++ )
      {
         SatID thisSat = *i;
         RinexSatID rsid(thisSat);   // Used only for output formatting
         Xvt xvt;
         if (navLib.getXvt(
                NavSatelliteID(thisSat),t,xvt, almOpt,
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
