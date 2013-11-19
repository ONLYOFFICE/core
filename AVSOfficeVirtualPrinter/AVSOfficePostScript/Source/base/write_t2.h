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

/* $Id: write_t2.h 8022 2007-06-05 22:23:38Z giles $ */
/*
Header for functions to serialize a type 2 (CFF) font so that it can
then be passed to FreeType via the FAPI FreeType bridge.
Started by Graham Asher, 9th August 2002.
*/

#ifndef write_t2_INCLUDED
#define write_t2_INCLUDED

#include "ifapi.h"

long FF_serialize_type2_font(FAPI_font* a_fapi_font,unsigned char* a_buffer,long a_buffer_size);

#endif
