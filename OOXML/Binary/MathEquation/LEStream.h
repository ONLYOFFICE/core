/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../Base/Base.h"
#include "Types.h"

namespace MathEquation
{
template<class StreamBase>
class CLEStream
{
public :
	
	CLEStream(StreamBase *pStream) : pBuffer(NULL)
	{
		if (NULL == pStream)
			return;

		unsigned int unFileSize = pStream->size();

		pBuffer = new BYTE[unFileSize];
		if (!pBuffer)
			return;

		pStream->read(pBuffer, unFileSize);

		pCur = pBuffer;
		pEnd = pBuffer + unFileSize;
	}

	~CLEStream()
	{
		if (NULL != pBuffer)
			delete pBuffer;
	}

    unsigned char  ReadUInt8()
    {
        if (pCur + 1 >= pEnd)
            return 0;

        unsigned char unResult = pCur[0];
        pCur++;
        return unResult;
    }

    _UINT16 ReadUInt16()
    {
        if (pCur + 2 >= pEnd)
            return 0;

        _UINT16 ushResult = (pCur[0]) | ((pCur[1]) << 8);
        pCur += 2;
        return ushResult;
    }

    _UINT32 ReadUInt32()
    {
        if (pCur + 4 >= pEnd)
            return 0;
        _UINT32 unResult = (_UINT32)((pCur[0]<< 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
        pCur += 4;
        return unResult;
    }

    void operator>>(unsigned char  &nValue)
	{
		nValue = ReadUInt8();
	}

    void operator>>(_UINT16 &nValue)
	{
		nValue = ReadUInt16();
	}

    void operator>>(_UINT32 &nValue)
	{
		nValue = ReadUInt32();
	}

    void operator>>(char   &nValue)
	{
		nValue = ReadUInt8();
	}

    void operator>>(short  &nValue)
	{
        nValue = ReadUInt16();
	}

	bool IsValid() const
	{
		if (NULL == pBuffer)
			return false;

		return true;
	}

	bool IsEof() const
	{
		if (pCur >= pEnd)
			return true;

		return false;
	}



    _UINT32 Tell()
	{
        return (_UINT32)(pCur - pBuffer);
	}

    void SeekRel(_UINT32 nPos)
	{
		pCur += nPos;
	}

private:

	BYTE *pBuffer;
	BYTE *pCur;
	BYTE *pEnd;
};

}
