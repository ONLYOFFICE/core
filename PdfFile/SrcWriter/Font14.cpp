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
#include "Font14.h"
#include "Document.h"

namespace PdfWriter
{
	static const char* c_sStandardFontNames[] =
	{
		"Helvetica",
		"Helvetica-Bold",
		"Helvetica-Oblique",
		"Helvetica-BoldOblique",
		"Courier",
		"Courier-Bold",
		"Courier-Oblique",
		"Courier-BoldOblique",
		"Times-Roman",
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
		if (eType < EStandard14Fonts::standard14fonts_Symbol)
			Add("Encoding", "WinAnsiEncoding");
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

	CFontEmbedded::CFontEmbedded(CXref* pXref, CDocument* pDocument) : CFontDict(pXref, pDocument), m_eFontType(fontUnknownType)
	{
	}
	bool CFontEmbedded::LoadFont(const std::string& sFontKey, EFontType eFontType, CObjectBase* pObj,
								 const std::map<unsigned int, unsigned int>& mCodeToWidth, const std::map<unsigned int, unsigned int>& mCodeToUnicode, const std::map<unsigned int, unsigned int>& mCodeToGID)
	{
		m_unDW = 1000;
		m_sFontKey = sFontKey;
		m_eFontType = eFontType;
		m_pObj = pObj;
		m_mCodeToWidth = mCodeToWidth;
		m_mCodeToUnicode = mCodeToUnicode;
		m_mCodeToGID = mCodeToGID;

		return true;
	}
	unsigned int CFontEmbedded::GetWidth(unsigned short ushCode)
	{
		// Return width from the map
		auto it = m_mCodeToWidth.find(ushCode);
		if (it != m_mCodeToWidth.end())
			return it->second;

		return m_unDW;
	}
	unsigned int CFontEmbedded::EncodeUnicode(const unsigned int& unGID, const unsigned int& unUnicode)
	{
		if (m_eFontType == fontCIDType0   ||
			m_eFontType == fontCIDType0C  ||
			m_eFontType == fontCIDType0COT||
			m_eFontType == fontCIDType2   ||
			m_eFontType == fontCIDType2OT)
		{
			for (auto& pair : m_mCodeToUnicode)
				if (pair.second == unUnicode)
					return pair.first;

			unsigned int ushCode = EncodeGID(unGID);
			if (ushCode)
				m_mCodeToUnicode.insert(std::pair<unsigned int, unsigned short>(ushCode, unUnicode));
			return ushCode;
		}

		unsigned int ushCode = EncodeGID(unGID);
		if (ushCode)
			return ushCode;

		for (auto& pair : m_mCodeToUnicode)
			if (pair.second == unUnicode)
				return pair.first;

		return 0;
	}
	unsigned int CFontEmbedded::EncodeGID(const unsigned int& unGID)
	{
		for (auto& pair : m_mCodeToGID)
			if (pair.second == unGID)
				return pair.first;
		return 0;
	}
	CObjectBase* CFontEmbedded::GetObj()
	{
		if (m_pObj->GetType() != object_type_UNKNOWN)
			return m_pObj;
		return new PdfWriter::CProxyObject(m_pObj);
	}
	CObjectBase* CFontEmbedded::GetObj2()
	{
		return m_pObj;
	}
}
