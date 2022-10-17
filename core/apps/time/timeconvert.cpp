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
 * - \subpage timeconvert - Convert times between a variety of formats
 * \page timeconvert
 * \tableofcontents
 *
 * \section timeconvert_name NAME
 * timeconvert - Display time stamps in a variety of formats
 *
 * \section timeconvert_synopsis SYNOPSIS
 * <b>timeconvert</b>  <b>-h</b> <br/>
 * <b>timeconvert</b>  <b>\--systems</b> <br/>
 * <b>timeconvert</b> <b>[-d</b><b>]</b> <b>[-v</b><b>]</b> <b>[-A</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-c</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-R</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-o</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-f</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-w</b>&nbsp;\argarg{TIME}<b>]</b> <b>[\--z29</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-Z</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-j</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-m</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-u</b>&nbsp;\argarg{TIME}<b>]</b> <b>[-y</b>&nbsp;\argarg{TIME}<b>]</b> <b>[\--src-sys</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--tgt-sys</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--nav</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--input-format</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--input-time</b>&nbsp;\argarg{ARG}<b>]</b> <b>[\--offset</b><b>]</b> <b>[-F</b>&nbsp;\argarg{ARG}<b>]</b> <b>[-a</b>&nbsp;\argarg{NUM}<b>]</b> <b>[-s</b>&nbsp;\argarg{NUM}<b>]</b>
 *
 * \section timeconvert_description DESCRIPTION
 * This application allows the user to convert between time formats
 * associated with GNSS. Time formats include, but are not limited to:
 * civilian time, Julian day of year and year, GPS week and seconds of
 * week, Z counts, and Modified Julian Date (MJD).
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-A, \--ansi=\argarg{TIME}}
 * \dicdef{"ANSI-Second"}
 * \dicterm{-c, \--civil=\argarg{TIME}}
 * \dicdef{"Month(numeric) DayOfMonth Year Hour:Minute:Second"}
 * \dicterm{-R, \--rinex-file=\argarg{TIME}}
 * \dicdef{"Year(2-digit) Month(numeric) DayOfMonth Hour Minute Second"}
 * \dicterm{-o, \--ews=\argarg{TIME}}
 * \dicdef{"GPSEpoch 10bitGPSweek SecondOfWeek"}
 * \dicterm{-f, \--ws=\argarg{TIME}}
 * \dicdef{"FullGPSWeek SecondOfWeek"}
 * \dicterm{-w, \--wz=\argarg{TIME}}
 * \dicdef{"FullGPSWeek Zcount"}
 * \dicterm{\--z29=\argarg{TIME}}
 * \dicdef{"29bitZcount"}
 * \dicterm{-Z, \--z32=\argarg{TIME}}
 * \dicdef{"32bitZcount"}
 * \dicterm{-j, \--julian=\argarg{TIME}}
 * \dicdef{"JulianDate"}
 * \dicterm{-m, \--mjd=\argarg{TIME}}
 * \dicdef{"ModifiedJulianDate"}
 * \dicterm{-u, \--unixtime=\argarg{TIME}}
 * \dicdef{"UnixSeconds UnixMicroseconds"}
 * \dicterm{-y, \--doy=\argarg{TIME}}
 * \dicdef{"Year DayOfYear SecondsOfDay"}
 * \dicterm{\--systems}
 * \dicdef{List available time systems}
 * \dicterm{\--src-sys=\argarg{ARG}}
 * \dicdef{Source time system when converting between systems}
 * \dicterm{\--tgt-sys=\argarg{ARG}}
 * \dicdef{Target time system when converting between systems}
 * \dicterm{\--nav=\argarg{ARG}}
 * \dicdef{Where to get the navigation data. Can be RINEX2, RINEX3.}
 * \dicterm{\--input-format=\argarg{ARG}}
 * \dicdef{Time format to use on input}
 * \dicterm{\--input-time=\argarg{ARG}}
 * \dicdef{Time to be parsed by "input-format" option}
 * \dicterm{\--offset}
 * \dicdef{Only display the offset at the reference time}
 * \dicterm{-F, \--format=\argarg{ARG}}
 * \dicdef{Time format to use on output}
 * \dicterm{-a, \--add-offset=\argarg{NUM}}
 * \dicdef{Add NUM seconds to specified time}
 * \dicterm{-s, \--sub-offset=\argarg{NUM}}
 * \dicdef{Subtract NUM seconds from specified time}
 * \enddictionary
 *
 * \section timeconvert_examples EXAMPLES
 *
 * \subsection timeconvert_example_default Get the current system time in UTC.
 * \code{.sh}
 * > timeconvert
 *
 *         Month/Day/Year H:M:S            08/10/2020 17:00:04
 *         Modified Julian Date            59071.708386331
 *         GPSweek DayOfWeek SecOfWeek     70 1  147604.579029
 *         FullGPSweek Zcount              2118  98403
 *         Year DayOfYear SecondOfDay      2020 223  61204.579029
 *         Unix: Second Microsecond        1597078804 579029
 *         Zcount: 29-bit (32-bit)         36798563 (1110540387)
 * \endcode
 *
 * \subsection timeconvert_example_rinex Convert RINEX file time.
 * \code{.sh}
 * > timeconvert -R "85 05 06 13 50 02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 *
 * \subsection timeconvert_example_civil Convert civil time.
 * \code{.sh}
 * > timeconvert -c "5 6 1985 13:50:02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 *
 * \subsection timeconvert_example_format Output only year, day of year and seconds of day.
 * \code{.sh}
 * > timeconvert  -F "%04Y %03j %05.0s"
 * 2020 223 61347
 * \endcode
 *
 * \subsection timeconvert_example_input_format Custom input format.
 * \code{.sh}
 * > timeconvert  --input-format="%Y %j %H:%M:%S" --input-time="1985 126 13:50:02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 *
 * \subsection timeconvert_example_subtract Display beginning of yesterday.
 * \code{.sh}
 * > timeconvert -s 86400 -F "%4Y %03j 0"
 * 2020 222 0
 * \endcode
 *
 * \subsection timeconvert_example_system Convert between time systems.
 * Show current time in the GPS time system.
 * \code{.sh}
 * > timeconvert --tgt-sys GPS
 *
 *         Month/Day/Year H:M:S            02/10/2021 21:12:34
 *         Modified Julian Date            59255.883732244
 *         GPSweek DayOfWeek SecOfWeek     96 3  335554.465877
 *         FullGPSweek Zcount              2144 223702
 *         Year DayOfYear SecondOfDay      2021 041  76354.465877
 *         Unix: Second Microsecond        1612991554 465877
 *         Zcount: 29-bit (32-bit)         50555350 (1124297174)
 * \endcode
 *
 * Convert GPS time to UTC.
 * \code{.sh}
 * > timeconvert -y "2021 2 0" --tgt-sys UTC
 * 
 *         Month/Day/Year H:M:S            01/01/2021 23:59:42
 *         Modified Julian Date            59215.999791667
 *         GPSweek DayOfWeek SecOfWeek     90 5  518382.000000
 *         FullGPSweek Zcount              2138 345588
 *         Year DayOfYear SecondOfDay      2021 001  86382.000000
 *         Unix: Second Microsecond        1609545582      0
 *         Zcount: 29-bit (32-bit)         47531508 (1121273332)
 * \endcode
 *
 * Show the current UTC-GPS offset in seconds.
 * \code{.sh}
 * > timeconvert --offset --tgt-sys GPS
 * 18
 * \endcode
 *
 * Convert GPS time to UTC using RINEX NAV DELTA-UTC headers.
 * \code{.sh}
 * > timeconvert -f "1854 233472" --nav gnsstk-apps/data/test_input_mpsolve.15n --tgt-sys UTC
 * 
 *         Month/Day/Year H:M:S            07/21/2015 16:51:29
 *         Modified Julian Date            57224.702418981
 *         GPSweek DayOfWeek SecOfWeek     830 2  233489.000000
 *         FullGPSweek Zcount              1854 155659
 *         Year DayOfYear SecondOfDay      2015 202  60689.000000
 *         Unix: Second Microsecond        1437497489      0
 *         Zcount: 29-bit (32-bit)         435314699 (972185611)
 * \endcode
 *
 * List supported time systems:
 * \code{.sh}
 * > timeconvert --systems
 * Available Time Systems:
 *   UNK
 *   Any
 *   GPS
 *   GLO
 *   GAL
 *   QZS
 *   BDT
 *   IRN
 *   UTC
 *   TAI
 *   TT
 *   TDB
 * \endcode
 *
 * \section timeconvert_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors occurred}
 * \dictentry{1,A C++ exception occurred (typically invalid time format or value)}
 * \enddictable
 * \section timeconvert_see_also SEE ALSO
 * printTime()
 */

#include "NewNavInc.h"
#include <gnsstk/BasicFramework.hpp>

#include <gnsstk/TimeString.hpp>
#include <gnsstk/TimeConstants.hpp>

#include <gnsstk/ANSITime.hpp>
#include <gnsstk/CivilTime.hpp>
#include <gnsstk/GPSWeekSecond.hpp>
#include <gnsstk/GPSWeekZcount.hpp>
#include <gnsstk/JulianDate.hpp>
#include <gnsstk/MJD.hpp>
#include <gnsstk/UnixTime.hpp>
#include <gnsstk/YDSTime.hpp>
#include <gnsstk/SystemTime.hpp>
#include <gnsstk/DebugTrace.hpp>
#include <gnsstk/CommandOptionWithCommonTimeArg.hpp>
#include <gnsstk/EnumIterator.hpp>
#include <gnsstk/BasicTimeSystemConverter.hpp>
#include <gnsstk/NavTimeSystemConverter.hpp>
#include <gnsstk/MultiFormatNavDataFactory.hpp>

using namespace std;
using namespace gnsstk;

class CommandOptionHelpTimeSystem : public gnsstk::CommandOptionHelp
{
public:
      /** Constructor.
       * @param[in] shOpt The one character command line option.
       *   Set to 0 if unused.
       * @param[in] loOpt The long command option.  Set to
       *   std::string() if unused.
       * @param[in] desc A string describing what this option does.
       */ 
   CommandOptionHelpTimeSystem(const char shOpt,
                               const std::string& loOpt,
                               const std::string& desc)
         : CommandOptionHelp(gnsstk::CommandOption::noArgument, shOpt, loOpt,
                             desc)
   {
   }

      /** Print the requested help information.
       * @param[in] out The stream to which the help text will be printed.
       * @param[in] pretty Unused in this child class. */
   void printHelp(std::ostream& out, bool pretty = true) override
   {
      out << "Available Time Systems:" << endl;
      for (gnsstk::TimeSystem e : TimeSystemIterator())
      {
         cout << "  " << gnsstk::StringUtils::asString(e) << endl;
      }
   }
};


class TimCvt : public BasicFramework
{
public:
   TimCvt(char* arg0);

protected:
   virtual void process();

private:
   CommandOptionWithCommonTimeArg ANSITimeOption;
   CommandOptionWithCommonTimeArg CivilTimeOption;
   CommandOptionWithCommonTimeArg RinexFileTimeOption;
   CommandOptionWithCommonTimeArg GPSEWSOption;
   CommandOptionWithCommonTimeArg GPSWSOption;
   CommandOptionWithCommonTimeArg GPSWZOption;
   CommandOptionWithCommonTimeArg GPSZ29Option;
   CommandOptionWithCommonTimeArg GPSZ32Option;
   CommandOptionWithCommonTimeArg JDOption;
   CommandOptionWithCommonTimeArg MJDOption;
   CommandOptionWithCommonTimeArg UnixTimeOption;
   CommandOptionWithCommonTimeArg YDSTimeOption;
   CommandOptionHelpTimeSystem timeSystemHelpOption;
   CommandOptionWithAnyArg srcTSOption; ///< Source time system
   CommandOptionWithAnyArg tgtTSOption; ///< Target time system
   CommandOptionWithAnyArg navOption;   ///< Navigation data input files.

   CommandOptionWithAnyArg inputFormatOption;
   CommandOptionWithAnyArg inputTimeOption;
   CommandOptionAllOf inputFormatAndTimeOption;

   CommandOptionNoArg offsOnlyOption;
   CommandOptionWithAnyArg formatOption;
   CommandOptionMutex offsetOrFormatOption;
   CommandOptionDependent offsetTgtTSOption;
   CommandOptionWithNumberArg addOption;
   CommandOptionWithNumberArg subOption;
   CommandOptionMutex mutexOption;

   string stringToParse;
   string timeSpec;
      /// nav data file reader
   gnsstk::NavDataFactoryPtr ndfp;
};


TimCvt::TimCvt(char* arg0)
      : BasicFramework(arg0, "Converts from a given input time specification"
                       " to other time formats.  Include the quotation marks."
                       "  All year values are four digit years.\n\nWhen"
                       " converting between time systems, GPS is the default"
                       " source time system when a time is specified, while UTC"
                       " is the default source time system when a time is not"
                       " specified."),
        ANSITimeOption('A', "ansi", "%K", "\"ANSI-Second\""),
        CivilTimeOption('c', "civil", "%m %d %Y %H:%M:%f",
                        "\"Month(numeric) DayOfMonth Year"
                        " Hour:Minute:Second\""),
        RinexFileTimeOption('R', "rinex-file", "%y %m %d %H %M %S",
                            "\"Year(2-digit) Month(numeric) DayOfMonth Hour"
                            " Minute Second\""),
        GPSEWSOption('o', "ews", "%E %G %g", 
                     "\"GPSEpoch 10bitGPSweek SecondOfWeek\""),
        GPSWSOption('f', "ws", "%F %g", "\"FullGPSWeek SecondOfWeek\""),
        GPSWZOption('w', "wz", "%F %Z", "\"FullGPSWeek Zcount\""),
        GPSZ29Option(0, "z29", "%E %c", "\"29bitZcount\""),
        GPSZ32Option('Z', "z32", "%C", "\"32bitZcount\""),
        JDOption('j', "julian", "%J", "\"JulianDate\""),
        MJDOption('m', "mjd", "%Q", "\"ModifiedJulianDate\""),
        UnixTimeOption('u',"unixtime", "%U %u",
                       "\"UnixSeconds UnixMicroseconds\""),
        YDSTimeOption('y', "doy", "%Y %j %s",
                      "\"Year DayOfYear SecondsOfDay\""),
        inputFormatOption(0, "input-format", "Time format to use on input"),
        inputTimeOption(0, "input-time",
                        "Time to be parsed by \"input-format\" option"),
        formatOption('F', "format", "Time format to use on output"),
        addOption('a', "add-offset", "Add NUM seconds to specified time"),
        subOption('s', "sub-offset",
                  "Subtract NUM seconds from specified time"),
        timeSystemHelpOption(0, "systems", "List available time systems"),
        srcTSOption(0, "src-sys", "Source time system when converting between"
                    " systems"),
        tgtTSOption(0, "tgt-sys", "Target time system when converting between"
                    " systems"),
        navOption(0, "nav", "if you see this, we failed"),
        offsOnlyOption(0, "offset", "Only display the offset at the reference"
                       " time"),
        offsetTgtTSOption(&tgtTSOption, &offsOnlyOption)
{
      // Initialize these two items in here rather than in the
      // initializer list to guarantee execution order and avoid seg
      // faults.
   ndfp = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
      // process only time offset messages
   ndfp->setTypeFilter({NavMessageType::TimeOffset});
   navOption.setDescription("Where to get the navigation data. Can be " +
                            ndfp->getFactoryFormats() + ".");
   ANSITimeOption.setMaxCount(1);
   CivilTimeOption.setMaxCount(1);
   RinexFileTimeOption.setMaxCount(1);
   GPSEWSOption.setMaxCount(1);
   GPSWSOption.setMaxCount(1);
   GPSWZOption.setMaxCount(1);
   GPSZ29Option.setMaxCount(1);
   GPSZ32Option.setMaxCount(1);
   JDOption.setMaxCount(1);
   MJDOption.setMaxCount(1);
   UnixTimeOption.setMaxCount(1);
   YDSTimeOption.setMaxCount(1);
   formatOption.setMaxCount(1);
   srcTSOption.setMaxCount(1);
   tgtTSOption.setMaxCount(1);

   inputFormatOption.setMaxCount(1);
   inputTimeOption.setMaxCount(1);
   inputFormatAndTimeOption.addOption(&inputFormatOption);
   inputFormatAndTimeOption.addOption(&inputTimeOption);

   mutexOption.addOption(&ANSITimeOption);
   mutexOption.addOption(&CivilTimeOption);
   mutexOption.addOption(&RinexFileTimeOption);
   mutexOption.addOption(&GPSEWSOption);
   mutexOption.addOption(&GPSWSOption);
   mutexOption.addOption(&GPSWZOption);
   mutexOption.addOption(&GPSZ29Option);
   mutexOption.addOption(&GPSZ32Option);
   mutexOption.addOption(&JDOption);
   mutexOption.addOption(&MJDOption);
   mutexOption.addOption(&UnixTimeOption);
   mutexOption.addOption(&YDSTimeOption);
   mutexOption.addOption(&inputFormatAndTimeOption);

   offsetOrFormatOption.addOption(&offsOnlyOption);
   offsetOrFormatOption.addOption(&formatOption);
}


void TimCvt::process()
{
   if (debugLevel)
   {
      DEBUGTRACE_ENABLE();
   }
   CommonTime ct;
   ct.setTimeSystem(TimeSystem::GPS);
   CommandOption *whichOpt = mutexOption.whichOne();

   if (navOption.getCount())
   {
         // create our time system converter
      CommonTime::tsConv = make_shared<NavTimeSystemConverter>();
      NavTimeSystemConverter *ntsc = dynamic_cast<NavTimeSystemConverter*>(
         CommonTime::tsConv.get());
         // create our NavLibrary and associate it with the
         // NavTimeSystemConverter
      ntsc->navLib = make_shared<NavLibrary>();
         // Set the NavLibrary to use the MultiFormatNavDataFactory
      ntsc->navLib->addFactory(ndfp);
         // load the data files.
      std::vector<string> values(navOption.getValue());
      for (size_t i=0; i<values.size(); i++)
      {
         if (!ndfp->addDataSource(values[i]))
         {
            cerr << "Unable to load \"" << values[i] << "\"" << endl;
               // this could be a missing or invalid file either one.
            exitCode = BasicFramework::EXIST_ERROR;
               // We continue because it's possible that the user
               // specified a valid file in addition to this invalid
               // one.  But we still want to indicate the error with
               // the message and exit code.
         }
      }
      switch (debugLevel)
      {
         case 0:
            break;
         case 1:
            ndfp->dump(std::cerr, gnsstk::DumpDetail::OneLine);
            break;
         case 2:
            ndfp->dump(std::cerr, gnsstk::DumpDetail::Brief);
            break;
         default:
            ndfp->dump(std::cerr, gnsstk::DumpDetail::Full);
            break;
      }
   }
   else
   {
      CommonTime::tsConv = make_shared<BasicTimeSystemConverter>();
   }

   if (whichOpt)
   {
      CommandOptionWithCommonTimeArg *cta =
         dynamic_cast<CommandOptionWithCommonTimeArg *>(whichOpt);
      if (cta)
      {
         ct = cta->getTime().front();
         if (srcTSOption.getCount())
         {
            ct.setTimeSystem(gnsstk::StringUtils::asTimeSystem(
                                srcTSOption.getValue()[0]));
         }
         else
         {
            ct.setTimeSystem(TimeSystem::GPS);
         }
      }
      else // whichOpt == &inputFormatAndTimeOption
      {
         mixedScanTime( ct,
                        inputTimeOption.getValue().front(),
                        inputFormatOption.getValue().front() );
         if (srcTSOption.getCount())
         {
            ct.setTimeSystem(gnsstk::StringUtils::asTimeSystem(
                                srcTSOption.getValue()[0]));
         }
         else
         {
            ct.setTimeSystem(TimeSystem::GPS);
         }
      }
   }
   else
   {
      ct = SystemTime();
      ct.setTimeSystem(TimeSystem::UTC); 
   }

   int i;
   int addOptions = addOption.getCount();
   int subOptions = subOption.getCount();
   for (i = 0; i < addOptions; i++)
      ct += StringUtils::asDouble(addOption.getValue()[i]);
   for (i = 0; i < subOptions; i++)
      ct -= StringUtils::asDouble(subOption.getValue()[i]);

   if (tgtTSOption.getCount())
   {
      gnsstk::TimeSystem ts = gnsstk::StringUtils::asTimeSystem(
         tgtTSOption.getValue()[0]);
      if (offsOnlyOption)
      {
         double offset;
         if (!CommonTime::tsConv->getOffset(ct.getTimeSystem(), ts, ct, offset))
         {
            cerr << "Unable to change time systems" << endl;
            exitCode = 1;
         }
         else
         {
            cout << setprecision(20) << offset << endl;
         }
         return;
      }
      else
      {
         if (!ct.changeTimeSystem(ts))
         {
            cerr << "Unable to change time systems" << endl;
            exitCode = 1;
            return;
         }
      }
   }

   if (formatOption.getCount())
   {
      cout << printTime(ct, formatOption.getValue()[0]) << endl;
   }
   else
   {
      using StringUtils::leftJustify;
      string eight(8, ' '); // eight spaces

      GPSWeekZcount wz(ct);
      CivilTime civ(ct);

      cout << endl
           << eight << leftJustify("Month/Day/Year H:M:S", 32)
           << CivilTime(ct).printf("%02m/%02d/%04Y %02H:%02M:%02S") << endl

           << eight << leftJustify("Modified Julian Date", 32)
           << setprecision(15) << MJD(ct).printf("%15.9Q") << endl

           << eight << leftJustify("GPSweek DayOfWeek SecOfWeek", 32)
           << GPSWeekSecond(ct).printf("%G %w % 13.6g") << endl

           << eight << leftJustify("FullGPSweek Zcount", 32)
           << wz.printf("%F % 6z") << endl

           << eight << leftJustify("Year DayOfYear SecondOfDay", 32)
           << YDSTime(ct).printf("%Y %03j % 12.6s") << endl

           << eight << leftJustify("Unix: Second Microsecond", 32)
           << UnixTime(ct).printf("%U % 6u") << endl

           << eight << leftJustify("Zcount: 29-bit (32-bit)", 32)
           << wz.printf("%c (%C)") << endl

           << endl << endl;
   }

   return;
}


int main(int argc, char* argv[])
{
#include "NewNavInit.h"
   try
   {
      TimCvt m(argv[0]);
      if (!m.initialize(argc, argv))
         return m.exitCode;
      m.run();
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
