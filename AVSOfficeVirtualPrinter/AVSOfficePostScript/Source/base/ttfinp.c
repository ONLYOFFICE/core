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

/* $Id: ttfinp.c 8022 2007-06-05 22:23:38Z giles $ */
/* A TT font input support. */


#include "ttmisc.h"

#include "ttfoutl.h"
#include "ttfsfnt.h"
#include "ttfinp.h"

unsigned char ttfReader__Byte(ttfReader *r)
{   unsigned char b; 

    r->Read(r, &b, 1); 
    return b;
}

signed char ttfReader__SignedByte(ttfReader *r)
{   signed char b; 

    r->Read(r, &b, 1); 
    return b;
}

signed short ttfReader__Short(ttfReader *r)
{   unsigned char buf[2]; 

    r->Read(r, buf, 2);
    return ((int16)buf[0] << 8) | (int16)buf[1];
}

unsigned short ttfReader__UShort(ttfReader *r)
{   unsigned char buf[2]; 

    r->Read(r, buf, 2);
    return ((uint16)buf[0] << 8) | (uint16)buf[1];
}

unsigned int ttfReader__UInt(ttfReader *r)
{   unsigned char buf[4]; 

    r->Read(r, buf, 4);
    return ((int32)buf[0] << 24) | ((int32)buf[1] << 16) | 
	   ((int32)buf[2] <<  8) |  (int32)buf[3];
}

signed int ttfReader__Int(ttfReader *r)
{   
    return (int)ttfReader__UInt(r);
}

