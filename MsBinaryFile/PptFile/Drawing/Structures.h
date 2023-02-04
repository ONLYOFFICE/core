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
#include "Attributes.h"

#define GETBIT(from, num) ((from & (1 << num)) != 0)
#define GETBITS(from, numL, numH) ((from & (((1 << (numH - numL + 1)) - 1) << numL)) >> numL)

typedef std::vector<std::wstring> vector_string;

namespace ODRAW
{
	static void CorrectColorPPT(_INT32& lSchemeIndex)
    {
        //0x00 //Background color
        //0x01 //Text color
        //0x02 //Shadow color
        //0x03 //Title text color
        //0x04 //Fill color
        //0x05 //Accent 1 color
        //0x06 //Accent 2 color
        //0x07 //Accent 3 color

        // [0]);//0
        // [1]);//1
        // [2]);//2
        // [3]);//3
        // [0]);//4
        // [4]);//5 //accent1
        // [5]);//6 //accent2
        // [0]);//7 //accent3
        // [5]);//8 //accent4
        // [4]);//9 //accent5
        // [7]);//10 //accent6
        // [6]);//11 //hlink
        // [7]);//12 //folHlink
        // [0]);//13 //lt1
        // [1]);//14 //dk1
        // [2]);//15 //lt2
        // [3]);//16 //dk2

        switch (lSchemeIndex)
        {
        case 0:
            lSchemeIndex = 13;
            break;
        case 1:
            lSchemeIndex = 14;
            break;
        case 2:
            lSchemeIndex = 15;
            break;
        case 3:
            lSchemeIndex = 16;
            break;
        case 4:
            lSchemeIndex = 5;
            break;
        case 5:
            lSchemeIndex = 6;
            break;
        case 6:
            lSchemeIndex = 11;
            break;
        case 7:
            lSchemeIndex = 12;
            break;
        default:
            lSchemeIndex = -1;
            break;
        }
    }

    struct SPointAtom
    {
        _INT32 X;
        _INT32 Y;
    };

    struct SRectAtom
    {
        _INT32 Left;
        _INT32 Top;
        _INT32 Right;
        _INT32 Bottom;
    };

    struct SSmallRectAtom
    {
        _INT16 Left;
        _INT16 Top;
        _INT16 Right;
        _INT16 Bottom;
    };

    struct SColorAtom
    {
        BYTE R;
        BYTE G;
        BYTE B;
        BYTE Index;

        bool bPaletteIndex;
        bool bPaletteRGB;
        bool bSystemRGB;
        bool bSchemeIndex;
        bool bSysIndex;

        SColorAtom()
        {
            R = 0;
            G = 0;
            B = 0;
            Index = -1;

            bPaletteIndex	= false;
            bPaletteRGB		= false;
            bSystemRGB		= false;
            bSchemeIndex	= false;
            bSysIndex		= false;
        }

        SColorAtom(const SColorAtom& oSrc)
        {
            *this = oSrc;
        }

        SColorAtom& operator=(const SColorAtom& oSrc)
        {
            R = oSrc.R;
            G = oSrc.G;
            B = oSrc.B;

            Index = oSrc.Index;

            bPaletteIndex	= oSrc.bPaletteIndex;
            bPaletteRGB		= oSrc.bPaletteRGB;
            bSystemRGB		= oSrc.bSystemRGB;
            bSchemeIndex	= oSrc.bSchemeIndex;
            bSysIndex		= oSrc.bSysIndex;

            return *this;
        }
        SColorAtom& operator=(const CColor& oSrc)
        {
            R = oSrc.R;
            G = oSrc.G;
            B = oSrc.B;

            return *this;
        }

        _UINT32 ToValue()
        {
            _UINT32 dwVal = (R | (G << 8) | (B << 16));
            return dwVal;
        }
        _UINT32 ToValue_RGB()
        {
            _UINT32 dwVal = (B | (G << 8) | (R << 16));
            return dwVal;
        }

        _UINT32 ToValueProperty()
        {
            _UINT32 dwVal = 0;
            if (!bSchemeIndex)
            {
                dwVal = (R | (G << 8) | (B << 16));
            }
            else
            {
                dwVal = (R | 0x01000000);
            }
            return dwVal;
        }

        void FromValue(BYTE _R, BYTE _G, BYTE _B)
        {
            R = _R;
            G = _G;
            B = _B;
            Index = -1;
        }

        void FromValue(_UINT32 dwValue)
        {

            R	= static_cast<unsigned char>(GETBITS(dwValue, 0, 7));
            G	= static_cast<unsigned char>(GETBITS(dwValue, 8, 15));
            B	= static_cast<unsigned char>(GETBITS(dwValue, 16, 23));

            Index = -1;

            bPaletteIndex	= GETBIT(dwValue, 24);
            bPaletteRGB		= GETBIT(dwValue, 25);
            bSystemRGB		= GETBIT(dwValue, 26);
            bSchemeIndex	= GETBIT(dwValue, 27);
            bSysIndex		= GETBIT(dwValue, 28);

            /*if(!bSchemeIndex && !bPaletteIndex && !bSysIndex)
                            {
                                    colorRGB = STR::toRGB(red, green, blue);
                            }
                            else */
            if(bSchemeIndex)
            {
                Index = R;
            }
            else if(bPaletteIndex)
            {
                Index = ((G) << 8) + R;
            }
            else if(bSysIndex)
            {
                Index = ((G) << 8) + R;
            }
        }

        void ToColor(CColor* pColor)
        {
            pColor->R = R;
            pColor->G = G;
            pColor->B = B;

            pColor->m_lSchemeIndex = -1;

            if (bSchemeIndex || bSysIndex)
            {
                pColor->m_lSchemeIndex = R;
                CorrectColorPPT(pColor->m_lSchemeIndex);
            }
        }
    };

    struct STextRange
    {
        UINT Begin;
        UINT End;
    };

 }
