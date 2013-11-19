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

/* $Id: wrfont.c 8022 2007-06-05 22:23:38Z giles $ */
/*
Support functions to serialize fonts as PostScript code that can
then be passed to FreeType via the FAPI FreeType bridge.
Started by Graham Asher, 9th August 2002.
*/

#include "wrfont.h"
#include "stdio_.h"

#define EEXEC_KEY 55665
#define EEXEC_FACTOR 52845
#define EEXEC_OFFSET 22719

void WRF_init(WRF_output* a_output,unsigned char* a_buffer,long a_buffer_size)
	{
	a_output->m_pos = a_buffer;
	a_output->m_limit = a_buffer_size;
	a_output->m_count = 0;
	a_output->m_encrypt = false;
	a_output->m_key = EEXEC_KEY;
	}

void WRF_wbyte(WRF_output* a_output,unsigned char a_byte)
	{
	if (a_output->m_count < a_output->m_limit)
		{
		if (a_output->m_encrypt)
			{
			a_byte ^= (a_output->m_key >> 8);
			a_output->m_key = (unsigned short)((a_output->m_key + a_byte) * EEXEC_FACTOR + EEXEC_OFFSET);
			}
		*a_output->m_pos++ = a_byte;
		}
	a_output->m_count++;
	}

void WRF_wtext(WRF_output* a_output,const unsigned char* a_string,long a_length)
	{
	while (a_length > 0)
		{
		WRF_wbyte(a_output,*a_string++);
		a_length--;
		}
	}

void WRF_wstring(WRF_output* a_output,const char* a_string)
	{
	while (*a_string)
		WRF_wbyte(a_output,*a_string++);
	}

void WRF_wfloat(WRF_output* a_output,double a_float)
	{
	char buffer[32];
	sprintf(buffer,"%f",a_float);
	WRF_wstring(a_output,buffer);
	}

void WRF_wint(WRF_output* a_output,long a_int)
	{
	char buffer[32];
	sprintf(buffer,"%ld",a_int);
	WRF_wstring(a_output,buffer);
	}
