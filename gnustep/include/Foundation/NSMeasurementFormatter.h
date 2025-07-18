/**Definition of class NSMeasurementFormatter
   Copyright (C) 2019 Free Software Foundation, Inc.
   
   By: Gregory John Casamento <greg.casamento@gmail.com>
   Date: Mon Sep 30 15:58:21 EDT 2019

   This file is part of the GNUstep Library.
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 31 Milk Street #960789 Boston, MA 02196 USA.
*/

#ifndef _NSMeasurementFormatter_h_GNUSTEP_BASE_INCLUDE
#define _NSMeasurementFormatter_h_GNUSTEP_BASE_INCLUDE

#import <Foundation/NSFormatter.h>

#if OS_API_VERSION(MAC_OS_X_VERSION_10_12, GS_API_LATEST)

#if	defined(__cplusplus)
extern "C" {
#endif

@class NSLocale, NSMeasurement, NSNumberFormatter, NSUnit;

enum {
    NSMeasurementFormatterUnitOptionsProvidedUnit = (1UL << 0),
    NSMeasurementFormatterUnitOptionsNaturalScale = (1UL << 1),
    NSMeasurementFormatterUnitOptionsTemperatureWithoutUnit = (1UL << 2),   
};
typedef NSUInteger NSMeasurementFormatterUnitOptions;

GS_EXPORT_CLASS
@interface NSMeasurementFormatter : NSFormatter <NSCoding>
{
  NSMeasurementFormatterUnitOptions _unitOptions;
  NSFormattingUnitStyle _unitStyle;
  NSLocale *_locale;
  NSNumberFormatter *_numberFormatter;  
}

- (NSMeasurementFormatterUnitOptions) unitOptions;
- (void) setUnitOptions: (NSMeasurementFormatterUnitOptions) unitOptions;
  
- (NSFormattingUnitStyle) unitStyle;
- (void) setUnitStyle: (NSFormattingUnitStyle)style;

- (NSLocale *) locale;
- (void) setLocale: (NSLocale *)locale;

- (NSNumberFormatter *) numberFormatter;
- (void) setNumberFormatter: (NSNumberFormatter *)numberFormatter;  
  
- (NSString *)stringFromMeasurement: (NSMeasurement *)measurement;

- (NSString *)stringFromUnit: (NSUnit *)unit;

@end

#if	defined(__cplusplus)
}
#endif

#endif	/* GS_API_MACOSX */

#endif	/* _NSMeasurementFormatter_h_GNUSTEP_BASE_INCLUDE */

