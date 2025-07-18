
/* Definition of class NSAppleScript
   Copyright (C) 2019 Free Software Foundation, Inc.
   
   By: Gregory John Casamento <greg.casamento@gmail.com>
   Date: Thu Sep 26 09:49:55 EDT 2019

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

#ifndef _NSAppleScript_h_GNUSTEP_BASE_INCLUDE
#define _NSAppleScript_h_GNUSTEP_BASE_INCLUDE

#include <Foundation/NSObject.h>

#if	defined(__cplusplus)
extern "C" {
#endif

#if OS_API_VERSION(MAC_OS_X_VERSION_10_0, GS_API_LATEST)

GS_EXPORT_CLASS
@interface NSAppleScript : NSObject

@end

#if	defined(__cplusplus)
}
#endif

#endif	/* GS_API_MACOSX */

#endif	/* _NSAppleScript_h_GNUSTEP_BASE_INCLUDE */

