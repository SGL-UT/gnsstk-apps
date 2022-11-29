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
#include <gnsstk/NavLibrary.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>
#include <gnsstk/TimeString.hpp>
#include <gnsstk/CommandOptionNavEnumHelp.hpp>
#include <gnsstk/NavTimeSystemConverter.hpp>
#include <gnsstk/CGCS2000Ellipsoid.hpp>
#include <gnsstk/GalileoEllipsoid.hpp>
#include <gnsstk/GPSEllipsoid.hpp>
#include <gnsstk/PZ90Ellipsoid.hpp>
#include <gnsstk/WGS84Ellipsoid.hpp>
// I'm leaving this code in here for now, but obscured via these
// preprocessor macros.  While most of the changes made recently were
// in fact for debugging, I'm not convinced these changes should be a
// permanent change to the behavior.  That said, it could provide more
// detail in the output, e.g. the full metadata of the nav message
// that was found and used for Xvt computation, currently not
// available (only the search key is output).
#ifdef DEBUG_NAVFIT
#include <gnsstk/OrbitData.hpp> // debug code
#include <gnsstk/NavFit.hpp> // debug code
#include <gnsstk/GLOCNavEph.hpp> // debug code
#endif
#include <gnsstk/DebugTrace.hpp>

using namespace std;
using namespace gnsstk;

/// Storage for find parameters
class FindParams
{
public:
   FindParams()
         : xmitHealth(SVHealth::Any), valid(NavValidityType::Any),
           order(NavSearchOrder::User), tgtSys(TimeSystem::Any)
   {
   }
      /** Perform a find on the given NavLibrary with the search
       * parameters stored internally.
       * @param[in] navLib The NavLibrary to retrieve the data from.
       * @param[out] ndp The NavDataPtr, if found.
       * @return true if successful. */
   bool find(std::shared_ptr<NavLibrary>& navLib, NavDataPtr& ndp) const;
      /** Compute a satellite XVT using the given NavLibrary with
       * the search parameters stored internally.
       * @param[in] navLib The NavLibrary to retrieve the data from.
       * @param[in] useAlm If true, use almanac, if false, use ephemeris.
       * @param[out] xvt The computed Xvt, if successful.
       * @return true if successful. */
   bool getXvt(std::shared_ptr<NavLibrary>& navLib, bool useAlm, Xvt& xvt)
      const;
      /** Decode the given string into this object.
       * @param[in] s The encoded set of find parameters as a string.
       * @param[in] warnNMT If true, warn on unknown nav message type.
       * @param[out] warnings Any warning messages regarding the
       *   decoding of the string.  If this string is not empty on return, this
       *   object's contents should be considered unusable. */
   void decode(const std::string& s, bool warnNMT, std::string& warnings);
      /** Subject satellite, transmit satellite, obs ID (obs type=nav,
       * carrier band, tracking code, xmit antenna, freq offset), nav
       * type, nav msg type */
   NavMessageID nmid;
      /// Time of interest for query
   CommonTime when;
      /// Target time system to convert to before doing query
   TimeSystem tgtSys;
      /// Desired health status of transmitting satellite
   SVHealth xmitHealth;
      /// Desired validity of the nav message
   NavValidityType valid;
      /// Find behavior, user or nearest.
   NavSearchOrder order;
};


ostream& operator<<(ostream& s, const FindParams& fp)
{
   s << "  " << fp.nmid << endl
     << "  " << printTime(fp.when, "%4Y/%03j/%02H:%02M:%015.12f") << endl
     << "  " << StringUtils::asString(fp.xmitHealth) << endl
     << "  " << StringUtils::asString(fp.valid) << endl;
   switch (fp.order)
   {
      case NavSearchOrder::User:
         s << "  User" << endl;
         break;
      case NavSearchOrder::Nearest:
         s << "  Nearest" << endl;
         break;
      default:
         s << "  Unknown search order" << endl;
         break;
   }
   return s;
}


/// Get command-line option help for NavMessageType strings.
class CommandOptionHelpNavMessageType : public CommandOptionHelp
{
public:
      /** Constructor.
       * @param[in] shOpt The one character command line option.
       *   Set to 0 if unused.
       * @param[in] loOpt The long command option.  Set to
       *   std::string() if unused.
       * @param[in] desc A string describing what this option does.
       */ 
   CommandOptionHelpNavMessageType(const char shOpt,
                                   const std::string& loOpt,
                                   const std::string& desc);

      /** Print the requested help information.
       * @param[in] out The stream to which the help text will be printed.
       * @param[in] pretty Unused in this child class. */
   void printHelp(std::ostream& out, bool pretty = true) override;
};


/// Command-line option specifically for NavLibrary queries.
class CommandOptionNavLibraryFind : public CommandOptionWithAnyArg
{
public:
      /** Constructor.
       * @param[in] shOpt The one character command line option.
       *   Set to 0 if unused.
       * @param[in] loOpt The long command option.  Set to
       *   std::string() if unused.
       * @param[in] desc A string describing what this option does.
       * @param[in] warnUnknownNMT If true, a warning will be printed
       *   for any unknown nav message type.  If false, a warning will
       *   NOT be printed if the literal "Unknown" is specified for
       *   nav message type.
       */ 
   CommandOptionNavLibraryFind(const char shOpt,
                               const std::string& loOpt,
                               const std::string& desc,
                               bool warnUnknownNMT = true);

      /// Validate the input arguments.
   std::string checkArguments() override;

      /// The actual parameters as interpreted by above.
   std::vector<FindParams> params;

      /// Indicate whether to warn on unknown nav message type.
   bool warnNMT;
};

/** \page apps
 * - \subpage navdump - Dumping any and all navigation data.
 * \page navdump
 * \tableofcontents
 *
 * \section navdump_name NAME
 * navdump - Dump the processed contents of navigation data files
 *
 * \section navdump_synopsis SYNOPSIS
 * <b>navdump</b>  <b>-h</b> <br/>
 * <b>navdump</b>  <b>-E</b>&nbsp;\argarg{ARG} <br/>
 * <b>navdump</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-t</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-l</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-F</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-X</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--xvt-file</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-g</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--to-file</b>&nbsp;\argarg{ARG}<b>]</b> \argarg{ARG} <b>[</b>...<b>]</b>
 *
 * \section navdump_description DESCRIPTION
 * Print (to standard output) the parsed contents of any number of
 * GNSS navigation data files.
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-t, \--type=\argarg{ARG}}
 * \dicdef{Navigation data type(s) to dump (use "-E navmsgtype" for list, default=all types)}
 * \dicterm{-E, \--enum=\argarg{ARG}}
 * \dicdef{Get help for enums, specify one of system, obstype, carrier, range, antenna, navtype, navmsgtype, health, validity, order, detail}
 * \dicterm{-l, \--level=\argarg{ARG}}
 * \dicdef{Specify detail level of output (use "-E detail" for list, default=Full)}
 * \dicterm{-F, \--find=\argarg{ARG}}
 * \dicdef{Find a specific object in the input matching 
"navmsgtype YYYY/DOY/HH:MM:SS.SSS[/SYS/TGT] subject-sat-id system [carrier [range [navtype [xmit-health [validity-type [xmit-sat-id system]]]]]]"
(optional time systems SYS and TGT are used to convert the specified time system from SYS to TGT before look-up; navmsgtype must be Ephemeris, Almanac or Unknown for Almanac-if-no-Ephemeris)}
 * \dicterm{-X, \--xvt=\argarg{ARG}}
 * \dicdef{Compute a satellite XVT using nav data matching 
"navmsgtype YYYY/DOY/HH:MM:SS.SSS[/SYS/TGT] subject-sat-id system [carrier [range [navtype [xmit-health [validity-type [xmit-sat-id system]]]]]]"
(optional time systems SYS and TGT are used to convert the specified time system from SYS to TGT before look-up; navmsgtype must be Ephemeris, Almanac or Unknown for Almanac-if-no-Ephemeris)}
 * \dicterm{\--xvt-file=\argarg{ARG}}
 * \dicdef{Compute a satellite XVT using nav data matching the specified file's contents.  The file should contain the same format specification as --xvt, one per line}
 * \dicterm{-g, \--geodetic=\argarg{ARG}}
 * \dicdef{Print position in geodetic coordinates in addition to an XVT in ECEF, using the specified ellipsoid model (WGS84, GPS, PZ90, Galileo, CGCS2000)}
 * \dicterm{\--to-file=\argarg{ARG}}
 * \dicdef{File containing nav data to be loaded for the sole purpose of providing time offsets when doing time system conversions during look-ups for everything else.  Only makes sense to use with -F -X or --xvt-file when a time system is specified.}
 * \enddictionary
 *
 * \section navdump_examples EXAMPLES
 *
 * \cmdex{navdump data/arlm200a.15n}
 *
 * Will print the full details of the processed data as derived from
 * the file arlm200a.15n.  This includes Ephemerides, Health,
 * Ionospheric corrections, Inter-signal corrections and time offset
 * data.
 *
 * \cmdex{navdump -l OneLine -t Ephemeris data/arlm200a.15n}
 *
 * Will print limited information about the ephemerides contained
 * within arlm200a.15n, one ephemeris per line of text.
 *
 * \section navdump_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors ocurred}
 * \dictentry{1,A C++ exception occurred}
 * \dictentry{2,One or both of the input files does not exist, or a bad command 
 *   line was used}
 * \enddictable
 */
class NavDump : public BasicFramework
{
public:
   NavDump(const string& applName);

   bool initialize(int argc, char *argv[], bool pretty=true) noexcept override;
   void process() override;
      /** Get and print the satellite Xvt given a set of search parameters.
       * @param[in] fp The set of search parameters specified either
       *   in a file or a command-line option.
       * @return true if successful. */
   bool getXvt(const FindParams& fp);

      /// Command-line option for specifying the type of data to dump.
   CommandOptionWithAnyArg typesOpt;
      /// Get a list of some other type of enum.
   CommandOptionNavEnumHelp enumHelpOpt;
      /// Specify the detail level.
   CommandOptionWithAnyArg detailOpt;
      /// All remaining command-line arguments are treated as input files.
   CommandOptionRest filesOpt;
      /// Get a specific object
   CommandOptionNavLibraryFind inqOpt;
      /// Get satellite XVT
   CommandOptionNavLibraryFind xvtOpt;
      /// Get satellite XVT as listed in a file
   CommandOptionWithAnyArg xvtFileOpt;
      /// Output positions in geodetic coordinates
   CommandOptionWithAnyArg geodOpt;
      /// Specify files to load ONLY for changing time systems.
   CommandOptionWithAnyArg toFileOpt;
      /// command option for specifying the location of the satellite metadata.
   CommandOptionWithAnyArg satMetaOpt;
      /// High level nav store interface.
   std::shared_ptr<NavLibrary> navLib;
      /// Time system offsets when requested.
   std::shared_ptr<TimeSystemConverter> tsc;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
      /// Dump output level of detail
   DumpDetail detail;
      /// Ellipsoid to use when printing geodetic coordinates
   EllipsoidModel *ell;
      /// Nav message types requested, for filter optimization.
   NavMessageTypeSet nmts;
      /// Description of how to specify a nav message
   static const std::string specHelp;
      /// Storage for PRN<->SVN translation.
   SatMetaDataStore satMetaDataStore;
};


const std::string NavDump::specHelp(
   "\n\"navmsgtype YYYY/DOY/HH:MM:SS.SSS[/SYS/TGT] subject-sat-id"
   " system [carrier [range [navtype [xmit-health [validity-type"
   " [xmit-sat-id system]]]]]]\"\n(optional time systems SYS and TGT"
   " are used to convert the specified time system from SYS to TGT"
   " before look-up; navmsgtype must be Ephemeris,"
   " Almanac or Unknown for Almanac-if-no-Ephemeris)");


bool FindParams ::
find(std::shared_ptr<NavLibrary>& navLib, NavDataPtr& ndp) const
{
   CommonTime inqTime(when);
   if ((tgtSys != TimeSystem::Any) &&
       (tgtSys != inqTime.getTimeSystem()))
   {
      inqTime.changeTimeSystem(tgtSys);
   }
   return navLib->find(nmid, inqTime, ndp, xmitHealth, valid, order);
}


bool FindParams ::
getXvt(std::shared_ptr<NavLibrary>& navLib, bool useAlm, Xvt& xvt) const
{
   CommonTime inqTime(when);
   if ((tgtSys != TimeSystem::Any) &&
       (tgtSys != inqTime.getTimeSystem()))
   {
      inqTime.changeTimeSystem(tgtSys);
   }
   return navLib->getXvt(nmid, inqTime, xvt, useAlm, xmitHealth, valid, order);
}


void FindParams ::
decode(const std::string& s, bool warnNMT, std::string& warnings)
{
   std::vector<std::string> words = StringUtils::split(s);
   unsigned numWords = words.size();
   if (numWords < 4)
   {
      warnings += "Insufficient parameters to find: " + s;
      return;
   }
   nmid.messageType = StringUtils::asNavMessageType(words[0]);
   if (nmid.messageType == NavMessageType::Unknown)
   {
      if (warnNMT || (words[0] != "Unknown"))
      {
         cerr << "Warning: Unknown nav message type: \"" << words[0] << "\""
              << endl;
      }
   }
   try
   {
      int num = StringUtils::numWords(words[1], '/');
      string::size_type idx;
      string srcTime, tgtSysStr;
      switch (num)
      {
         case 3:
            gnsstk::scanTime(when, words[1], "%Y/%j/%H:%M:%f");
            when.setTimeSystem(TimeSystem::Any);
            break;
         case 5:
            idx = words[1].find_last_of('/');
            srcTime = words[1].substr(0, idx);
            tgtSysStr = words[1].substr(idx+1);
            gnsstk::scanTime(when, srcTime, "%Y/%j/%H:%M:%f/%P");
            tgtSys = gnsstk::StringUtils::asTimeSystem(tgtSysStr);
            break;
         default:
            warnings += "Improperly formatted time: " + words[1] +
               "\nUse %Y/%j/%H:%M:%f or %Y/%j/%H:%M:%f/%P";
            return;
      }
   }
   catch (gnsstk::Exception& exc)
   {
      warnings += exc.getText() + " (" + words[1] + ")";
      return;
   }
   nmid.sat.system = StringUtils::asSatelliteSystem(words[3]);
      // default the overall system to the subject system
      // (potentially change later to the transmit system)
   nmid.system = nmid.sat.system;
   if (nmid.sat.system == SatelliteSystem::Unknown)
   {
      cerr << "Warning: Unknown system \"" << words[3] << "\"" << endl;
   }
   if ((nmid.sat.system == SatelliteSystem::Glonass) &&
       (words[2].find('/') != std::string::npos))
   {
      std::vector<string> strid = StringUtils::split(words[2],'/');
      if (strid.size() != 2)
      {
         warnings += "Invalid GLONASS SLOT/CHANNEL: \"" + words[2] + "\"";
         return;
      }
      nmid.sat.id = StringUtils::asInt(strid[0]);
         /** @todo We can use 0 to represent a wildcard slot ID
          * which lets us match when using data that doesn't
          * include the slot ID, but we can't use 0 for a wildcard
          * channel because 0 is a valid channel.  Probably need to
          * figure out a way around this. */
      if (nmid.sat.id == 0)
      {
         nmid.sat.wildId = true;
      }
      if (strid[1] != "*")
      {
         nmid.obs.freqOffs = StringUtils::asInt(strid[1]);
         nmid.obs.freqOffsWild = false;
      }
   }
   else
   {
      nmid.sat.id = StringUtils::asInt(words[2]);
   }
   nmid.obs.type = ObservationType::NavMsg;
   nmid.obs.band = CarrierBand::Any;
   nmid.obs.code = TrackingCode::Any;
   nmid.xmitSat.makeWild();
   nmid.nav = NavType::Any;
   if (numWords > 4)
   {
      nmid.obs.band = StringUtils::asCarrierBand(words[4]);
      if (nmid.obs.band == CarrierBand::Unknown)
      {
         cerr << "Warning: Unknown carrier band \"" << words[4] << "\""
              << endl;
      }
   }
   if (numWords > 5)
   {
      nmid.obs.code = StringUtils::asTrackingCode(words[5]);
      if (nmid.obs.code == TrackingCode::Unknown)
      {
         cerr << "Warning: Unknown tracking code \"" << words[5] << "\""
              << endl;
      }
   }
   if (numWords > 6)
   {
      nmid.nav = StringUtils::asNavType(words[6]);
      if (nmid.nav == NavType::Unknown)
      {
         cerr << "Warning: Unknown nav type \"" << words[6] << "\""
              << endl;
      }
   }
   if (numWords > 7)
   {
      xmitHealth = StringUtils::asSVHealth(words[7]);
      if (xmitHealth == SVHealth::Unknown)
      {
         cerr << "Warning: Unknown health \"" << words[7] << "\""
              << endl;
      }
   }
   if (numWords > 8)
   {
      valid = StringUtils::asNavValidityType(words[8]);
      if (valid == NavValidityType::Unknown)
      {
         cerr << "Warning: Unknown validity type \"" << words[8] << "\""
              << endl;
      }
   }
   if (numWords == 10)
   {
      warnings += "xmit-sat-id also requires system to be specified";
      return;
   }
   if (numWords > 10)
   {
         // use SatID constructor to properly set wildcard flags
      nmid.xmitSat =
         SatID(StringUtils::asInt(words[9]),
               StringUtils::asSatelliteSystem(words[10]));
      if (nmid.xmitSat.system == SatelliteSystem::Unknown)
      {
         cerr << "Warning: Unknown system \"" << words[10] << "\"" << endl;
      }
   }
   if (numWords > 11)
   {
      warnings += "Extraneous text at end of \"" + s + "\"";
      return;
   }
}


CommandOptionNavLibraryFind ::
CommandOptionNavLibraryFind(const char shOpt,
                            const std::string& loOpt,
                            const std::string& desc,
                            bool warnUnknownNMT)
      : CommandOptionWithAnyArg(shOpt, loOpt, desc),
        warnNMT(warnUnknownNMT)
{
}


std::string CommandOptionNavLibraryFind ::
checkArguments()
{
   std::string rv;
   for (unsigned i = 0; i < value.size(); i++)
   {
      FindParams params1;
      params1.decode(value[i], warnNMT, rv);
      params.push_back(params1);
   }
   return rv;
}


NavDump ::
NavDump(const string& applName)
      : BasicFramework(applName, "Dump any and all navigation data (that we"
                       " support).  Debug will enable tracing (if tracing was"
                       " compiled into the libraries)\n\n"
                       "WARNING: The time system used for the find and xvt"
                       " options is whatever is used by the file store, and"
                       " should be taken into account when comparing results"),
        typesOpt('t', "type", "Navigation data type(s) to dump (use \"-E"
                 " navmsgtype\" for list, default=all types)"),
        detailOpt('l',"level","Specify detail level of output (use \"-E"
                  " detail\" for list, default=Full)"),
        inqOpt('F', "find", "Find a specific object in the input matching " +
               specHelp),
        xvtOpt('X', "xvt", "Compute a satellite XVT using nav data matching " +
               specHelp, false),
        xvtFileOpt(0, "xvt-file", "Compute a satellite XVT using nav data"
                   " matching the specified file's contents.  The file should"
                   " contain the same format specification as --xvt, one per"
                   " line"),
        geodOpt('g', "geodetic", "Print position in geodetic coordinates"
                " in addition to an XVT in ECEF, using the specified ellipsoid"
                " model (WGS84, GPS, PZ90, Galileo, CGCS2000)"),
        toFileOpt(0, "to-file", "File containing nav data to be loaded for the"
                  " sole purpose of providing time offsets when doing time"
                  " system conversions during look-ups for everything else."
                  "  Only makes sense to use with -F -X or --xvt-file when a"
                  " time system is specified."),
        satMetaOpt('M', "svconfig", "File containing satellite configuration"
                   " information for mapping SVN<->PRN"),
        enumHelpOpt('E', "enum"),
        detail(DumpDetail::Full),
        ell(nullptr),
        filesOpt("", true)
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   filesOpt.setDescription("NAV-FILE [...] (supported formats: " +
                           ndfp->getFactoryFormats() + ")");
   detailOpt.setMaxCount(1);
   geodOpt.setMaxCount(1);
}


bool NavDump ::
initialize(int argc, char *argv[], bool pretty) noexcept
{
   if (!BasicFramework::initialize(argc, argv, pretty))
      return false;
   if (debugLevel)
   {
      DEBUGTRACE_ENABLE();
      DEBUGTRACE_FUNCTION();
      ObsID::verbose = true; // show details when dumping
   }
   if (geodOpt.getCount())
   {
      string ellName(geodOpt.getValue()[0]);
      gnsstk::StringUtils::upperCase(ellName);
      if (ellName == "WGS84")
      {
         ell = new WGS84Ellipsoid();
      }
      else if (ellName == "GPS")
      {
         ell = new GPSEllipsoid();
      }
      else if ((ellName == "GALILEO") || (ellName == "GAL"))
      {
         ell = new GalileoEllipsoid();
      }
      else if ((ellName == "PZ90") || (ellName == "GLO"))
      {
         ell = new PZ90Ellipsoid();
      }
      else if ((ellName == "CGCS2000") || (ellName == "BDS"))
      {
         ell = new CGCS2000Ellipsoid();
      }
      else
      {
         cerr << "Unknown ellipsoid name: \"" << ellName << "\"" << endl;
         exitCode = BasicFramework::OPTION_ERROR;
         return false;
      }
   }
   navLib = make_shared<NavLibrary>();
   navLib->addFactory(ndfp);
      // set up time system conversion in case it's needed
   CommonTime::tsConv = make_shared<NavTimeSystemConverter>();
   NavTimeSystemConverter *ntsc;
   ntsc = dynamic_cast<NavTimeSystemConverter*>(CommonTime::tsConv.get());
   ntsc->navLib = navLib;
      // process nav message type command line arguments
   vector<string> types(typesOpt.getValue());
   for (unsigned i = 0; i < types.size(); i++)
   {
      NavMessageType nmt = StringUtils::asNavMessageType(types[i]);
      if (nmt == NavMessageType::Unknown)
      {
         cerr << "\"" << types[i] << "\" is not a valid nav message type"
              << endl;
         exitCode = BasicFramework::OPTION_ERROR;
         return false;
      }
      nmts.insert(nmt);
   }
      // set the NavLibrary's nav message types
      // But not if xvts are being requested in case a time conversion
      // is needed.
   if ((nmts.size() > 0) && (xvtOpt.getCount() == 0) &&
       (xvtFileOpt.getCount() == 0))
   {
      navLib->setTypeFilter(nmts);
   }
   else
   {
      nmts = allNavMessageTypes;
   }
      // set the output detail level
   if (detailOpt.getCount())
   {
      string detailStr = detailOpt.getValue()[0];
      detail = StringUtils::asDumpDetail(detailStr);
      if (detail == DumpDetail::Unknown)
      {
         cerr << "\"" << detailStr << "\" is not a valid detail level"
              << endl;
         exitCode = BasicFramework::OPTION_ERROR;
         return false;
      }
   }
   if (satMetaOpt.getCount())
   {
      for (unsigned i = 0; i < satMetaOpt.getCount(); i++)
      {
         if (!satMetaDataStore.loadData(satMetaOpt.getValue()[i]))
         {
            cerr << "Failed to load \"" << satMetaOpt.getValue()[i]
                 << "\"" << endl;
            return false;
         }
      }
   }
   gnsstk::NavData::satMetaDataStore = &satMetaDataStore;
   return true;
}


void NavDump ::
process()
{
   DEBUGTRACE_FUNCTION();
      // read in ephemeris data
   vector<string> names = filesOpt.getValue();
   for (size_t i=0; i<names.size(); i++)
   {
      if (!ndfp->addDataSource(names[i]))
      {
         cerr << "Unable to load \"" << names[i] << "\"" << endl;
         exitCode = BasicFramework::EXIST_ERROR;
         return;
      }
   }
      // load time offset data
   ndfp->setTypeFilter({gnsstk::NavMessageType::TimeOffset});
   names = toFileOpt.getValue();
   for (size_t i=0; i<names.size(); i++)
   {
      if (!ndfp->addDataSource(names[i]))
      {
         cerr << "Unable to load \"" << names[i] << "\"" << endl;
         exitCode = BasicFramework::EXIST_ERROR;
         return;
      }
   }
      // restore the type filter for delayed-action NavDataFactory classes.
   if (names.size() > 0)
   {
      nmts.insert(gnsstk::NavMessageType::TimeOffset);
   }
   ndfp->setTypeFilter(nmts);
      // Set to true if any look-up fails, for later dumping.
   bool notFound = false;
   if (inqOpt.params.size() > 0)
   {
      for (unsigned i = 0; i < inqOpt.params.size(); i++)
      {
         if (verboseLevel)
         {
            cout << "Looking for" << endl << inqOpt.params[i];
         }
         NavDataPtr ndp;
         if (inqOpt.params[i].find(navLib, ndp))
         {
            if (debugLevel)
            {
               cout << ndp->signal << endl;
            }
            ndp->dump(cout, detail);
         }
         else
         {
            cout << "Not found" << endl;
            notFound = true;
            exitCode = BasicFramework::EXIST_ERROR;
         }
      }
   }
   for (unsigned i = 0; i < xvtOpt.params.size(); i++)
   {
      if (!getXvt(xvtOpt.params[i]))
      {
         notFound = true;
      }
   } // for (unsigned i
   if (xvtFileOpt.getCount() > 0)
   {
      vector<string> xvtfiles(xvtFileOpt.getValue());
      for (unsigned i = 0; i < xvtfiles.size(); i++)
      {
         FindParams fp;
         string warnings, paramStr;
         ifstream is(xvtfiles[i].c_str());
         if (!is)
         {
            cerr << "Unable to open \"" << xvtfiles[i] << "\" for input"
                 << endl;
            if (exitCode == 0)
            {
               exitCode = BasicFramework::EXIST_ERROR;
            }
            continue;
         }
         while (is)
         {
            getline(is, paramStr);
            if (!is)
            {
               break;
            }
            if (paramStr.empty())
            {
               continue;
            }
            fp.decode(paramStr, false, warnings);
            if (!warnings.empty())
            {
               cerr << warnings << endl;
               return;
            }
            if (!getXvt(fp))
            {
               notFound = true;
            }
         }
      }
   }
   if ((inqOpt.params.empty() && xvtOpt.params.empty() &&
        (xvtFileOpt.getCount() == 0)) ||
       ((debugLevel > 0) && notFound))
   {
         // dump the processed results
      if (debugLevel)
      {
         cout << "Dumping navLib:" << endl;
      }
      navLib->dump(cout, detail);
   }
}


bool NavDump ::
getXvt(const FindParams& fp)
{
   bool found = true;
   if (verboseLevel)
   {
      cout << "Looking for" << endl << fp;
   }
   Xvt xvt;
   bool useAlm = false;
   NavDataPtr ndp; // temporary test code
   switch (fp.nmid.messageType)
   {
      case NavMessageType::Almanac:
         useAlm = true;
      case NavMessageType::Ephemeris:
#ifdef DEBUG_NAVFIT
            // this is temporary test code (see above)
         if (fp.find(navLib, ndp))
         {
            OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
            ObsID oid;
            if (orb->getXvt(fp.when, xvt, oid))
            {
               cout << fp.nmid
                    << gnsstk::printTime(fp.when, " %Y/%j/%02H:%02M:%015.12f %P ")
                    << setprecision(15) << xvt;
               if (ell != nullptr)
               {
                  Position coords(xvt.x, Position::Cartesian, ell, xvt.frame);
                  coords.transformTo(Position::Geodetic);
                  cout << " " << coords;
               }
               NavFit *nf = dynamic_cast<NavFit*>(ndp.get());
               if (nf != nullptr)
               {
                  if ((fp.when >= nf->beginFit) && (fp.when <= nf->endFit))
                  {
                     GLOCNavEph *ceph = dynamic_cast<GLOCNavEph*>(ndp.get());
                     if (ceph == nullptr)
                     {
                        cout << " FIT" << endl;
                     }
                     else
                     {
                        ceph->dump(std::cerr, gnsstk::DumpDetail::Full);
                        if (std::fabs(fp.when-ceph->Toe) <= 900)
                        {
                           cout << " FIT" << endl;
                        }
                        else
                        {
                           cout << " LONGTERM" << endl;
                        }
                     }
                  }
                  else
                  {
                        // shouldn't ever happen.
                     cout << " UNFIT" << endl;
                  }
               }
               else
               {
                  cout << " PRECISE" << endl; // probably
               }
               cout << endl;
            }
         }
         break;
#endif
            // this is the real code
         if (fp.getXvt(navLib, useAlm, xvt))
         {
            cout << fp.nmid
                 << gnsstk::printTime(fp.when, " %Y/%j/%02H:%02M:%015.12f %P ")
                 << setprecision(15) << xvt;
            if (ell != nullptr)
            {
               Position coords(xvt.x, Position::Cartesian, ell, xvt.frame);
               coords.transformTo(Position::Geodetic);
               cout << " " << coords;
            }
            cout << endl;
         }
         else
         {
            cout << "Not found" << endl;
            found = false;
            exitCode = BasicFramework::EXIST_ERROR;
         }
         break;
      case NavMessageType::Unknown:
         if (fp.getXvt(navLib, useAlm, xvt))
         {
            cout << fp.nmid
                 << gnsstk::printTime(fp.when, " %Y/%j/%02H:%02M:%015.12f %P ")
                 << setprecision(15) << xvt;
            if (ell != nullptr)
            {
               Position coords(xvt.x, Position::Cartesian, ell, xvt.frame);
               coords.transformTo(Position::Geodetic);
               cout << " " << coords;
            }
            cout << endl;
         }
         else
         {
            cout << "Not found" << endl;
            found = false;
            exitCode = BasicFramework::EXIST_ERROR;
         }
         break;
      default:
         cerr << "Can't compute an XVT using message type \""
              << StringUtils::asString(fp.nmid.messageType)
              << "\"" << endl;
         break;
   } // switch
   return found;
}


int main(int argc, char *argv[])
{
#include "NewNavInit.h"
   try
   {
      NavDump app(argv[0]);

      if (!app.initialize(argc, argv))
         return app.exitCode;

      app.run();

      return app.exitCode;
   }
   catch (gnsstk::Exception& exc)
   {
      cerr << exc << endl;
   }
   catch (std::exception& exc)
   {
      cerr << "std::exception " << exc.what() << endl;
   }
   catch (...)
   {
      cerr << "Caught an unnamed exception. Exiting." << endl;
   }
   return gnsstk::BasicFramework::EXCEPTION_ERROR;
}
