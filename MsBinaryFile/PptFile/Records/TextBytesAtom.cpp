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
#include "TextBytesAtom.h"

using namespace PPT;

CRecordTextBytesAtom::CRecordTextBytesAtom()
{
}

CRecordTextBytesAtom::~CRecordTextBytesAtom()
{
}

void CRecordTextBytesAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //UTF-16 Unicode  character whose high byte is 0x00.
    unsigned short	*pUTF16		= new unsigned short[m_oHeader.RecLen];
    unsigned char	*pUTF16_low = new unsigned char	[m_oHeader.RecLen];

    if (pUTF16 && pUTF16_low)
    {
        pStream->read(pUTF16_low, m_oHeader.RecLen);

        for (UINT i = 0 ; i < m_oHeader.RecLen; i++)
        {
            pUTF16[i] = pUTF16_low[i];
        }

        m_strText = NSFile::CUtf8Converter::GetWStringFromUTF16(pUTF16, m_oHeader.RecLen);

    }

    RELEASEARRAYOBJECTS(pUTF16_low);
    RELEASEARRAYOBJECTS(pUTF16);

    //std::string tmpStrTextA = StreamUtils::ReadCStringA(pStream, m_oHeader.RecLen);

    //std::wstring tmpStrTextW (tmpStrTextA.begin(), tmpStrTextA.end());

    //m_strText = std_string2string(tmpStrTextW);
}
