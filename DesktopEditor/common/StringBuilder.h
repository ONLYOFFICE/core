/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#ifndef _BUILD_STRING_BUILDER_CROSSPLATFORM_H_
#define _BUILD_STRING_BUILDER_CROSSPLATFORM_H_

#include <string>
#include <string.h>
#include <algorithm>
#include "Types.h"

#ifndef STRINGBUILDER_USE_DYNAMIC_LIBRARY
#define STRINGBUILDER_DECL_EXPORT
#else
#include "./base_export.h"
#define STRINGBUILDER_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace NSStringUtils
{
    class STRINGBUILDER_DECL_EXPORT CStringBuilderA
	{
	private:
		char*		m_pData;
		size_t		m_lSize;

		char*		m_pDataCur;
		size_t		m_lSizeCur;

	public:
        CStringBuilderA();
        ~CStringBuilderA();
        void AddSize(size_t nSize);

	public:
        void SetText(const std::string& sText);
        void WriteString(const std::string& str);

        void WriteStringNoSafe(const char* pString, size_t nLen);
        void WriteString(const char* pString, size_t nLen);

        void AddCharNoSafe(const char& _c);
        void AddCharSafe(const char& _c);

        size_t GetCurSize();
        void SetCurSize(size_t lCurSize);
        size_t GetSize();

        void Clear();
        void ClearNoAttack();

        std::string GetData();
        char* GetBuffer();
	};

    class STRINGBUILDER_DECL_EXPORT CStringBuilder
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
        CStringBuilder();
        ~CStringBuilder();

        void AddSize(size_t nSize);

	public:

        void SetText(const std::wstring& bsText);

        void operator+=(const std::wstring& oTemp);

        void WriteStringNoSafe(const wchar_t* pString, size_t nLen);
        void WriteString(const wchar_t* pString, size_t nLen = -1);

        void WriteString(const std::wstring& sString);

        void AddCharNoSafe(const wchar_t& _c);
        void AddCharNoCheck(const wchar_t& _c);
        void AddSpaceNoCheck();
        void AddCharSafe(const wchar_t& _c);
        void AddChar2Safe(const wchar_t _c1, const wchar_t& _c2);

        void WriteEncodeXmlString(const std::wstring& sString);

        void WriteEncodeXmlString(const wchar_t* pString, int nCount = -1);

        size_t GetCurSize();
        void SetCurSize(size_t lCurSize);
        size_t GetSize();

        void Write(CStringBuilder& oWriter, const size_t& offset = 0);

        void Clear();
        void ClearNoAttack();

        std::wstring GetData();
        wchar_t* GetBuffer();

        void RemoveLastSpaces();
        bool IsSpace();

        void AddInt(int val);
        void AddIntDel10(int val);
        void AddIntDel100(int val);
        void AddInt64(__int64 val);

        void AddIntNoCheck(int val);

        void AddIntNoCheckDel10(int val);
        void AddIntNoCheckDel100(int val);
        void AddDouble(double val, int count);

        inline void WriteHexByteNoSafe(const unsigned char& value);
        inline void WriteHexByte(const unsigned char& value);
        inline void WriteHexInt3(const unsigned int& value);
        inline void WriteHexColor3(const unsigned char& r, const unsigned char& g, const unsigned char& b);
        inline void WriteHexColor3(const unsigned int& value);

	protected:
        unsigned char CheckCode(const wchar_t& c);
	};

    STRINGBUILDER_DECL_EXPORT void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo);
    STRINGBUILDER_DECL_EXPORT void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo);
}

#endif // _BUILD_STRING_BUILDER_CROSSPLATFORM_H_
