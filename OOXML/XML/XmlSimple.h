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

#include "../Base/Nullable.h"

namespace XmlUtils
{
	using namespace NSCommon;
	class CAttribute
	{
	public:
		std::wstring m_strValue;

		CAttribute();
		void Write(const std::wstring& strName, const int& value);
		void Write(const std::wstring& strName, const size_t& value);
		void Write(const std::wstring& strName, const double& value);
		void Write(const std::wstring& strName, const std::wstring& value);
		void Write(const std::wstring& strName, const bool& value);

		template <typename T>
		AVSINLINE void WriteLimit(const std::wstring& strName, const T& value)
		{
			m_strValue += (L" ") + strName + L"=\"" + value.get() + L"\"";
		}
		template <typename T>
		AVSINLINE void WriteNullable(const std::wstring& strName, const nullable<T>& value)
		{
			if (value.IsInit())
				Write(strName, value->ToString());
		}
		AVSINLINE void WriteNullable(const std::wstring& strName, const nullable_string& value)
		{
			if (value.IsInit())
				WriteLimit(strName, value);
		}
		template <typename T>
		AVSINLINE void WriteLimit2(const std::wstring& strName, const T& value)
		{
			Write(strName, value.ToString());
		}
		template <typename T>
		AVSINLINE void WriteLimitNullable(const std::wstring& strName, const nullable_limit<T>& value)
		{
			if (!value.IsInit())
				return;
			Write(strName, value->get());
		}
		
	public:
		void Write(const std::wstring& strName, const nullable_int& value);
		void Write(const std::wstring& strName, const nullable_uint& value);
		void Write(const std::wstring& strName, const nullable_sizet& value);
		void Write(const std::wstring& strName, const nullable_double& value);
		void Write(const std::wstring& strName, const nullable_string& value);
		void Write(const std::wstring& strName, const nullable_bool& value);
		void Write2(const std::wstring& strName, const nullable_bool& value);

	public:
		CAttribute(const CAttribute& oSrc);
		CAttribute& operator=(const CAttribute& oSrc);
	};

	class CNodeValue
	{
	public:
		std::wstring m_strValue;

	public:
		CNodeValue();

		template <typename T>
		AVSINLINE void Write(T& value)
		{
			m_strValue += value.toXML();
		}
		template <typename T>
		AVSINLINE void Write(const std::wstring& strNodeName, T& value)
		{
			m_strValue += (L"<") + strNodeName + L">";
			m_strValue += value.toXML();
			m_strValue += (L"</") + strNodeName + L">";
		}
		template <typename T>
		AVSINLINE void WriteNullable(const nullable<T>& value)
		{
			if (value.IsInit())
				m_strValue += value->toXML();
		}
		template <typename T>
		AVSINLINE void WriteArray(const std::wstring& strNodeName, const std::vector<T>& oArray)
		{
			m_strValue += (L"<") + strNodeName + L">";
			size_t count = oArray.size();
			for (size_t i = 0; i < count; ++i)
				m_strValue += oArray[i].toXML();
			m_strValue += (L"</") + strNodeName + L">";
		}

		template <typename T>
		AVSINLINE void WriteArray(const std::vector<T>& oArray)
		{
			size_t count = oArray.size();
			for (size_t i = 0; i < count; ++i)
				m_strValue += oArray[i].toXML();
		}
		// --------------------------------------------------------------- //
		void Write2(const std::wstring& strName, const int& value);
		void Write2(const std::wstring& strName, const size_t& value);
		void Write2(const std::wstring& strName, const double& value);
		void Write2(const std::wstring& strName, const std::wstring& value);
		void Write2(const std::wstring& strName, const bool& value);

		// ---------------------------------------------------------------- //
		void Write2(const std::wstring& strName, const nullable_int& value);
		void Write2(const std::wstring& strName, const nullable_sizet& value);
		void Write2(const std::wstring& strName, const nullable_double& value);
		void Write2(const std::wstring& strName, const nullable_string& value);
		void Write2(const std::wstring& strName, const nullable_bool& value);
	};

	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr);
	std::wstring CreateNode(const std::wstring& strName, const CNodeValue& oNode);
	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const CNodeValue& oNode);
	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const std::wstring& strXml);
	std::wstring CreateNode(const std::wstring& strName, const std::wstring& strXml);

	void SaveToFile(const std::wstring& strFile, const std::wstring& strXml);
}
