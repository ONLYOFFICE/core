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

#include <string>
#include <string.h>
#include <algorithm>
#include "Types.h"

#include "../../Common/kernel_config.h"

namespace NSStringUtils
{
	class KERNEL_DECL CStringBuilderA
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

	class KERNEL_DECL CStringBuilder
	{
	protected:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringBuilder();
		virtual ~CStringBuilder();

		virtual void AddSize(size_t nSize);

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

		void WriteEncodeXmlStringHHHH(const std::wstring& sString);
		void WriteEncodeXmlStringHHHH(const wchar_t* pString, int nCount = -1);

		size_t GetCurSize();
		void SetCurSize(size_t lCurSize);
		size_t GetSize();

		void Write(CStringBuilder& oWriter, const size_t& offset = 0);
		void WriteBefore(CStringBuilder& oWriter);

		void Clear();
		void ClearNoAttack();

		std::wstring GetData();
		std::wstring GetSubData(const size_t& start = 0, const size_t& count = std::wstring::npos);
		wchar_t* GetBuffer();

		void RemoveLastSpaces();
		bool IsSpace();

		void AddBool2(bool val);
		void AddInt(int val);
		void AddUInt(unsigned int val);
		void AddIntDel10(int val);
		void AddIntDel100(int val);
		void AddInt64(__int64 val);

		void AddIntNoCheck(int val);

		void AddIntNoCheckDel10(int val);
		void AddIntNoCheckDel100(int val);
		void AddDouble(double val, int count);

		void WriteHexByteNoSafe(const unsigned char& value);
		void WriteHexByte(const unsigned char& value);
		void WriteHexInt3(const unsigned int& value);
		void WriteHexInt4(const unsigned int& value);
		void WriteHexColor3(const unsigned char& r, const unsigned char& g, const unsigned char& b);
		void WriteHexColor3(const unsigned int& value);

		void Skip(int nSkip);

		void StartNode(const std::wstring& name);
		void StartNodeWithNS(const std::wstring& ns, const std::wstring& name);
		void StartAttributes();
		void EndAttributes();
		void EndAttributesAndNode();
		void EndNode(const std::wstring& name);
		void EndNodeWithNS(const std::wstring& ns, const std::wstring& name);
		void WriteNodeBegin(std::wstring strNodeName, bool bAttributed = false);
		void WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode = false, bool bEndNode = true);

		void WriteAttribute(const std::wstring& strName, bool value);
		void WriteAttribute(const std::wstring& strName, int value);
		void WriteAttribute(const std::wstring& strName, unsigned int value);
		void WriteAttribute(const std::wstring& strName, double value);
		void WriteAttribute(const std::wstring& strName, const std::wstring& value);
		void WriteAttribute(const std::wstring& strName, const wchar_t* value);
		void WriteAttributeEncodeXml(const std::wstring& strName, const std::wstring& value);
		void WriteAttributeEncodeXml(const std::wstring& strName, const wchar_t* value);

		void WriteNodeValue(const std::wstring& strName, bool value);
		void WriteNodeValue(const std::wstring& strName, int value);
		void WriteNodeValue(const std::wstring& strName, unsigned int value);
		void WriteNodeValue(const std::wstring& strName, double value);
		void WriteNodeValue(const std::wstring& strName, const std::wstring& value);
		void WriteNodeValue(const std::wstring& strName, const wchar_t* value);
		void WriteNodeValueEncodeXml(const std::wstring& strName, const std::wstring& value);
		void WriteNodeValueEncodeXml(const std::wstring& strName, const wchar_t* value);
	protected:
		inline void WriteEncodeXmlString_4bytes(const wchar_t* pString, int nCount);
		inline void WriteEncodeXmlString_2bytes(const wchar_t* pString, int nCount);
		inline void WriteEncodeXmlStringHHHH_4bytes(const wchar_t* pString, int nCount);
		inline void WriteEncodeXmlStringHHHH_2bytes(const wchar_t* pString, int nCount);
		inline void WriteEncodeXmlChar(wchar_t code, unsigned char type);
		inline unsigned char CheckXmlCode(unsigned int c);
		inline unsigned char CheckXmlCodeHHHH(unsigned int c, const wchar_t* pData);
	};

	KERNEL_DECL void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo);
	KERNEL_DECL void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo);
}
