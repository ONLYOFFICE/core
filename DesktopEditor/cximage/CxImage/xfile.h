﻿/*
 * File:	xfile.h
 * Purpose:	General Purpose File Class 
 */
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	CxFile (c)  11/May/2002 Davide Pizzolato - www.xdp.it
	CxFile version 2.00 23/Aug/2002
	CxFile version 2.10 16/Dec/2007
	
	Special thanks to Chris Shearer Cooper for new features, enhancements and bugfixes

	Covered code is provided under this license on an "as is" basis, without warranty
	of any kind, either expressed or implied, including, without limitation, warranties
	that the covered code is free of defects, merchantable, fit for a particular purpose
	or non-infringing. The entire risk as to the quality and performance of the covered
	code is with you. Should any covered code prove defective in any respect, you (not
	the initial developer or any other contributor) assume the cost of any necessary
	servicing, repair or correction. This disclaimer of warranty constitutes an essential
	part of this license. No use of any covered code is authorized hereunder except under
	this disclaimer.

	Permission is hereby granted to use, copy, modify, and distribute this
	source code, or portions hereof, for any purpose, including commercial applications,
	freely and without fee, subject to the following restrictions: 

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.
  --------------------------------------------------------------------------------
 */
#if !defined(__xfile_h)
#define __xfile_h

#include <stdio.h>
#include <stdlib.h>

#include "ximadef.h"

class DLL_EXP CxFile
{
public:
	CxFile(void) { }
	virtual ~CxFile() { }

	virtual bool	Close() = 0;
	virtual size_t	Read(void *buffer, size_t size, size_t count, void* limit_start = NULL, void* limit_end = NULL) = 0;
	virtual size_t	Write(const void *buffer, size_t size, size_t count) = 0;
	virtual bool	Seek(int32_t offset, int32_t origin) = 0;
	virtual int32_t	Tell() = 0;
	virtual int32_t	Size() = 0;
	virtual bool	Flush() = 0;
	virtual bool	Eof() = 0;
	virtual int32_t	Error() = 0;
	virtual bool	PutC(uint8_t c)
		{
		// Default implementation
		size_t nWrote = Write(&c, 1, 1);
		return (bool)(nWrote == 1);
		}
	virtual int32_t	GetC() = 0;
	virtual char *	GetS(char *string, int32_t n) = 0;
	virtual int32_t	Scanf(const char *format, void* output) = 0;
};

static void clamp_buffer(void*& buffer, size_t& size, void* limit_start, void* limit_end)
{
	if (NULL == limit_start || NULL == limit_end)
		return;

	uint8_t* _buffer = (uint8_t*)buffer;
	uint8_t* _limit_start = (uint8_t*)limit_start;
	uint8_t* _limit_end = (uint8_t*)limit_end;

	if (_buffer > _limit_end)
	{
		buffer = limit_end;
		size = 0;
		return;
	}

	if (_buffer < _limit_start)
		_buffer = _limit_start;

	if ((_buffer + size) > _limit_end)
		size = (_limit_end - _buffer);
}

#endif //__xfile_h
