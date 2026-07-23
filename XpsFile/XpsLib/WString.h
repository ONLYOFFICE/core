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
#ifndef _XPS_XPSLIB_WSTRING_H
#define _XPS_XPSLIB_WSTRING_H

#include <vector>
#include <string>

namespace XPS
{
	class CWStringBuffer;
	class CWString
	{
	public:
		CWString();
		CWString(const wchar_t* wsString);
        CWString(const std::wstring& wsString);
		CWString(const CWString& wsString);
		CWString(wchar_t* wsString, bool bCopy, int nLen = -1);
		~CWString();
		void create(const wchar_t*, bool bCopy, int nLen = -1);
        void create(const std::wstring& sString, bool bCopy);
		void operator=(const wchar_t* wsString);
		void operator=(const CWString& wsString);
		bool operator<(const CWString& wsString) const;
		bool operator>(const CWString& wsString) const;
		bool operator==(const CWString& wsString) const;
		bool operator==(const wchar_t* wsString) const;
		unsigned int size() const;
		bool empty() const;
		wchar_t operator[](const unsigned int& unIndex) const;
		const wchar_t* c_str() const;
        const std::wstring c_stdstr() const;
		void clear();

        int tointeger() const;

		std::vector<CWString> split(wchar_t wChar, bool bCopy = false);

	private:

		void*        m_pBuffer;
		unsigned int m_unLen;
		bool         m_bOwnBuffer;
	};
}

#endif //_XPS_XPSLIB_WSTRING_H
