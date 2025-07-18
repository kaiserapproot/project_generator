/* CFNumber.h

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

#ifndef __COREFOUNDATION_CFNUMBER_H__
#define __COREFOUNDATION_CFNUMBER_H__

#include <CoreFoundation/CFBase.h>

CF_EXTERN_C_BEGIN
/** \ingroup CFBooleanRef */
typedef const struct __CFBoolean *CFBooleanRef;
/** \ingroup CFNumberRef */
typedef const struct __CFNumber *CFNumberRef;

/** \defgroup CFBooleanRef CFBoolean Reference
    \{
 */
CF_EXPORT const CFBooleanRef kCFBooleanTrue;
CF_EXPORT const CFBooleanRef kCFBooleanFalse;

CF_EXPORT CFTypeID CFBooleanGetTypeID (void);

CF_EXPORT Boolean CFBooleanGetValue (CFBooleanRef boolean);
/** \} */

/** \defgroup CFNumberRef CFNumber Reference
    \{
 */
CF_EXPORT const CFNumberRef kCFNumberNaN;
CF_EXPORT const CFNumberRef kCFNumberNegativeInfinity;
CF_EXPORT const CFNumberRef kCFNumberPositiveInfinity;

typedef enum
{
  kCFNumberSInt8Type = 1,
  kCFNumberSInt16Type = 2,
  kCFNumberSInt32Type = 3,
  kCFNumberSInt64Type = 4,
  kCFNumberFloat32Type = 5,
  kCFNumberFloat64Type = 6,
  kCFNumberCharType = 7,
  kCFNumberShortType = 8,
  kCFNumberIntType = 9,
  kCFNumberLongType = 10,
  kCFNumberLongLongType = 11,
  kCFNumberFloatType = 12,
  kCFNumberDoubleType = 13,
  kCFNumberCFIndexType = 14,
#if OS_API_VERSION(MAC_OS_X_VERSION_10_5, GS_API_LATEST)
  kCFNumberNSIntegerType = 15,
  kCFNumberCGFloatType = 16,
  kCFNumberMaxType = 16
#else
  kCFNumberMaxType = 14
#endif
} CFNumberType;

/** \name Creating a Number
    \{
 */
CF_EXPORT CFNumberRef
CFNumberCreate (CFAllocatorRef allocator, CFNumberType theType,
                const void *valuePtr);
/** \} */

/** \name Getting Information About Numbers
    \{
 */
CF_EXPORT CFIndex CFNumberGetByteSize (CFNumberRef number);

CF_EXPORT CFNumberType CFNumberGetType (CFNumberRef number);

CF_EXPORT Boolean
CFNumberGetValue (CFNumberRef number, CFNumberType theType, void *valuePtr);

CF_EXPORT Boolean CFNumberIsFloatType (CFNumberRef number);
/** \} */

/** \name Comparing Numbers
    \{
 */
CF_EXPORT CFComparisonResult
CFNumberCompare (CFNumberRef number, CFNumberRef otherNumber, void *context);
/** \} */

/** \name Getting the CFNumber Type ID
    \{
 */
CF_EXPORT CFTypeID CFNumberGetTypeID (void);
/** \} */
/** \} */

CF_EXTERN_C_END
#endif /* __COREFOUNDATION_CFNUMBER_H__ */
