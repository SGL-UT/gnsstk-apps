\page usingwheresat Using WHERESAT

By Tracie Conn

Applied Research Laboratories,

The University of Texas at Austin

November 14, 2005

# 1 Overview

The wheresat application is a GNSSTk [1] based application that generates an estimated position and clock state for a GPS satellite from data contained in a RINEX [2] navigation file.  The default outputs are the satellite position and clock correction for a given time.  If the antenna position is specified, azimuth, elevation, and satellite range from the user can be generated as well.  Several options are available that allow the user to customize the output.

# 2 Synopsis

The user executes wheresat from the command line.  The processing performed by wheresat is specified through the command line arguments.  The full set of arguments is defined below.  This list can be duplicated by running wheresat -h on the command line.

shell]./wheresat -h

   Usage: wheresat [OPTION] ...

   Computes the ephemeris data for a single prn.

   Required arguments:

     -b, --broadcast=ARG       RINEX navigation file. User may enter multiple

                               files.

     -p, --prn=NUM             Which SV to analyze.

   Optional arguments:

     -h, --help                Print help usage.

     -u, --position=ARG        Antenna position in ECEF (x,y,z) coordinates.

 Format as a string: &quot;X Y Z&quot;. Used to give

 user-centered data (SV range, azimuth &amp;

 elevation).

     -s, --start=ARG           Starting time for analysis. Format as string:

                               &quot;MO/DD/YYYY HH:MM:SS&quot;. If no start time is

                               specified, the default is end of file.

     -e, --end=ARG             End time for analysis. Format as string:

                               &quot;MO/DD/YYYY HH:MM:SS&quot;. If no end time is

                               specified, the default is beginning of file.

     -o, --output-filename=ARG Results output to a MATLAB suitable file.

     -t, --time=NUM            Time increment for ephemeris calculation.

                               Enter increment in seconds. Default is 900 (15

                               min).

# 3 Detailed Description

The _ephemeris_ of a GPS satellite is essentially a set of parametric equations that predict or describe its position and clock offset over time. The ephemeris borrows its meaning from the Greek term describing something that is brief and only lasts a day. This is appropriate as the ephemeris of a satellite is typically only valid for a few hours.

Each satellite broadcasts its own ephemeris as part of the GPS navigation message. Those ephemerides are used within a receiver to estimate the user position. It is also possible to extract the ephemerides from a receiver in order to process satellite positions on a computer. Arguably the most popular format for GPS ephemerides is RINEX. Usually geodetic quality receivers can report ephemerides in the RINEX format using the control software provided with the receiver. Also, institutions such as the IGS and CORS provide RINEX formatted ephemerides on the Internet from their global networks of GPS reference stations. The wheresat program requires RINEX formatted ephemerides as input.

Johannes Kepler (1571-1630) made a number of key discoveries about the orbits of the planets, including the fact that those orbits are essentially elliptic in shape. For this reason we refer to orbit parameterizations that use the properties of conic sections as _Keplerian_. The GPS ephemeris is essentially Keplerian. For example, it contains parameters such as eccentricity and semi-major axis. The process of computing a satellite&#39;s position based on Keplerian orbit parameters is treated in a number of texts. See references [3], [4], and [5] for general treatments.

The GPS ephemeris does contain parameters (such as perturbations and clock models) not found in classic Keplerian orbit descriptions. For a thorough development of those parameters the reader is referred to references [6] and [7]. The U.S. government, which provides GPS free of charge to all users, also provides the recommended algorithms for using the system for positioning. Those algorithms are defined (but not developed) in the IS-GPS-200 (formerly known as the ICD-GPS-200), reference [8].

# 4 Examples and Usage Notes

## 4.1 Generating Satellite Position and Clock Correction

With only a navigation file and PRN number, wheresat will generate the satellite position and clock correction, beginning with the initial data timestamp given in the file and incrementing by fifteen minutes for the length of the file.  The following sample output demonstrates the most basic processing provided by wheresat.

shell]./wheresat -b s081311a.05n -p 13

 Navigation File:   s081311a.05n

 Start Time:        11/06/2005 23:59:44

 End Time:          11/07/2005 22:00:00

 PRN:               13

 Prn 13 Earth-fixed position and clock information:

 Date       Time(UTC)   X (meters)          Y (meters)          Z (meters)         Clock Correc (s)

 =================================================================================================

 11/06/2005 23:59:44  25592958.219773      -6945904.791468      -879464.271885       0.000022

 11/07/2005 00:14:44  25569192.673629      -6757460.879695      2036138.107937       0.000022

 11/07/2005 00:29:44  25245107.038075      -6458319.603851      4916550.518960       0.000022

 11/07/2005 00:44:44  24641360.332724      -6014158.168108      7711931.308209       0.000022

 11/07/2005 00:59:44  23786475.974395      -5395793.325331      10373840.319877      0.000022

 11/07/2005 01:14:44  22715718.851026      -4580526.400801      12856096.136380      0.000022

## 4.2 Generating Satellite Azimuth, Elevation, and Range

Additionally, the user may specify the antenna position in Earth-entered, Earth Fixed (ECEF) XYZ coordinates and wheresat will generate the azimuth, elevation, and distance of the satellite from the user. These three quantities form a polar coordinate system that locates the satellite relative to the receiver. Azimuth is defined as degrees clockwise from due north, ranging from 0?? to 360??.  Elevation is defined as degrees above the horizon, ranging from 0?? to 90??.  The RINEX navigation message contains orbital data of the satellites for two hour epochs.  Thus if the satellite is rising or setting during the two hour epoch, the application will continue propagating data values for the satellite until the end of the epoch, even if the satellite is below the horizon. This results in elevation angles below zero degrees. When the satellite is below the horizon, it is typically not trackable by a user on the Earth&#39;s surface.

shell]./wheresat -b s081311a.05n -p 13 -u &quot;-740289.807 -5457071.744 3207245.649&quot; -t 7200

 Antenna Position:  -740290  -5.45707e+06  3.20725e+06

 Navigation File:   s081311a.05n

 Start Time:        11/06/2005 23:59:44

 End Time:          11/07/2005 22:00:00

 PRN:               13

 Data for user reference frame:

 Date       Time(UTC)   Azimuth        Elevation      Range to SV (m)

 =====================================================================

 11/06/2005 23:59:44  95.414919       -8.273658       26676597.631756

 11/07/2005 01:59:44  47.374312       3.748820        25287119.770503

 11/07/2005 03:59:44  14.690499       -22.923973      28330633.330613

 11/07/2005 05:59:44  13.567816       -64.055577      32158771.148813

 11/07/2005 09:59:44  231.180314      -45.607615      30764130.512051

 11/07/2005 11:59:44  272.842809      -19.964302      28009759.883277

 11/07/2005 13:59:44  310.202530      11.312081       24480596.645928

 11/07/2005 15:59:44  321.286323      60.090457       20791804.049111

 11/07/2005 17:59:44  193.931515      50.406138       21352872.489626

 11/07/2005 19:59:44  175.617925      -0.508936       25897208.530643

 11/07/2005 21:59:44  139.570738      -21.619231      28265177.691673

 Calculated 11 increments for prn 13.

Note that with the -t argument, the time increment was specified as 7200 seconds, or 2 hours, instead of the default setting of 15 minutes. Also please note that the units of the ECEF user position is in meters.

## 4.3 MATLAB suitable output

If the user intends to utilize MATLAB or another program for analysis of the generated data, an output file may be generated using the ???o argument followed by the desired file name.  The file will contain data in matrix format without a header.  The data type for each column will be printed in the first line, which will be ignored by MATLAB or Octave.  Note that the start and end times for processing may be specified.

shell]./wheresat -b s081311a.05n -p 13 -s &quot;11/07/2005 01:00:00&quot; -e &quot;11/07/2005 02:00:00&quot; ???o SampleOutput

#10-bit GPS week, GPS seconds of week,X,Y,Z(m),Clock Corr.(sec)

1348  90000.000000 23769211.694027   -5383075.291000   10419670.501593   0.000022

1348  90900.000000 22694970.770070   -4564147.271599   12898332.239115   0.000022

1348  91800.000000 21446248.244995   -3533003.258024   15153505.282540   0.000022

1348  92700.000000 20067514.048400   -2283058.078848   17146010.665781   0.000022

# 5.0 References

1. The GNSS Toolkit, GNSSTk.  Website: http://www.gnsstk.org/

1. RINEX: The Receiver Independent Exchange Format Version 2.10.  Available on the web at http://www.ngs.noaa.gov/CORS/Rinex2.html

1. Roger Bate, Donald Mueller, and Jerry White. _Fundamentals of Astrodynamics_. Dover. New York, NY. 1971.

1. A. E. Roy. _Orbital Motion, 2nd Edition_. Adam Hilger Ltd. Bristol, Great Britain. 1982.

1. Battin, Richard. _An Introduction to the Mathematics and Methods of Astrodynamics_, Revised Editino. AIAA Press. Reston, Virginia, 1999.

1. B. Hoffman-Wellenhof, H. Lichtenegger, and J. Collins. GPS: Theory and Practice, 5th
 edition. Springer Verlag.  New York, NY. 2001

1. Misra, Pratap and Enge, Per. _Global Positioning System: Signals, Measurements and Performance_.  Ganga-Jamuna Press. 2004.

1. ICD-GPS-200, now more recently renamed to IS-GPS-200. Website: http://www.navcen.uscg.gov/ftp/policy/icd200/ICD200Cw1234.pdf
