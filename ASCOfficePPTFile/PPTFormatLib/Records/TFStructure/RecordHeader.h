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

#include "../IRecord.h"

class SRecordHeader
{
public:
    unsigned char	RecVersion;
    unsigned short	RecInstance;
    RecordType      RecType;
    _UINT32	        RecLen;


    void Clear()
    {
        RecVersion = 0;
        RecInstance = 0;
        RecType = 0;
        RecLen = 0;
    }

    SRecordHeader()
    {
        Clear();
    }

    bool ReadFromStream(const CFStreamPtr &pStream)
    {
        Clear();

        if (pStream->isEOF()) return FALSE;
        POLE::uint64 nRd = 0;

        unsigned short rec =0;
        pStream->read((unsigned char*)&(rec), 2);

        RecInstance = rec >> 4;
        RecVersion	= rec - (RecInstance << 4);

        *pStream >> RecType >> RecLen;

        unsigned long sz = pStream->getStreamSize() - pStream->getStreamPointer();

        if (RecLen > sz )
        {
            RecLen = sz;
        }

        return true;
    }

    bool ReadFromStream(POLE::Stream * pStream)
    {
        Clear();
        if (!pStream) return false;

        POLE::uint64 nRd = 0;

        unsigned short rec =0;
        nRd = pStream->read((unsigned char*)&(rec), 2);

        if (nRd != 2) return false;

        RecInstance = rec >> 4;
        RecVersion	= rec - (RecInstance<<4);

        nRd = pStream->read((unsigned char*)&(RecType), 2);

        nRd = pStream->read((unsigned char*)&(RecLen), 4);

        POLE::uint64 sz = pStream->size() - pStream->tell();

        if (RecLen > sz )
        {
            RecLen = (UINT)sz;
        }

        return true;
    }

    bool IsContainer()
    {
        /*if ((RecVersion == PSFLAG_CONTAINER) || ((RecVersion & 0x0F) == 0x0F))
        {
            return TRUE;
        }*/
        if (1064 == RecType)
            return false;

        if (RecVersion == 0x0F)
        {
            return true;
        }
        return false;
    }

    SRecordHeader& operator =(const SRecordHeader& oSrc)
    {
        RecVersion	= oSrc.RecVersion;
        RecInstance = oSrc.RecInstance;
        RecType		= oSrc.RecType;
        RecLen		= oSrc.RecLen;
        return (*this);
    }

};
