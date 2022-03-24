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

#include "NewNavInc.h"
#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/RinexNavDataFactory.hpp>
#include <gnsstk/CommandOptionWithTimeArg.hpp>
#include <gnsstk/SystemTime.hpp>
#include <gnsstk/TimeString.hpp>

using namespace std;
using namespace gnsstk;

class RinEditNav : public BasicFramework
{
public:
      /// Nav stream shared (managed) pointer.
   using StrmPtr = shared_ptr<Rinex3NavStream>;
      /// Map system to output stream.
   using OutputMap = map<SatelliteSystem, StrmPtr>;

      /// RINEX version to use when writing out RINEX 2 format.
   static constexpr double DefRINEX2Ver = 2.11;

      /// Minimal data for uniquely identifying nav data for the output file
   struct UniqueNav
   {
      UniqueNav()
            : PRNID(-1)
      {}
      UniqueNav(const Rinex3NavData& rnd)
            : time(rnd.time), satSys(rnd.satSys), PRNID(rnd.PRNID)
      {
            // Kludge to make everything in the same time system.
            // This isn't truly valid, but it should be in most cases.
         time.setTimeSystem(TimeSystem::Any);
      }
      bool operator<(const UniqueNav& right) const
      {
         if (time < right.time) return true;
         if (right.time < time) return false;
         if (satSys < right.satSys) return true;
         if (right.satSys < satSys) return false;
         if (PRNID < right.PRNID) return true;
         return false;
      }
      CommonTime time;     ///< Time according to the sat/epoch record (TOC)
      std::string satSys;  ///< Satellite system of Epoch: G,R,E,S,C
      short PRNID;         ///< SV PRN ID
   };

   RinEditNav(const string& applName);

   bool initialize(int argc, char *argv[], bool pretty=true) throw() override;

   void process();

      /** Parse the RINEX 3 or RINEX 2 output options.
       * @param[in] opt The command line option to process output file
       *   names from.
       * @param[out] outMap Where the opened streams will be stored.
       * @param[in,out] outFNs Used to make sure we don't attempt to
       *   open a file more than once.
       * @return true on success, false on failure. */
   bool parseOut(const CommandOptionWithAnyArg& opt, OutputMap& outMap,
                 set<string>& outFNs);

      /// Write the headers for all open output streams.  Merges input headers.
   void writeHeaders();

      /// Create the mappings from system string to enum.
   void makeSysMappings();

      /// Command option for start time for matching
   CommandOptionWithTimeArg timeOpt;
      /// Command option for end time for matching
   CommandOptionWithTimeArg eTimeOpt;
      /// Command option for excluding specific satellites
   CommandOptionWithAnyArg excludeOpt;
      /// Command option for output file (RINEX 3)
   CommandOptionWithAnyArg output3Opt;
      /// Command option for output file (RINEX 2)
   CommandOptionWithAnyArg output2Opt;
      /// Make sure at least one of the output options has been used
   CommandOptionOneOf outputOpts;
      /// Command option for input files
   CommandOptionRest filesOpt;
      /// Map stream to data
   map<StrmPtr, Rinex3NavData> dataMap;
      /// Map time stamp of record in dataMap to stream
   map<CommonTime, StrmPtr> orderMap;
      /// Map satellite system to RINEX 3 output stream
   OutputMap output3Map;
      /// Map satellite system to RINEX 2 output stream
   OutputMap output2Map;
      /// Excluded satellites
   set<SatID> exclSats;
      /// First time to be included from input data
   CommonTime minTime;
      /// First time to NOT be included from input data
   CommonTime maxTime;
      /// Single character RINEX system IDs
   map<string,SatelliteSystem> id1map;
      /// Three character RINEX system IDs
   map<string,SatelliteSystem> id3map;
      /// Keep track of written data for removing duplicates.
   set<UniqueNav> uniques;
};


RinEditNav ::
RinEditNav(const string& applName)
      : BasicFramework(applName, "reads one or more RINEX (v.2+) navigation"
                       " files and writes the merged navigation data to one or"
                       " more output (ver 2 or 3) files. A summary of the"
                       " ephemeris data may be written to the screen."),
        timeOpt('t', "time", "%Y %m %d %H %M %S", "Start time (of data) for"
                " processing (time in RINEX 3 format, i.e. \"YYYY mm dd HH MM"
                " SS\")"),
        eTimeOpt('e', "end-time", "%Y %m %d %H %M %S", "End time (of data) for"
                 " processing"),
        excludeOpt('x', "exclude", "Exclude satellite [system] from output"
                   " [e.g. G17,R]"),
        output3Opt('o', "out", "Output (ARG=[sys@]filename) to RINEX ver. 3"
                   " file fn [repeatable]"),
        output2Opt('2', "out2", "Output (ARG=[sys@]filename) to RINEX ver. 2"
                   " file fn [repeatable]"),
        filesOpt("RINEX-NAV-FILE [...]", true),
        minTime(CommonTime::BEGINNING_OF_TIME),
        maxTime(CommonTime::END_OF_TIME)
{
   timeOpt.setMaxCount(1);
   eTimeOpt.setMaxCount(1);
   outputOpts.addOption(&output2Opt);
   outputOpts.addOption(&output3Opt);
}


bool RinEditNav ::
initialize(int argc, char *argv[], bool pretty) throw()
{
   if (!BasicFramework::initialize(argc, argv, pretty))
      return false;

   makeSysMappings();

   if (timeOpt.getCount() > 0)
   {
      minTime = timeOpt.getTime()[0];
   }
   if (eTimeOpt.getCount() > 0)
   {
      maxTime = eTimeOpt.getTime()[0];
   }
   minTime.setTimeSystem(TimeSystem::Any);
   maxTime.setTimeSystem(TimeSystem::Any);

   vector<string> files(filesOpt.getValue());
   set<string> fileSet; // for uniqueness
      // Open all requested files and read the first record so we can
      // process the data in time order (this assumes the individual
      // files are in time order to begin with).
   for (unsigned i = 0; i < files.size(); i++)
   {
      if (fileSet.count(files[i]) > 0)
      {
         cerr << "\"" << files[i]
              << "\" specified multiple times, only using once" << endl;
         continue;
      }
      fileSet.insert(files[i]);
      StrmPtr sp = make_shared<Rinex3NavStream>(files[i].c_str(),std::ios::in);
      if (!(*sp))
      {
         cerr << "Unable to open \"" << files[i] << "\" for input" << endl;
         exitCode = BasicFramework::EXIST_ERROR;
         return false;
      }
      sp->exceptions(std::fstream::failbit);
         // read the first data record of the file into the map
      (*sp) >> dataMap[sp];
         // also save the time stamp so we always have everything in order
      orderMap[dataMap[sp].time] = sp;
   }

   set<string> outFNs;
   if (!parseOut(output3Opt, output3Map, outFNs))
   {
      return false;
   }
   if (!parseOut(output2Opt, output2Map, outFNs))
   {
      return false;
   }

   vector<string> exsats(excludeOpt.getValue());
   for (unsigned i = 0; i < exsats.size(); i++)
   {
      try
      {
         RinexSatID sid(exsats[i]);
         if (sid.id == -1)
         {
               // If you don't specify a satellite ID, only a system,
               // you get a satellite ID of -1.  Turn that into a
               // wildcard.
            sid.wildId = true;
         }
         exclSats.insert(sid);
      }
      catch (...)
      {
         cerr << "\"" << exsats[i] << "\" is not a valid satellite designation"
              << endl;
         exitCode = BasicFramework::OPTION_ERROR;
         return false;
      }
   }
   
   return true;
}   


void RinEditNav ::
process()
{
   bool done = false;
   writeHeaders();
   while (!done)
   {
      if (orderMap.empty())
      {
            // nothing more to process
         break;
      }
         // first element of orderMap is always the oldest
      auto omi = orderMap.begin();
      StrmPtr sp = omi->second;
         // write the already-read data record to output
      if ((omi->first >= minTime) && (omi->first < maxTime) &&
          (exclSats.count(dataMap[sp].sat) == 0))
      {
         UniqueNav key(dataMap[sp]);
         if (uniques.count(key) == 0)
         {
            uniques.insert(key);
            for (auto o3i : output3Map)
            {
                  // Treat "Unknown" as a wildcard here
               if ((o3i.first == SatelliteSystem::Unknown) ||
                   (o3i.first == dataMap[sp].sat.system))
               {
                  (*o3i.second) << dataMap[sp];
               }
            }
            for (auto o2i : output2Map)
            {
                  // Treat "Unknown" as a wildcard here
               if ((o2i.first == SatelliteSystem::Unknown) ||
                   (o2i.first == dataMap[sp].sat.system))
               {
                  (*o2i.second) << dataMap[sp];
               }
            }
         }
      }
         // read the next record in the file and update orderMap;
      orderMap.erase(omi);
      (*sp) >> dataMap[sp];
      if (*sp)
      {
            // only add the record if it's valid.
         orderMap[dataMap[sp].time] = sp;
      }
   }
}


bool RinEditNav ::
parseOut(const CommandOptionWithAnyArg& opt, OutputMap& outMap,
         set<string>& outFNs)
{
   vector<string> outs(opt.getValue());
   for (unsigned i = 0; i < outs.size(); i++)
   {
         // sysFn[0] = system, sysFn[1] = file name
      vector<string> sysFn;
      SatelliteSystem sys;
      if (outs[i].find('@') != string::npos)
      {
         sysFn = gnsstk::StringUtils::split(outs[i], '@');
         string sysUp = sysFn[0];
         StringUtils::upperCase(sysUp);
         if (id1map.find(sysUp) != id1map.end())
         {
            sys = id1map[sysUp];
         }
         else if (id3map.find(sysUp) != id3map.end())
         {
            sys = id3map[sysUp];
         }
         else
         {
            sys = gnsstk::StringUtils::asSatelliteSystem(sysFn[0]);
         }
         if (sys == SatelliteSystem::Unknown)
         {
            cerr << "\"" << sysFn[0] << "\" is not a valid system ID" << endl;
            exitCode = BasicFramework::OPTION_ERROR;
            return false;
         }
      }
      else
      {
         sysFn.resize(2);
         sysFn[1] = outs[i];
         sys = SatelliteSystem::Unknown;
      }
      if (outFNs.count(sysFn[1]) > 0)
      {
         cerr << "Can't open \"" << sysFn[1] << "\" for output more than once"
              << endl;
         exitCode = BasicFramework::OPTION_ERROR;
         return false;
      }
      outFNs.insert(sysFn[1]);
      StrmPtr sp = make_shared<Rinex3NavStream>(sysFn[1].c_str(),std::ios::out);
      sp->exceptions(std::fstream::failbit);
      outMap[sys] = sp;
   }
   return true;
}


void RinEditNav ::
writeHeaders()
{
   bool firstRec = true;
   Rinex3NavHeader merged;
   merged.fileType = "N: GNSS NAV DATA";
   merged.fileSys = "";
   merged.fileProgram = argv0;
   merged.fileAgency = "gnsstk";
   merged.date = printTime(SystemTime(),"%Y%02m%02d %02H%02M%02S %P");
   merged.valid = Rinex3NavHeader::validVersion | Rinex3NavHeader::validRunBy |
      Rinex3NavHeader::validEoH;
   size_t pos = merged.fileProgram.find_last_of("/\\");
   if (pos != string::npos)
      merged.fileProgram.erase(0,pos+1);
   SatelliteSystem mergedSys = SatelliteSystem::Last;
   for (auto dmi : dataMap)
   {
      if (firstRec &&
          (dmi.first->header.valid & Rinex3NavHeader::validLeapSeconds))
      {
            // Just pull these things from the first header, which is
            // really the best we can do.
         merged.leapSeconds = dmi.first->header.leapSeconds;
         merged.leapDelta = dmi.first->header.leapDelta;
         merged.leapWeek = dmi.first->header.leapWeek;
         merged.leapDay = dmi.first->header.leapDay;
         merged.valid |= Rinex3NavHeader::validLeapSeconds;
         firstRec = false;
      }
         // merge comments
      for (unsigned i = 0; i < dmi.first->header.commentList.size(); i++)
      {
         merged.commentList.push_back(dmi.first->header.commentList[i]);
         merged.valid |= Rinex3NavHeader::validComment;
      }
         // merge time system corrections
      for (const auto& i : dmi.first->header.mapTimeCorr)
      {
         merged.mapTimeCorr[i.first] = i.second;
         merged.valid |= Rinex3NavHeader::validTimeSysCorr;
      }
         // merge iono corrections
      for (const auto& i : dmi.first->header.mapIonoCorr)
      {
         merged.mapIonoCorr[i.first] = i.second;
         merged.valid |= (
            dmi.first->header.valid & (Rinex3NavHeader::validIonoCorrGPS |
                                       Rinex3NavHeader::validIonoCorrGal));
      }
         // attempt to determine the system or mixed from the input
      SatelliteSystem guessedSys;
      if (dmi.first->header.version < 3.0)
      {
            // try and pick up the system from the data
         guessedSys = dmi.second.sat.system;
      }
      else
      {
            // pick up the system from the input header for 3.0+
         guessedSys = dmi.first->header.fileSysSat.system;
      }
      if (mergedSys == SatelliteSystem::Last)
      {
         mergedSys = guessedSys;
      }
      else if (mergedSys != guessedSys)
      {
         mergedSys = SatelliteSystem::Mixed;
      }
   }
   for (auto& i : output3Map)
   {
      merged.version = Rinex3ObsBase::currentVersion;
      RinexSatID sat;
         // If the map key is "Unknown", that means there's no
         // restriction on which systems go in the output file, so use
         // our best guess.  Otherwise, we know the file will only
         // contain the one system so use that instead.
      if (i.first == SatelliteSystem::Unknown)
      {
         sat.system = mergedSys;
      }
      else
      {
         sat.system = i.first;
      }
      merged.fileSys = string(1, sat.systemChar()) + ": " +
         sat.systemString();
      (*i.second) << merged;
   }
   for (auto& i : output2Map)
   {
      merged.version = DefRINEX2Ver;
      (*i.second) << merged;
   }
}


void RinEditNav ::
makeSysMappings()
{
      // This is a pretty clunky way to generate the mappings but this
      // was the only way I found to get system mappings for RINEX.
   RinexSatID sat;
   for (SatelliteSystem sys : SatelliteSystemIterator())
   {
      sat.system = sys;
      string sysStr = string(1, sat.systemChar());
      if (sysStr != "?")
         id1map[sysStr] = sys;
      sysStr = sat.systemString3();
      if (sysStr != "Unk")
         id1map[sysStr] = sys;
   }
}


int main(int argc, char *argv[])
{
#include "NewNavInit.h"
   try
   {
      RinEditNav app(argv[0]);
      if (!app.initialize(argc, argv))
         return app.exitCode;
      app.run();
      return app.exitCode;
   }
   catch (Exception& e)
   {
      cerr << e << endl;
   }
   catch (exception& e)
   {
      cerr << e.what() << endl;
   }
   catch (...)
   {
      cerr << "unknown exception" << endl;
   }
   return BasicFramework::EXCEPTION_ERROR;
}
