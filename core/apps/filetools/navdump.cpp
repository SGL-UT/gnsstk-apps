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
#include <gnsstk/DebugTrace.hpp>

using namespace std;
using namespace gnsstk;

/// Storage for find parameters
class FindParams
{
public:
   FindParams()
         : xmitHealth(SVHealth::Any), valid(NavValidityType::Any),
           order(NavSearchOrder::User)
   {
   }
      /** Subject satellite, transmit satellite, obs ID (obs type=nav,
       * carrier band, tracking code, xmit antenna, freq offset), nav
       * type, nav msg type */
   NavMessageID nmid;
      /// Time of interest for query
   CommonTime when;
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
     << "  " << printTime(fp.when, "%4Y/%03j/%02H:%02M:%02S") << endl
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
 * <b>navdump</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-t</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-l</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-F</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-X</b>&nbsp;\argarg{ARG}<b>]</b> \argarg{ARG} <b>[</b>...<b>]</b>
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
 * \dicdef{Find a specific object in the input matching "navmsgtype YYYY/DOY/HH:MM:SS subject-sat-id system [carrier [range [navtype [xmit-health [validity-type [xmit-sat-id system]]]]]]"}
 * \dicterm{-X, \--xvt=\argarg{ARG}}
 * \dicdef{Compute a satellite XVT using nav data matching "navmsgtype YYYY/DOY/HH:MM:SS subject-sat-id system [carrier [range [navtype [xmit-health [validity-type [xmit-sat-id system]]]]]]"  (navmsgtype must be Ephemeris, Almanac or Unknown for Almanac-if-no-Ephemeris)}
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

   bool initialize(int argc, char *argv[], bool pretty=true) throw() override;
   void process() override;

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
      /// High level nav store interface.
   NavLibrary navLib;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
      /// Dump output level of detail
   DumpDetail detail;
};


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
      std::vector<std::string> words = StringUtils::split(value[i]);
      unsigned numWords = words.size();
      if (numWords < 4)
      {
         rv += "Invalid parameter to find";
         break;
      }
      params1.nmid.messageType = StringUtils::asNavMessageType(words[0]);
      if (params1.nmid.messageType == NavMessageType::Unknown)
      {
         if (warnNMT || (words[0] != "Unknown"))
         {
            cerr << "Warning: Unknown nav message type: \"" << words[0] << "\""
                 << endl;
         }
      }
      try
      {
         gnsstk::scanTime(params1.when, words[1], "%Y/%j/%H:%M:%S");
         params1.when.setTimeSystem(TimeSystem::Any);
      }
      catch (gnsstk::Exception& exc)
      {
         rv += exc.getText() + " (" + words[1] + ")";
         break;
      }
      params1.nmid.sat.id = StringUtils::asInt(words[2]);
      params1.nmid.sat.system = StringUtils::asSatelliteSystem(words[3]);
         // default the overall system to the subject system
         // (potentially change later to the transmit system)
      params1.nmid.system = params1.nmid.sat.system;
      if (params1.nmid.sat.system == SatelliteSystem::Unknown)
      {
         cerr << "Warning: Unknown system \"" << words[3] << "\"" << endl;
      }
      params1.nmid.obs.type = ObservationType::NavMsg;
      params1.nmid.obs.band = CarrierBand::Any;
      params1.nmid.obs.code = TrackingCode::Any;
      params1.nmid.xmitSat.makeWild();
      params1.nmid.nav = NavType::Any;
      if (numWords > 4)
      {
         params1.nmid.obs.band = StringUtils::asCarrierBand(words[4]);
         if (params1.nmid.obs.band == CarrierBand::Unknown)
         {
            cerr << "Warning: Unknown carrier band \"" << words[4] << "\""
                 << endl;
         }
      }
      if (numWords > 5)
      {
         params1.nmid.obs.code = StringUtils::asTrackingCode(words[5]);
         if (params1.nmid.obs.code == TrackingCode::Unknown)
         {
            cerr << "Warning: Unknown tracking code \"" << words[5] << "\""
                 << endl;
         }
      }
      if (numWords > 6)
      {
         params1.nmid.nav = StringUtils::asNavType(words[6]);
         if (params1.nmid.nav == NavType::Unknown)
         {
            cerr << "Warning: Unknown nav type \"" << words[6] << "\""
                 << endl;
         }
      }
      if (numWords > 7)
      {
         params1.xmitHealth = StringUtils::asSVHealth(words[7]);
         if (params1.xmitHealth == SVHealth::Unknown)
         {
            cerr << "Warning: Unknown health \"" << words[7] << "\""
                 << endl;
         }
      }
      if (numWords > 8)
      {
         params1.valid = StringUtils::asNavValidityType(words[8]);
         if (params1.valid == NavValidityType::Unknown)
         {
            cerr << "Warning: Unknown validity type \"" << words[8] << "\""
                 << endl;
         }
      }
      if (numWords == 10)
      {
         rv += "xmit-sat-id also requires system be specified";
         break;
      }
      if (numWords > 10)
      {
            // use SatID constructor to properly set wildcard flags
         params1.nmid.xmitSat =
            SatID(StringUtils::asInt(words[9]),
                  StringUtils::asSatelliteSystem(words[10]));
         if (params1.nmid.xmitSat.system == SatelliteSystem::Unknown)
         {
            cerr << "Warning: Unknown system \"" << words[10] << "\"" << endl;
         }
      }
      if (numWords > 11)
      {
         rv += "Extraneous text at end of \"" + value[i] + "\"";
         break;
      }
      params.push_back(params1);
   }
   return rv;
}


NavDump ::
NavDump(const string& applName)
      : BasicFramework(applName, "Dump any and all navigation data (that we"
                       " support).  Debug will enable tracing (if tracing was"
                       " compiled into the libraries)"),
        typesOpt('t', "type", "Navigation data type(s) to dump (use \"-E"
                 " navmsgtype\" for list, default=all types)"),
        detailOpt('l',"level","Specify detail level of output (use \"-E"
                  " detail\" for list, default=Full)"),
        inqOpt('F', "find", "Find a specific object in the input matching \""
               "navmsgtype YYYY/DOY/HH:MM:SS subject-sat-id system [carrier"
               " [range [navtype [xmit-health [validity-type [xmit-sat-id"
               " system]]]]]]\""),
        xvtOpt('X', "xvt", "Compute a satellite XVT using nav data matching \""
               "navmsgtype YYYY/DOY/HH:MM:SS subject-sat-id system [carrier"
               " [range [navtype [xmit-health [validity-type [xmit-sat-id"
               " system]]]]]]\"  (navmsgtype must be Ephemeris, Almanac or"
               " Unknown for Almanac-if-no-Ephemeris)", false),
        enumHelpOpt('E', "enum"),
        detail(DumpDetail::Full),
        filesOpt("", true)
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   filesOpt.setDescription("NAV-FILE [...] (supported formats: " +
                           ndfp->getFactoryFormats() + ")");
   detailOpt.setMaxCount(1);
}


bool NavDump ::
initialize(int argc, char *argv[], bool pretty) throw()
{
   if (!BasicFramework::initialize(argc, argv, pretty))
      return false;
   if (debugLevel)
   {
      DEBUGTRACE_ENABLE();
      DEBUGTRACE_FUNCTION();
   }
   navLib.addFactory(ndfp);
      // process nav message type command line arguments
   vector<string> types(typesOpt.getValue());
   NavMessageTypeSet nmts;
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
   if (nmts.size() > 0)
   {
      navLib.setTypeFilter(nmts);
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
   if (inqOpt.params.size() > 0)
   {
      for (unsigned i = 0; i < inqOpt.params.size(); i++)
      {
         if (verboseLevel)
         {
            cout << "Looking for" << endl << inqOpt.params[i];
         }
         NavDataPtr ndp;
         if (navLib.find(inqOpt.params[i].nmid, inqOpt.params[i].when, ndp,
                         inqOpt.params[i].xmitHealth, inqOpt.params[i].valid,
                         inqOpt.params[i].order))
         {
            ndp->dump(cout, detail);
         }
         else
         {
            cout << "Not found" << endl;
         }
      }
   }
   if (xvtOpt.params.size() > 0)
   {
      for (unsigned i = 0; i < xvtOpt.params.size(); i++)
      {
         if (verboseLevel)
         {
            cout << "Looking for" << endl << xvtOpt.params[i];
         }
         Xvt xvt;
         bool useAlm = false;
         switch (xvtOpt.params[i].nmid.messageType)
         {
            case NavMessageType::Almanac:
               useAlm = true;
            case NavMessageType::Ephemeris:
               if (navLib.getXvt(xvtOpt.params[i].nmid, xvtOpt.params[i].when,
                                 xvt, useAlm, xvtOpt.params[i].xmitHealth,
                                 xvtOpt.params[i].valid,
                                 xvtOpt.params[i].order))
               {
                  cout << setprecision(15) << xvt << endl;
               }
               else
               {
                  cout << "Not found" << endl;
               }
               break;
            case NavMessageType::Unknown:
               if (navLib.getXvt(xvtOpt.params[i].nmid, xvtOpt.params[i].when,
                                 xvt, xvtOpt.params[i].xmitHealth,
                                 xvtOpt.params[i].valid,
                                 xvtOpt.params[i].order))
               {
                  cout << setprecision(15) << xvt << endl;
               }
               else
               {
                  cout << "Not found" << endl;
               }
               break;
            default:
               cerr << "Can't compute an XVT using message type \""
                    << StringUtils::asString(xvtOpt.params[i].nmid.messageType)
                    << "\"" << endl;
               break;
         } // switch
      } // for (unsigned i
   } // if (xvtOpt.params.size() > 0)
   if (inqOpt.params.empty() && xvtOpt.params.empty())
   {
         // dump the processed results
      navLib.dump(cout,detail);
   }
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
