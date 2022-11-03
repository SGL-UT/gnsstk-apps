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
 * \page rinexelvstrip rinexelvstrip - Remove RINEX OBS data below a specified elevation angle
 * \tableofcontents
 *
 * \section rinexelvstrip_name NAME
 * rinexelvstrip - Remove RINEX OBS data below a specified elevation angle
 *
 * \section rinexelvstrip_synopsis SYNOPSIS
 * <b>rinexelvstrip</b>  <b>-h</b> <br/>
 * <b>rinexelvstrip</b> <b>-i</b>&nbsp;\argarg{ARG} <b>-o</b>&nbsp;\argarg{ARG} <b>-n</b>&nbsp;\argarg{ARG} <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-p</b>&nbsp;\argarg{POSITION}<b>]</b> <b>[-e</b>&nbsp;\argarg{ARG}<b>]</b>
 *
 * \section rinexelvstrip_description DESCRIPTION
 * Use RINEX nav data to process RINEX obs data and strip any
 * observations that are below a set elevation angle. It requires an
 * antenna position to work, if one is specified on the command line
 * it will use that, if not, it will look for one in the RINEX obs
 * header.
 * 
 * \dictionary
 * \dicterm{-i, \--input=\argarg{ARG}}
 * \dicdef{Input RINEX obs file}
 * \dicterm{-o, \--output=\argarg{ARG}}
 * \dicdef{Output RINEX obs file}
 * \dicterm{-n, \--nav=\argarg{ARG}}
 * \dicdef{Input RINEX or FIC nav file(s)}
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-p, \--position=\argarg{POSITION}}
 * \dicdef{position (XYZ ECEF in meters)}
 * \dicterm{-e, \--elevation=\argarg{ARG}}
 * \dicdef{elevation strip angle (default = 10 degrees)}
 * \enddictionary
 *
 * \section rinexelvstrip_examples EXAMPLES
 *
 * \cmdex{rinexelvstrip -i data/305-2015/s071305a.15o -o rinexelvstrip.out -n data/305-2015/s071305a.15n}
 *
 * Produces a file called rinexelvstrip.out containing only the
 * observation data in data/305-2015/s071305a.15o that is above 10
 * degrees elevation.  The elevation is determined using the
 * navigation data in data/305-2015/s071305a.15n.
 *
 * \section rinexelvstrip_exit_status EXIT STATUS
 * Abort/failure error codes given on return:
 * \dictable
 * \dictentry{0,No errors occurred}
 * \dictentry{1,A C++ exception occurred}
 * \enddictable
 */

#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/CommandOptionWithPositionArg.hpp>
#include <gnsstk/RinexObsData.hpp>
#include <gnsstk/RinexObsHeader.hpp>
#include <gnsstk/RinexObsStream.hpp>
#include <gnsstk/StringUtils.hpp>
#include <gnsstk/NavLibrary.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>
#include <gnsstk/TimeString.hpp>
#include "NewNavInc.h"

using namespace std;
using namespace gnsstk;

   /**
    * This class uses RINEX nav data to process RINEX obs data and strips
    * any observations that are below a set elevation angle. It requires
    * an antenna position to work, if one is specified on the command line
    * it will use that, if not, it will look for one in the RINEX obs
    * header.
    */
class RinexElvStrip : public BasicFramework
{
public:
   RinexElvStrip(const string& app) noexcept;
   virtual ~RinexElvStrip() noexcept {}
   virtual bool initialize(int argc, char *argv[], bool pretty=true) noexcept;

protected:
   virtual void additionalSetup();
   virtual void process();

private:
   static const double DEFAULT_ELEVATION_MASK;
   CommandOptionWithAnyArg inputArg;
   CommandOptionWithAnyArg outputArg;
   CommandOptionWithAnyArg navArg;
   CommandOptionWithPositionArg posArg;
   CommandOptionWithAnyArg elvArg;
   Position pos;
   double elvmask;
      /// High level nav store interface.
   NavLibrary navLib;
      /// nav data file reader
   NavDataFactoryPtr ndfp;
};

const double RinexElvStrip::DEFAULT_ELEVATION_MASK = 10.0;

RinexElvStrip::RinexElvStrip(const string& app) noexcept
      : BasicFramework(app, "Strip low elevation data from observations."),
        inputArg('i', "input", "Input RINEX obs file", true),
        outputArg('o', "output", "Output RINEX obs file", true),
        navArg('n', "nav", "Input nav file(s)", true),
        posArg('p', "position", "%x %y %z", "position (XYZ ECEF in meters)"),
        elvArg('e', "elevation", "elevation strip angle (default = 10 degrees)")
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   navArg.setDescription("Input nav file(s). Can be " +
                         ndfp->getFactoryFormats() + ".");
   inputArg.setMaxCount(1);
   outputArg.setMaxCount(1);
   posArg.setMaxCount(1);
   elvArg.setMaxCount(1);
}

bool RinexElvStrip::initialize(int argc, char *argv[], bool pretty) noexcept
{
   if (!BasicFramework::initialize(argc, argv, pretty))
      return false;

   if (elvArg.getCount())
      elvmask = StringUtils::asDouble(elvArg.getValue()[0]);
   else
      elvmask = DEFAULT_ELEVATION_MASK;

   navLib.addFactory(ndfp);
      // without clock, SP3 doesn't work.
   navLib.setTypeFilter({NavMessageType::Ephemeris, NavMessageType::Almanac,
                         NavMessageType::Clock});

   return true;
}

void RinexElvStrip::additionalSetup()
{
      // get the ephemeris source(s)
   vector<string> names = navArg.getValue();
   for (size_t i=0; i<names.size(); i++)
   {
      if (!ndfp->addDataSource(names[i]))
      {
         cerr << "Unable to load \"" << names[i] << "\"" << endl;
         exitCode = BasicFramework::EXIST_ERROR;            
      }
   }
}

void RinexElvStrip::process()
{
   if (exitCode)
      return; // already failing

   RinexObsStream oros(outputArg.getValue()[0].c_str(), ios::out);
   RinexObsStream iros(inputArg.getValue()[0].c_str());
   RinexObsHeader roh;

   if (!oros)
   {
      cerr << "Can't open \"" << outputArg.getValue()[0] << "\" for output"
           << endl;
      exitCode = 1;
      return;
   }
   if (!iros)
   {
      cerr << "Can't open \"" << inputArg.getValue()[0] << "\" for input"
           << endl;
      exitCode = gnsstk::BasicFramework::EXIST_ERROR;
      return;
   }

   iros >> roh;
   oros << roh;

      // was the position specified on the command line?
   if (posArg.getCount())
   {
      pos = posArg.getPosition()[0];
   }
      // is there a valid position in the header?
   else if (roh.valid & RinexObsHeader::antennaPositionValid)
      pos = roh.antennaPosition;
   else
   {
         // we have no position, terminate with error
      cerr << "No valid position specified or in header." << endl;
      exitCode = 1;
      return;
   }

   RinexObsData irod;
   while (iros >> irod)
   {
      RinexObsData orod;
      orod.time = irod.time;
      orod.epochFlag = irod.epochFlag;
      orod.clockOffset = irod.clockOffset;
      orod.auxHeader = irod.auxHeader;
      for (RinexObsData::RinexSatMap::iterator i = irod.obs.begin();
           i != irod.obs.end(); i++)
      {
         try
         {
            Xvt xvt;
            if (!navLib.getXvt(
                   NavSatelliteID(i->first), irod.time, xvt, SVHealth::Any,
                   NavValidityType::ValidOnly, NavSearchOrder::Nearest))
            {
               cerr << "Couldn't get XVT for " << i->first << " at "
                    << printTime(irod.time, "%02m/%02d/%02Y %02H:%02M:%03.1f")
                    << endl;
               continue;
            }
            double elv = pos.elevation(xvt.getPos());
            if (elv >= elvmask)
               orod.obs[i->first] = i->second;
            else if (verboseLevel)
               cout << "Stripped PRN " << i->first.id << " (elv = "
                    << setprecision(2) << elv << ") at "
                    << printTime(irod.time, "%02m/%02d/%02Y %02H:%02M:%03.1f")
                    << endl;
         }
         catch(InvalidRequest& e)
         {
            if (verboseLevel)
               cerr << e.getText() << endl;
         }
      }
      orod.numSvs = orod.obs.size();
      oros << orod;
   }
   oros.close();
   iros.close();
}

int main(int argc, char *argv[])
{
  try
  {
#include "NewNavInit.h"
     RinexElvStrip proc(argv[0]);
     if (!proc.initialize(argc, argv))
        return proc.exitCode;
     proc.run();
     return proc.exitCode;
  }
  catch(gnsstk::Exception& e)
  {
     cerr << e << endl;
  }
  catch(std::exception& e)
  {
     cerr << e.what() << endl;
  }
  catch(...)
  {
     cerr << "unknown exception" << endl;
  }
  return gnsstk::BasicFramework::EXCEPTION_ERROR;
}
