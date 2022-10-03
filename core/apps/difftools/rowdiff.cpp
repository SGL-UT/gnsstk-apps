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
 * - \subpage rowdiff - Difference the contents of two RINEX OBS files
 * \page rowdiff
 * \tableofcontents
 *
 * \section rowdiff_name NAME
 * rowdiff - Difference the contents of two RINEX OBS files
 *
 * \section rowdiff_synopsis SYNOPSIS
 * \b rowdiff [\argarg{OPTION}] ... file1 file2
 *
 * \section rowdiff_description DESCRIPTION
 * Perform a simple difference between two RINEX OBS files.
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-t, \--time=\argarg{TIME}}
 * \dicdef{Start of time range to compare (default = "beginning of time")}
 * \dicterm{-e, \--end-time=\argarg{TIME}}
 * \dicdef{End of time range to compare (default = "end of time")}
 * \dicterm{-p, \--precision=\argarg{ARG}}
 * \dicdef{Limit data comparison to \argarg{ARL} decimal places. Default = 5}
 * \enddictionary
 *
 * Time may be specified in one of three formats:
 * - month/day/year
 * - year day-of-year
 * - year day-of-year seconds-of-day
 *
 * \section rowdiff_examples EXAMPLES
 *
 * \cmdex{rowdiff data/arlm200a.15o data/arlm200z.15o}
 *
 * Will show differences in individual records.  Records that appear
 * in the first file will start with "<" and records that appear in
 * the second file will start with ">".  Records with the same epoch
 * and satellite ID will be printed with a timestamp followed by the
 * differences between the two obs data records.
 *
 * \todo Add an example or two using the time options.
 *
 * \section rowdiff_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred\, or differences were encountered}
 * \dictentry{2,One or both of the input files does not exist}
 * \enddictable
 *
 * \section rowdiff_see_also SEE ALSO
 * \ref rnwdiff, \ref rmwdiff
 */

/// This utility assumes that epochs are in ascending time order

#include "NewNavInc.h"
#include <gnsstk/FileFilterFrameWithHeader.hpp>
#include <gnsstk/Rinex3ObsStream.hpp>
#include <gnsstk/Rinex3ObsFilterOperators.hpp>
#include <gnsstk/TimeString.hpp>

#include "DiffFrame.hpp"

#include <gnsstk/YDSTime.hpp>

using namespace std;
using namespace gnsstk;

class ROWDiff : public DiffFrame
{
public:
      /// Input file does not exist exit code
   static const int EXIST_ERROR = 2;
      /// Differences found in input files
   static const int DIFFS_CODE = 1;
   ROWDiff(char* arg0)
         : DiffFrame(arg0, std::string("RINEX Obs")),
           precisionOption('p',"precision","Limit data comparison to n decimal"
                           " places. Default = 5")
   {}
   virtual bool initialize(int argc, char* argv[]) noexcept;

protected:
   virtual void process();
   gnsstk::CommandOptionWithAnyArg precisionOption;

private:
   int precision;
   static const int DEFAULT_PRECISION = 5;
};

bool ROWDiff::initialize(int argc, char* argv[]) noexcept
{
   if (!DiffFrame::initialize(argc, argv))
   {
      return false;
   }
   if (precisionOption.getCount())
   {
      precision = atoi(precisionOption.getValue()[0].c_str());
   }
   else
   {
      precision = DEFAULT_PRECISION;
   }
   return true;
}

void ROWDiff::process()
{
   gnsstk::FileFilterFrameWithHeader<Rinex3ObsStream, Rinex3ObsData, Rinex3ObsHeader>
      ff1(inputFileOption.getValue()[0]), ff2(inputFileOption.getValue()[1]);

      // no data?  FIX make this program faster.. if one file
      // doesn't exist, there's little point in reading any.
   if (ff1.emptyHeader())
   {
      cerr << "No header information for " << inputFileOption.getValue()[0]
           << endl;
   }
   if (ff2.emptyHeader())
   {
      cerr << "No header information for " << inputFileOption.getValue()[1]
           << endl;
   }
   if (ff1.emptyHeader() || ff2.emptyHeader())
   {
      cerr << "Check that files exist." << endl;
      cerr << "diff failed." << endl;
      exitCode = EXIST_ERROR;
      return;
   }

      // determine whether the two input files have the same observation types

   Rinex3ObsHeader header1, header2;
   Rinex3ObsStream ros1(inputFileOption.getValue()[0]),
      ros2(inputFileOption.getValue()[1]);

   ros1 >> header1;
   ros2 >> header2;

      // find the obs data intersection

   if (header1.version != header2.version)
   {
      cout << "File 1 and file 2 are not the same RINEX version" << endl;
         // Reading a R2 file in translates/guesses its obsTypes into
         // R3-style obsIDs, but translating the R3 obsIDs to R2 is
         // more likely to match.  So map R3 -> R2 then change the R2
         // header to match.
      if (header1.version < 3 && header2.version >= 3)
      {
         header2.prepareVer2Write();
         Rinex3ObsHeader::RinexObsVec r3ov;
         for (const auto& r2it : header1.R2ObsTypes)
         {
            r3ov.push_back(header2.mapSysR2toR3ObsID["G"][r2it]);
         }
         header1.mapObsTypes["G"] = r3ov;
         ff1.frontHeader().mapObsTypes["G"] = r3ov;
      }
      else if (header2.version < 3 && header1.version >= 3)
      {
         header1.prepareVer2Write();
         Rinex3ObsHeader::RinexObsVec r3ov;
         for (const auto& r2it : header2.R2ObsTypes)
         {
            r3ov.push_back(header1.mapSysR2toR3ObsID["G"][r2it]);
         }
         header2.mapObsTypes["G"] = r3ov;
         ff2.frontHeader().mapObsTypes["G"] = r3ov;
      }
   }

      // Find out what obs IDs header 1 has that header 2 does/ doesn't have
      // add those to intersectionRom/ diffRom respectively.
   cout << "Comparing the following fields:" << endl;
   Rinex3ObsHeader::RinexObsMap diffRom;
   Rinex3ObsHeader::RinexObsMap intersectRom;
   for (const auto& mit : header1.mapObsTypes)
   {
      string sysChar = mit.first;
      cout << sysChar << ": ";
      for (const auto& id1 : mit.second)
      {
         try
         {
            header2.getObsIndex(sysChar, id1);
            intersectRom[sysChar].push_back(id1);
            cout << " " << id1.asString();
         }
         catch(...)
         {
            diffRom[sysChar].push_back(id1);
         }
      }
      cout << endl;
   }

      // Find out what obs IDs header 2 has that header 1 doesn't
      // and add them to diffRom
   for (const auto& mit : header2.mapObsTypes)
   {
      string sysChar = mit.first;
      for (const auto& id2 : mit.second)
      {
         try
         {
            header1.getObsIndex(sysChar, id2);
         }
         catch(...)
         {
            diffRom[sysChar].push_back(id2);
         }
      }
   }

      // Print out the differences between the obs IDs in header1 and header2
   if (!diffRom.empty())
   {
      cout << "Ignoring unshared obs:" << endl;
      for (const auto& mit : diffRom)
      {
         string sysChar = mit.first;
         cout << sysChar << ": ";
         for (const auto& id : mit.second)
         {
            cout << id.asString() << " ";
         }
         cout << endl;
      }
   }

   std::list<Rinex3ObsData> a =
      ff1.halfDiff(ff2,Rinex3ObsDataOperatorLessThanFull(intersectRom),
                   precision);
   std::list<Rinex3ObsData> b =
      ff2.halfDiff(ff1, Rinex3ObsDataOperatorLessThanFull(intersectRom),
                   precision);

   pair< list<Rinex3ObsData>, list<Rinex3ObsData> > difflist =
      pair< list<Rinex3ObsData>, list<Rinex3ObsData> >( a, b);

   if (difflist.first.empty() && difflist.second.empty())
   {
         //Indicate to the user, before exiting, that rowdiff
         //performed properly and no differences were found.
      cout << "For the observation types that were compared, "
           << "no differences were found." << endl;
      exitCode = 0;
      return;
   }

      // differences found
   exitCode = DIFFS_CODE;

   auto firstDiffItr = difflist.first.begin();
   auto secondDiffItr = difflist.second.begin();
   while ((firstDiffItr != difflist.first.end()) ||
          (secondDiffItr != difflist.second.end()))
   {
         //Epoch in both files
         //Epoch only in first file
      if ((firstDiffItr != difflist.first.end()) &&
          ((secondDiffItr == difflist.second.end()) ||
           (firstDiffItr->time < secondDiffItr->time)))
      {
         for (const auto& firstObsItr : firstDiffItr->obs)
         {
            cout << "<" << setw(3) << (static_cast<YDSTime>(firstDiffItr->time))
                 << ' ' << setw(2) << firstObsItr.first << ' ';
            string sysString = string(1,firstObsItr.first.systemChar());
            for (const auto& romIt : intersectRom[sysString])
            {
               size_t idx = header1.getObsIndex(sysString, romIt);
               cout << setw(15) << setprecision(3) << fixed
                    << firstObsItr.second[idx].data << ' ' << romIt.asString()
                    << ' ';
            }
            cout << endl;
         }
         firstDiffItr++;
      }
         //Epoch only in second file
      else if ((secondDiffItr != difflist.second.end()) &&
               ((firstDiffItr == difflist.first.end()) ||
                (secondDiffItr->time < firstDiffItr->time)))
      {
         for (const auto& secondObsItr : secondDiffItr->obs)
         {
            cout << ">" << setw(3)
                 << (static_cast<YDSTime>(secondDiffItr->time))
                 << ' ' << setw(2) << secondObsItr.first << ' ';
            string sysString = string(1,secondObsItr.first.systemChar());
            for (const auto& romIt : intersectRom[sysString])
            {
               size_t idx = header2.getObsIndex(sysString, romIt);
               cout << setw(15) << setprecision(3) << fixed
                    << secondObsItr.second[idx].data << ' '
                    << romIt.asString() << ' ';
            }
            cout << endl;
         }
         secondDiffItr++;
      }
      else if (firstDiffItr->time == secondDiffItr->time)
      {
         auto firstObsItr = firstDiffItr->obs.begin();
         auto secondObsItr = secondDiffItr->obs.begin();
            // For each satellite
         while ((firstObsItr != firstDiffItr->obs.end()) ||
                (secondObsItr != secondDiffItr->obs.end()))
         {
               // Both files have data for that satellite
            if (firstObsItr->first == secondObsItr->first)
            {
               string sysString = string(1,firstObsItr->first.systemChar());
               cout << "-" << setw(3)
                    << (static_cast<YDSTime>(firstDiffItr->time))
                    << ' ' << setw(2) << firstObsItr->first << ' ';
               for (const auto& romIt : intersectRom[sysString])
               {
                  size_t idx1 = header1.getObsIndex(sysString, romIt);
                  size_t idx2 = header2.getObsIndex(sysString, romIt);
                  cout << setw(15) << setprecision(3) << fixed
                       << (firstObsItr->second[idx1].data -
                           secondObsItr->second[idx2].data)
                       << ' ' << romIt.asString() << ' ';
               }
               firstObsItr++;
               secondObsItr++;
            }
               // Only file 1 has data for that satellite
            else if ((firstObsItr != firstDiffItr->obs.end()) &&
                     ((secondObsItr == secondDiffItr->obs.end()) ||
                      (firstObsItr->first.id < secondObsItr->first.id)))
            {
               string sysString = string(1,firstObsItr->first.systemChar());
               cout << "<" << setw(3)
                    << (static_cast<YDSTime>(firstDiffItr->time))
                    << ' ' << setw(2) << firstObsItr->first << ' ';
               for (const auto& romIt : intersectRom[sysString])
               {
                  size_t idx = header1.getObsIndex(sysString, romIt);
                  cout << setw(15) << setprecision(3) << fixed
                       << firstObsItr->second[idx].data << ' '
                       << romIt.asString() << ' ';
               }
               firstObsItr++;
            }
               // Only file 2 has data for that satellite
            else if (secondObsItr != secondDiffItr->obs.end())
            {
               string sysString = string(1,secondObsItr->first.systemChar());
               cout << ">" << setw(3)
                    << (static_cast<YDSTime>(secondDiffItr->time))
                    << ' ' << setw(2) << secondObsItr->first << ' ';
               for (const auto& romIt : intersectRom[sysString])
               {
                  size_t idx = header2.getObsIndex(sysString, romIt);
                  cout << setw(15) << setprecision(3) << fixed
                       << secondObsItr->second[idx].data << ' '
                       << romIt.asString() << ' ';
               }
               secondObsItr++;
            }
            cout << endl;
         }

         firstDiffItr++;
         secondDiffItr++;
      }
   }
}

int main(int argc, char* argv[])
{
#include "NewNavInit.h"
   try
   {
      ROWDiff m(argv[0]);
      if (!m.initialize(argc, argv))
         return m.exitCode;
      if (!m.run())
         return m.exitCode;

      return m.exitCode;
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
      cout << "unknown error" << endl;
   }
      // only reach this point if an exception was caught
   return BasicFramework::EXCEPTION_ERROR;
}
