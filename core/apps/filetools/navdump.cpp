//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

#include <gnsstk/BasicFramework.hpp>
#include <gnsstk/NavLibrary.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>

using namespace std;
using namespace gnsstk;

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



/// Get command-line option help for DumpDetail strings.
class CommandOptionHelpDumpDetail : public CommandOptionHelp
{
public:
      /** Constructor.
       * @param[in] shOpt The one character command line option.
       *   Set to 0 if unused.
       * @param[in] loOpt The long command option.  Set to
       *   std::string() if unused.
       * @param[in] desc A string describing what this option does.
       */ 
   CommandOptionHelpDumpDetail(const char shOpt,
                               const std::string& loOpt,
                               const std::string& desc);

      /** Print the requested help information.
       * @param[in] out The stream to which the help text will be printed.
       * @param[in] pretty Unused in this child class. */
   void printHelp(std::ostream& out, bool pretty = true) override;
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
 * <b>navdump</b>  <b>-T</b> <br/>
 * <b>navdump</b>  <b>-L</b> <br/>
 * <b>navdump</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-t</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-l</b>&nbsp;\argarg{ARG}<b>]</b> \argarg{ARG} <b>[</b>...<b>]</b>
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
 * \dicdef{Navigation data type(s) to dump (use -T for list, default=all types)}
 * \dicterm{-T, \--typelist}
 * \dicdef{List out valid nav message types and quit}
 * \dicterm{-l, \--level=\argarg{ARG}}
 * \dicdef{Specify detail level of output (default=Full)}
 * \dicterm{-L, \--levellist}
 * \dicdef{List out valid detail levels}
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
 *
 * \section rnwdiff_see_also SEE ALSO
 * \ref rmwdiff, \ref rowdiff
 * navdump will return 
 */
class NavDump : public BasicFramework
{
public:
   NavDump(const string& applName);

   bool initialize(int argc, char *argv[], bool pretty=true) throw() override;
   void process() override;

      /// Command-line option for specifying the type of data to dump.
   CommandOptionWithAnyArg typesOpt;
      /// Get a list of nav message types.
   CommandOptionHelpNavMessageType typesHelpOpt;
      /// Specify the detail level.
   CommandOptionWithAnyArg detailOpt;
      /// Get a list of detail levels.
   CommandOptionHelpDumpDetail detailHelpOpt;
      /// All remaining command-line arguments are treated as input files.
   CommandOptionRest filesOpt;
      /// High level nav store interface.
   NavLibrary navLib;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
      /// Dump output level of detail
   DumpDetail detail;
};


CommandOptionHelpNavMessageType ::
CommandOptionHelpNavMessageType(const char shOpt,
                                const std::string& loOpt,
                                const std::string& desc)
      : CommandOptionHelp(gnsstk::CommandOption::noArgument, shOpt, loOpt,
                          desc)
{
}


void CommandOptionHelpNavMessageType ::
printHelp(std::ostream& out, bool pretty)
{
   ostringstream s;
   char *colch = getenv("COLUMNS");
   int columns = 80;
   unsigned maxlen = 0;
   if (colch)
   {
      string colStr(colch);
      columns = StringUtils::asInt(colStr);
   }
   s << "Types:";
   for (NavMessageType i : NavMessageTypeIterator())
   {
      if (i != NavMessageType::Unknown)
         s << " " << StringUtils::asString(i);
   }
   if (pretty)
      out << StringUtils::prettyPrint(s.str(),"\n","   ","",columns);
   else
      out << s.str();
   out << endl;
}




CommandOptionHelpDumpDetail ::
CommandOptionHelpDumpDetail(const char shOpt,
                                const std::string& loOpt,
                                const std::string& desc)
      : CommandOptionHelp(gnsstk::CommandOption::noArgument, shOpt, loOpt,
                          desc)
{
}


void CommandOptionHelpDumpDetail ::
printHelp(std::ostream& out, bool pretty)
{
   ostringstream s;
   char *colch = getenv("COLUMNS");
   int columns = 80;
   unsigned maxlen = 0;
   if (colch)
   {
      string colStr(colch);
      columns = StringUtils::asInt(colStr);
   }
   s << "Detail levels:";
   for (DumpDetail i : DumpDetailIterator())
   {
      if (i != DumpDetail::Unknown)
         s << " " << StringUtils::asString(i);
   }
   if (pretty)
      out << StringUtils::prettyPrint(s.str(),"\n","   ","",columns);
   else
      out << s.str();
   out << endl;
}


NavDump ::
NavDump(const string& applName)
      : BasicFramework(applName, "Dump any and all navigation data (that we"
                       " support)"),
        typesOpt('t', "type", "Navigation data type(s) to dump (use -T for"
                 " list, default=all types)"),
        typesHelpOpt('T', "typelist", "List out valid nav message types and"
                     " quit"),
        detailOpt('l',"level","Specify detail level of output (default=Full)"),
        detailHelpOpt('L', "levellist", "List out valid detail levels"),
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
      // dump the processed results
   navLib.dump(cout,detail);
}


int main(int argc, char *argv[])
{
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
