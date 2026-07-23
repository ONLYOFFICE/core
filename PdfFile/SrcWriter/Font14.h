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
#ifndef _PDF_WRITER_SRC_FONT14_H
#define _PDF_WRITER_SRC_FONT14_H

#include "Font.h"

namespace PdfWriter
{
	class CXref;
	class CDocument;
	class CFont14 : public CFontDict
	{
	public:
		CFont14(CXref* pXref, CDocument* pDocument, EStandard14Fonts eType);
		EFontType GetFontType()
		{
			return fontType1;
		}
		unsigned int GetWidth(unsigned short ushCode);
		void         AddWidth(unsigned int nWidth);
		unsigned short EncodeUnicode(const unsigned int& unGID, const unsigned int& unUnicode, bool& bNew);
		unsigned short EncodeGID(const unsigned int& unGID, bool& bNew);

	private:
		unsigned short                         m_ushCodesCount;
		std::map<unsigned int, unsigned short> m_mUnicodeToCode;
		std::vector<unsigned int>              m_vCodeToGid;
		std::vector<unsigned int>              m_vWidths;

	};

	class CFontEmbedded : public CFontDict
	{
	public:
		CFontEmbedded(CXref* pXref, CDocument* pDocument);

		bool LoadFont(const std::string& sFontKey, EFontType eFontType, CObjectBase* pObj,
					  const std::map<unsigned int, unsigned int>& mCodeToWidth, const std::map<unsigned int, unsigned int>& mCodeToUnicode, const std::map<unsigned int, unsigned int>& mCodeToGID);

		EFontType GetFontType() { return m_eFontType; }
		unsigned int GetWidth(unsigned short ushCode);
		unsigned int EncodeUnicode(const unsigned int& unGID, const unsigned int& unUnicode);
		unsigned int EncodeGID(const unsigned int& unGID);
		CObjectBase* GetObj();
		CObjectBase* GetObj2();
		const char* GetFontKey() const { return m_sFontKey.c_str(); }
		void UpdateKey(const std::string& sFontKey) { m_sFontKey = sFontKey; }
		void SetDW(unsigned int unDW) { m_unDW = unDW; }
		void SetFontPath(const std::wstring& wsFontPath) { m_wsFontPath = wsFontPath; }
		std::wstring GetFontPath() { return m_wsFontPath; }
		void SetFontIndex(unsigned int unIndex) { m_unIndex = unIndex; }
		unsigned int GetFontIndex() { return m_unIndex; }

	private:
		unsigned int m_unDW;
		std::string m_sFontKey;
		EFontType m_eFontType;
		CObjectBase* m_pObj;

		std::wstring m_wsFontPath;
		unsigned int m_unIndex;

		std::map<unsigned int, unsigned int> m_mCodeToUnicode;
		std::map<unsigned int, unsigned int> m_mCodeToGID;
		std::map<unsigned int, unsigned int> m_mCodeToWidth; // Code -> Width in em units
	};
}

#endif // _PDF_WRITER_SRC_FONT14_H
