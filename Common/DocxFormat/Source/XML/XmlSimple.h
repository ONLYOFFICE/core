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

#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../Base/Nullable.h"
#include "../XML/Utils.h"

#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../SystemUtility/SystemUtility.h"

namespace XmlUtils
{
	class CAttribute
	{
	public:
		std::wstring m_strValue;

		CAttribute()
		{
			m_strValue = _T("");
		}
		AVSINLINE void Write(const std::wstring& strName, const int& value)
		{
            m_strValue += L" " + strName + L"=\"" + std::to_wstring(value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const size_t& value)
		{
            m_strValue += L" " + strName + L"=\"" + std::to_wstring((unsigned int)value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const double& value)
		{
			m_strValue += L" " + strName + L"=\"" + XmlUtils::DoubleToString(value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const std::wstring& value)
		{
			m_strValue += (_T(" ") + strName + _T("=\"") + value + _T("\""));
		}
		AVSINLINE void Write(const std::wstring& strName, const bool& value)
		{
			if (value)
				m_strValue += (_T(" ") + strName + _T("=\"true\""));
			else
				m_strValue += (_T(" ") + strName + _T("=\"false\""));
		}
		template <typename T>
		AVSINLINE void WriteLimit(const std::wstring& strName, const T& value)
		{
			m_strValue += (_T(" ") + strName + _T("=\"") + value.get() + _T("\""));
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
		
	public:
		AVSINLINE void Write(const std::wstring& strName, const nullable_int& value)
		{
			if (!value.IsInit())
				return;
            m_strValue += L" " + strName + L"=\"" + std::to_wstring(*value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const nullable_sizet& value)
		{
			if (!value.IsInit())
				return;
            m_strValue += L" " + strName + L"=\"" + std::to_wstring((unsigned int)*value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const nullable_double& value)
		{
			if (!value.IsInit())
				return;
			m_strValue += L" " + strName + L"=\"" + XmlUtils::DoubleToString(*value) + L"\"";
		}
		AVSINLINE void Write(const std::wstring& strName, const nullable_string& value)
		{
			if (!value.IsInit())
				return;
			m_strValue += (_T(" ") + strName + _T("=\"") + *value + _T("\""));
		}
		AVSINLINE void Write(const std::wstring& strName, const nullable_bool& value)
		{
			if (!value.IsInit())
				return;
			if (*value)
				m_strValue += (_T(" ") + strName + _T("=\"true\""));
			else
				m_strValue += (_T(" ") + strName + _T("=\"false\""));
		}
		template <typename T>
		AVSINLINE void WriteLimitNullable(const std::wstring& strName, const nullable_limit<T>& value)
		{
			if (!value.IsInit())
				return;
			Write(strName, value->get());
		}

	public:
		CAttribute(const CAttribute& oSrc)
		{
			*this = oSrc;
		}
		CAttribute& operator=(const CAttribute& oSrc)
		{
			m_strValue = oSrc.m_strValue;
            return (*this);
		}
	};

	class CNodeValue
	{
	public:
		std::wstring m_strValue;

	public:
		CNodeValue()
		{
			m_strValue = _T("");
		}
		template <typename T>
		AVSINLINE void Write(T& value)
		{
			m_strValue += value.toXML();
		}
		template <typename T>
		AVSINLINE void Write(const std::wstring& strNodeName, T& value)
		{
			m_strValue += (_T("<") + strNodeName + _T(">"));
			m_strValue += value.toXML();
			m_strValue += (_T("</") + strNodeName + _T(">"));
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
			m_strValue += (_T("<") + strNodeName + _T(">"));
			size_t count = oArray.size();
			for (size_t i = 0; i < count; ++i)
				m_strValue += oArray[i].toXML();
			m_strValue += (_T("</") + strNodeName + _T(">"));
		}

        template <typename T>
        AVSINLINE void WriteArray(const std::vector<T>& oArray)
        {
            size_t count = oArray.size();
            for (size_t i = 0; i < count; ++i)
                m_strValue += oArray[i].toXML();
        }
		// --------------------------------------------------------------- //
		AVSINLINE void Write2(const std::wstring& strName, const int& value)
		{
            Write2(strName, std::to_wstring(value));
		}
		AVSINLINE void Write2(const std::wstring& strName, const size_t& value)
		{
            std::wstring s = L"=\"" + std::to_wstring((unsigned int)value) + L"\"";
			Write2(strName, s);
		}
		AVSINLINE void Write2(const std::wstring& strName, const double& value)
		{
			std::wstring s = L"=\"" + XmlUtils::DoubleToString(value) + L"\"";
			Write2(strName, s);
		}
		AVSINLINE void Write2(const std::wstring& strName, const std::wstring& value)
		{
			m_strValue += (_T("<") + strName + _T(">") + value + _T("</") + strName + _T(">"));
		}
		AVSINLINE void Write2(const std::wstring& strName, const bool& value)
		{
			if (value)
				m_strValue += (_T("<") + strName + _T(">true</") + strName + _T(">"));
			else
				m_strValue += (_T("<") + strName + _T(">false</") + strName + _T(">"));
		}

		// ---------------------------------------------------------------- //
		AVSINLINE void Write2(const std::wstring& strName, const nullable_int& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const std::wstring& strName, const nullable_sizet& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const std::wstring& strName, const nullable_double& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const std::wstring& strName, const nullable_string& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const std::wstring& strName, const nullable_bool& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
	};

	AVSINLINE std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr)
	{
		if (strName.empty()) return L"";

		return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L"/>";
	}
	AVSINLINE std::wstring CreateNode(const std::wstring& strName, const CNodeValue& oNode)
	{
		if (strName.empty()) return L"";

		if (oNode.m_strValue.empty())
			return L"<" + strName + L"/>";

		return L"<" + strName + L">" + oNode.m_strValue + L"</" + strName + L">";
	}
	AVSINLINE std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const CNodeValue& oNode)
	{
		if (strName.empty()) return L"";

		if ( oNode.m_strValue.empty())
			return CreateNode(strName, oAttr);
		
		return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L">" + oNode.m_strValue +  L"</" + strName + L">";
	}
	AVSINLINE std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const std::wstring& strXml)
	{
		if (strName.empty()) return L"";

		if (strXml.empty())	return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L"/>";
		else				return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L">" + strXml + L"</" + strName + L">";
		
		
	}
	AVSINLINE std::wstring CreateNode(const std::wstring& strName, const std::wstring& strXml)
	{
		if (strName.empty()) return L"";

		if (strXml.empty()) return L"<" + strName + L"/>";
		else				return L"<" + strName + L">" + strXml + L"</" + strName + L">";
	}

	AVSINLINE void SaveToFile(const std::wstring& strFile, const std::wstring& strXml)
	{
		NSFile::CFileBinary::SaveToFile(strFile, strXml);
	}
}
