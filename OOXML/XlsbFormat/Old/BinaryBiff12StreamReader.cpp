/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "BinaryBiff12StreamReader.h"

namespace XLSB
{
    CBinaryBiff12StreamReader::CBinaryBiff12StreamReader()
    {

    }

    DWORD CBinaryBiff12StreamReader::CBinaryBiff12StreamReader::GetDword(LPBYTE p)
    {
        return *(DWORD*)p;
    }

    WORD CBinaryBiff12StreamReader::GetWord(LPBYTE p)
    {
        return *(WORD*)p;
    }

    BYTE CBinaryBiff12StreamReader::GetByte(LPBYTE p)
    {
        return *(BYTE*)p;
    }

    double CBinaryBiff12StreamReader::GetFloat(LPBYTE p)
    {

        // When it's a simple precision float, Excel uses a special
        // encoding
        long rk = *(long*)p;
        double d = 0;
        if (rk & 0x02)
        {
            // int
            d = (double) (rk >> 2);
        }
        else
        {
            // hi words of IEEE num
            *((long *)&d+1) = rk & 0xfffffffc;
            *((long *)&d) = 0;
        }
        if (rk & 0x01)
        {
            // divide by 100
            d /= 100;
        }

        return d;
    }

    double CBinaryBiff12StreamReader::GetDouble(LPBYTE p)
    {
        return *(double*)p;
    }

    std::wstring CBinaryBiff12StreamReader::GetString(LPBYTE p, int len)
    {

        LONG lIndex = 0;
        std::wstring res;
        len *= 2;
        while (lIndex < len)
        {
           res.push_back((uint16_t)(p[lIndex]) | ((uint16_t)p[lIndex + 1] << 8));
           lIndex+=2;
        }

        return res;
    }

    bool CBinaryBiff12StreamReader::GetRecordId(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& recid)
    {
        recid = 0;

        if (!p || p > p_last)
            return false;
        BYTE b1 = *(BYTE*)(p++);
        recid = DWORD(b1);

        if ((b1 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b2 = *(BYTE*)(p++);
        recid = (DWORD(b2) << 8) | recid;

        if ((b2 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b3 = *(BYTE*)(p++);
        recid = (DWORD(b3) << 16) | recid;

        if ((b3 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b4 = *(BYTE*)(p++);
        recid = (DWORD(b4) << 24) | recid;

        return true;
    }

    bool CBinaryBiff12StreamReader::GetRecordLength(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& reclen)
    {
        reclen = 0;

        if (!p || p > p_last)
            return false;
        BYTE b1 = *(BYTE*)(p++);
        reclen = DWORD(b1 & 0x7F);

        if ((b1 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b2 = *(BYTE*)(p++);
        reclen = (DWORD(b2 & 0x7F) << 7) | reclen;

        if ((b2 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b3 = *(BYTE*)(p++);
        reclen = (DWORD(b3 & 0x7F) << 14) | reclen;

        if ((b3 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b4 = *(BYTE*)(p++);
        reclen = (DWORD(b4 & 0x7F) << 21) | reclen;

        return true;
    }

    bool CBinaryBiff12StreamReader::GetBit(unsigned char byte, int position) // position in range 0-7
    {
        return (byte >> position) & 0x1;
    }

    DWORD CBinaryBiff12StreamReader::GetBits(unsigned x,int p,int n)
    {
        return (x >> (p+1-n)) & ~(~0 << n);
    }
}
