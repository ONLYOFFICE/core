/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../Reader/Records.h"

class CRecordFontEntityAtom : public CUnknownRecord
{
public:
	CStringW m_strFaceName;

    BYTE m_lfCharSet;

	bool m_bEmbedSubsetted;
	bool m_bIsRaster;
	bool m_bIsDevice;
	bool m_bIsTrueType;
	bool m_bIsNoFontSubstitution;

    BYTE m_lfPitchAndFamily;

public:
	
	CRecordFontEntityAtom()
	{
	}

	~CRecordFontEntityAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

        //face name - utf16 string with 0, 64 bytes always allocated
        unsigned char utf16FaceName[64+2] = {};

        ULONG lReadByte = pStream->read(utf16FaceName, 64);

        if (sizeof(wchar_t) == 4)
        {
            int lLen  = 0;
            for (lLen = 0; lLen < lReadByte; lLen +=2)
                if (utf16FaceName[lLen] == 0)break;

            lLen/=2;

            UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
            pStrUtf32[lLen] = 0 ;

            const	UTF16 *pStrUtf16_Conv = (const UTF16 *) utf16FaceName;
                    UTF32 *pStrUtf32_Conv =                 pStrUtf32;

             if (conversionOK == ConvertUTF16toUTF32 ( &pStrUtf16_Conv, &pStrUtf16_Conv[lLen]
                                               , &pStrUtf32_Conv, &pStrUtf32 [lLen]
                                               , strictConversion))
            {
                m_strFaceName = CString((wchar_t*)pStrUtf32/*, lLen*/);
            }
            delete [] pStrUtf32;
         }
        else
        {
            m_strFaceName = CString((wchar_t*)utf16FaceName/*, lReadByte/2*/); // по факту .. нули нам не нужны
        }
        m_lfCharSet = StreamUtils::ReadBYTE(pStream);
		
		BYTE Mem = 0;
		Mem = StreamUtils::ReadBYTE(pStream);
		m_bEmbedSubsetted = ((Mem & 0x01) == 0x01);

		Mem = 0;
		Mem = StreamUtils::ReadBYTE(pStream);

		m_bIsRaster = ((Mem & 0x01) == 0x01);
		m_bIsDevice = ((Mem & 0x02) == 0x02);
		m_bIsTrueType = ((Mem & 0x04) == 0x04);
		m_bIsNoFontSubstitution = ((Mem & 0x08) == 0x08);

		m_lfPitchAndFamily = StreamUtils::ReadBYTE(pStream);
	}
};
