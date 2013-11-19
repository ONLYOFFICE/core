/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/
/* $Id: gdevmacttf.h 8766 2008-05-21 19:46:43Z giles $ */
/* Datatypes for simpler access to data structures in TrueType fonts */

#ifndef gdevmacttf_INCLUDED
#  define gdevmacttf_INCLUDED


/* Font Directory Component */

typedef struct {
	UInt32		tagName;
	UInt32		checkSum;
	UInt32		offset;
	UInt32		length;
} TTFontDirComponent;



/* Font Directory */

typedef struct {
	UInt32				version;
	UInt16				numTables;
	UInt16				searchRange;
	UInt16				entrySelector;
	UInt16				rangeShift;
	TTFontDirComponent	components[1];	/* an array of numTables components */
} TTFontDir;



/* Tag definitions */
#define		TTF_FONT_NAMING_TABLE	'name'



/* Font Naming Table */

typedef struct {
	UInt16				formatSelector;
	UInt16				numNames;
	UInt16				stringAreaOffset;
	UInt16				platformID;
	UInt16				platformSpecificID;
	UInt16				languageID;
	UInt16				nameID;
	UInt16				length;
	UInt16				offset;
} TTFontNamingTable;

#endif /* gdevmacttf_INCLUDED */
