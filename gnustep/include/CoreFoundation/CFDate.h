/* CFDate.h

   Copyright (C) 2010 Free Software Foundation, Inc.

   Written by: Stefan Bidigaray
   Date: January, 2010

   This file is part of GNUstep CoreBase Library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.         See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; see the file COPYING.LIB.
   If not, see <http://www.gnu.org/licenses/> or write to the 
   Free Software Foundation, 51 Franklin Street, Fifth Floor, 
   Boston, MA 02110-1301, USA.
*/

#ifndef __COREFOUNDATION_CFDATE_H__
#define __COREFOUNDATION_CFDATE_H__

#include <CoreFoundation/CFBase.h>

CF_EXTERN_C_BEGIN

/** \ingroup CFDateRef */
typedef const struct __CFDate *CFDateRef;
/** \ingroup CFTimeZoneRef */
typedef const struct __CFTimeZone *CFTimeZoneRef;

/** \defgroup TimeUtils Time Utilities
    \{
 */
/** \name Data Types
    \{
 */
typedef double CFTimeInterval;
typedef CFTimeInterval CFAbsoluteTime;

typedef struct CFGregorianDate CFGregorianDate;
struct CFGregorianDate
{
  SInt32 year;
  SInt8 month;
  SInt8 day;
  SInt8 hour;
  SInt8 minute;
  double second;
};

typedef struct CFGregorianUnits CFGregorianUnits;
struct CFGregorianUnits
{
  SInt32 years;
  SInt32 months;
  SInt32 days;
  SInt32 hours;
  SInt32 minutes;
  double seconds;
};

typedef enum
{
  kCFGregorianUnitsYears = (1 << 0),
  kCFGregorianUnitsMonths = (1 << 1),
  kCFGregorianUnitsDays = (1 << 2),
  kCFGregorianUnitsHours = (1 << 3),
  kCFGregorianUnitsMinutes = (1 << 4),
  kCFGregorianUnitsSeconds = (1 << 5),
  kCFGregorianAllUnits = 0x00FFFFFF
} CFGregorianUnitFlags;
/** \} */

/** \name Constants
    \{
 */
CF_EXPORT const CFTimeInterval kCFAbsoluteTimeIntervalSince1970;
CF_EXPORT const CFTimeInterval kCFAbsoluteTimeIntervalSince1904;
/** \} */

/** \name Time Utilities Functions
    \{
 */
CF_EXPORT CFAbsoluteTime
CFAbsoluteTimeAddGregorianUnits (CFAbsoluteTime at, CFTimeZoneRef tz,
                                 CFGregorianUnits units);

CF_EXPORT CFAbsoluteTime CFAbsoluteTimeGetCurrent (void);

CF_EXPORT SInt32
CFAbsoluteTimeGetDayOfWeek (CFAbsoluteTime at, CFTimeZoneRef tz);

CF_EXPORT SInt32
CFAbsoluteTimeGetDayOfYear (CFAbsoluteTime at, CFTimeZoneRef tz);

CF_EXPORT CFGregorianUnits
CFAbsoluteTimeGetDifferenceAsGregorianUnits (CFAbsoluteTime at1,
                                             CFAbsoluteTime at2,
                                             CFTimeZoneRef tz,
                                             CFOptionFlags unitFlags);

CF_EXPORT CFGregorianDate
CFAbsoluteTimeGetGregorianDate (CFAbsoluteTime at, CFTimeZoneRef tz);

CF_EXPORT SInt32
CFAbsoluteTimeGetWeekOfYear (CFAbsoluteTime at, CFTimeZoneRef tz);

CF_EXPORT CFAbsoluteTime
CFGregorianDateGetAbsoluteTime (CFGregorianDate gdate, CFTimeZoneRef tz);

CF_EXPORT Boolean
CFGregorianDateIsValid (CFGregorianDate gdate, CFOptionFlags unitFlags);
/** \} */
/** \} */

/** \defgroup CFDateRef CFData Reference
    \{
 */
/** \name CFDate Functions
    \{
 */
CF_EXPORT CFComparisonResult
CFDateCompare (CFDateRef theDate, CFDateRef otherDate, void *context);

CF_EXPORT CFDateRef CFDateCreate (CFAllocatorRef allocator, CFAbsoluteTime at);

CF_EXPORT CFAbsoluteTime CFDateGetAbsoluteTime (CFDateRef theDate);

CF_EXPORT CFTimeInterval
CFDateGetTimeIntervalSinceDate (CFDateRef theDate, CFDateRef otherDate);

CF_EXPORT CFTypeID CFDateGetTypeID (void);
/** \} */
/** \} */

CF_EXTERN_C_END
#endif /* __COREFOUNDATION_CFDATE_H__ */
