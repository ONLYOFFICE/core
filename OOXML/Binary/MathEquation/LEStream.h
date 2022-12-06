/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
