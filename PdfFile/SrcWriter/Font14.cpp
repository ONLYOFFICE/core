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
#include "Font14.h"
#include "Document.h"

namespace PdfWriter
{
	static const char* c_sStandardFontNames[] =
	{
		"Helvetica",
		"Helvetica-Bold",
		"Helvetica-Oblique",
		"Helvetice-BoldOblique",
		"Courier",
		"Courier-Bold",
		"Courier-Oblique",
		"Courier-BoldOblique",
		"Times",
		"Times-Bold",
		"Times-Oblique",
		"Times-BoldOblique",
		"Symbol",
		"ZapfDingbats",
		NULL
	};
	CFont14::CFont14(CXref* pXref, CDocument* pDocument, EStandard14Fonts eType) : CFontDict(pXref, pDocument)
	{
		Add("Type", "Font");
		Add("Subtype", "Type1");
		Add("BaseFont", c_sStandardFontNames[(int)eType]);
		m_ushCodesCount = 0;
	}
	unsigned int CFont14::GetWidth(unsigned short ushCode)
	{
		std::map<unsigned int, unsigned short>::const_iterator oIter = m_mUnicodeToCode.find(ushCode);
		if (oIter == m_mUnicodeToCode.end())
			return 0;
		ushCode = oIter->second;
		if (ushCode >= m_vWidths.size())
			return 0;

		return m_vWidths.at(ushCode);
	}
	void CFont14::AddWidth(unsigned int nWidth)
	{
		m_vWidths.push_back(nWidth);
	}
	unsigned short CFont14::EncodeUnicode(const unsigned int& unGID, const unsigned int& unUnicode, bool& bNew)
	{
		std::map<unsigned int, unsigned short>::const_iterator oIter = m_mUnicodeToCode.find(unUnicode);
		if (oIter != m_mUnicodeToCode.end())
			return oIter->second;

		unsigned short ushCode = EncodeGID(unGID, bNew);
		m_mUnicodeToCode.insert(std::pair<unsigned int, unsigned short>(unUnicode, ushCode));
		return ushCode;
	}
	unsigned short CFont14::EncodeGID(const unsigned int& unGID, bool& bNew)
	{
		for (unsigned short ushCurCode = 0, ushCodesCount = m_vCodeToGid.size(); ushCurCode < ushCodesCount; ushCurCode++)
		{
			if (unGID == m_vCodeToGid.at(ushCurCode))
				return ushCurCode;
		}

		unsigned short ushCode = m_ushCodesCount++;

		m_vCodeToGid.push_back(unGID);
		bNew = true;

		return ushCode;
	}
}
