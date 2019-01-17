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
