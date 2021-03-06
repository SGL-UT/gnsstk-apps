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

/** \page apps
 * - \subpage calgps - Generate a GPS calendar in text
 * \page calgps
 * \tableofcontents
 *
 * \section calgps_name NAME
 * calgps - Display time stamps in a variety of formats
 *
 * \section calgps_synopsis SYNOPSIS
 * \b calgps [\argarg{OPTION}] ...
 *
 * \section calgps_description DESCRIPTION
 * This application generates a dual GPS and Julian calendar to
 * stdout. The arguments and format are inspired by the UNIX `cal`
 * utility. With no arguments, the current month is printed. The
 * last and next month can also be printed. Also, the current or any
 * given year can be printed.
 *
 * \dictionary
 * \dicterm{-h, \--help}
 * \dicdef{Display argument list.}
 * \dicterm{-3, \--three-months}
 * \dicdef{Display last, this and next months.}
 * \dicterm{-y, \--year}
 * \dicdef{Display all months for the current year}
 * \dicterm{-Y, \--specific-year=\argarg{NUM}}
 * \dicdef{Display all months for a given year}
 * \enddictionary
 *
 * \section calgps_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred}
 * \enddictable
 */

#include "NewNavInc.h"
#include <iostream>
#include <iomanip>
#include <gnsstk/CommonTime.hpp>
#include <gnsstk/CivilTime.hpp>
#include <gnsstk/YDSTime.hpp>
#include <gnsstk/GPSWeekSecond.hpp>
#include <gnsstk/TimeConstants.hpp>
#include <gnsstk/TimeString.hpp>
#include <gnsstk/SystemTime.hpp>
#include <gnsstk/CommandOptionParser.hpp>
#include <gnsstk/StringUtils.hpp>
#include <gnsstk/BasicFramework.hpp>

using namespace std;
using namespace gnsstk;


void printMonth(short month, short year)
{
   CivilTime civ(year, month, 1, 0, 0, 0.0);

   cout << endl << civ.printf("%26b %4Y") << endl;

   GPSWeekSecond gws(civ);

   for (; civ.month == month; ++gws.week, gws.sow = 0, civ = gws)
   {
      cout << setw(4) << gws.week << "  ";

      for (short thisDow = 0; thisDow < 7; ++thisDow)
      {
         gws.sow = thisDow * SEC_PER_DAY;
         CommonTime com(gws);
         if (CivilTime(com).month == month)
	   cout << printTime(com, "%2d-%03j ");
         else
           cout << "       ";
      }
      cout << endl;
   }
}

int main(int argc, char* argv[])
{
#include "NewNavInit.h"

   try {

      CommandOptionNoArg helpOption('h',"help","Display argument list.",false);
      CommandOptionNoArg threeOption('3',"three-months","Display last, this and next months.",false);
      CommandOptionNoArg thisYearOption('y',"year","Display all months for the current year");
      CommandOptionWithNumberArg givenYearOption('Y',"specific-year","Display all months for a given year");

      CommandOptionParser cop("GNSSTk GPS Calendar Generator");
      cop.parseOptions(argc, argv);

      if (cop.hasErrors())
      {
         cop.dumpErrors(cout);
         cop.displayUsage(cout);
         return 1;
      }

      if(helpOption.getCount())
      {
         cop.displayUsage(cout);
         return 0;
      }


      // Print this month
      SystemTime st;
      CivilTime now(st);
      int firstMonth = now.month;
      int lastMonth  = now.month;
      int firstYear  = now.year;
      int lastYear   = now.year;

      if (thisYearOption.getCount())
      {
         firstMonth =1;
         lastMonth  =12;
      }

      if (givenYearOption.getCount())
      {
         firstMonth =1;
         lastMonth  =12;

         firstYear = gnsstk::StringUtils::asInt((givenYearOption.getValue())[0]);
         lastYear = firstYear;

      }

      if (threeOption.getCount())
      {
         firstMonth--;
         if (firstMonth==0)
         {
            firstMonth = 12;
            firstYear--;
         }

         lastMonth++;
         if (lastMonth==13)
         {
            lastMonth = 1;
            lastYear++;
         }
      }

      int mcount=0;
      for (short m=firstMonth, y=firstYear;
          (y<lastYear) || ((m<=lastMonth) && (y==lastYear));
           m++)
      {
         if (m==13)
         {
            m=1;
            y++;
         }

          printMonth(m, y);
      }

      cout << endl;


   }
   catch (Exception& error)
   {
      cout << error << endl;
      return gnsstk::BasicFramework::EXCEPTION_ERROR;
   }

   return 0;
}
