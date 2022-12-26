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

#include "../../../DesktopEditor/common/File.h"
#include "../../../OOXML/Base/Unit.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include <vector>

namespace NSCustomShapesConvert
{
    static std::wstring	g_bstr_nodeopen				= L"<";
    static std::wstring	g_bstr_nodeclose			= L">";
    static std::wstring	g_bstr_nodeopen_slash		= L"</";
    static std::wstring	g_bstr_nodeclose_slash		= L"/>";
    static std::wstring	g_bstr_node_space			= L" ";
    static std::wstring	g_bstr_node_equal			= L"=";
    static std::wstring	g_bstr_node_quote			= L"\"";
    static std::wstring	g_bstr_boolean_true			= L"true";
    static std::wstring	g_bstr_boolean_false		= L"false";
    static std::wstring	g_bstr_boolean_true2		= L"1";
    static std::wstring	g_bstr_boolean_false2		= L"0";
	
	static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}

	class CXmlWriter
	{
	public:
		NSStringUtils::CStringBuilder m_oWriter;

		CXmlWriter();
		~CXmlWriter();
		
		std::wstring GetXmlString();
		void ClearNoAttack();
		int GetSize();
		
		// write value
		void WriteString(const std::wstring& strValue);
		void WriteStringXML(std::wstring strValue);
		void WriteDouble(const double& val);
		void WriteLONG(const long& val);
		void WriteINT(const int& val);
		void WriteDWORD(const DWORD& val);
		void WriteDWORD_hex(const DWORD& val);
		void WriteBool(const bool& val);
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& val);
		void WriteAttribute2(const std::wstring& strAttributeName, const std::wstring& val);
		void WriteAttribute(const std::wstring& strAttributeName, const double& val);
		void WriteAttribute(const std::wstring& strAttributeName, const int& val);
		void WriteAttribute(const std::wstring& strAttributeName, const bool& val);
		void WriteAttribute(const std::wstring& strAttributeName, const LONG& val);
		void WriteAttribute(const std::wstring& strAttributeName, const DWORD& val);
		void WriteAttributeDWORD_hex(const std::wstring& strAttributeName, const DWORD& val);
		// document methods
		void WriteNodeBegin(std::wstring strNodeName, bool bAttributed = false);
		void WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode = false, bool bEndNode = true);
		// write node values
		void WriteNodeValue(const std::wstring& strNodeName, const std::wstring& val);
		void WriteNodeValue(const std::wstring& strNodeName, const bool& val);
		void WriteNodeValue(const std::wstring& strNodeName, const double& val);
		void WriteNodeValue(const std::wstring& strNodeName, const LONG& val);
		void WriteNodeValue(const std::wstring& strNodeName, const int& val);
		void WriteNodeValue(const std::wstring& strNodeName, const DWORD& val);
		void WriteNodeValueDWORD_hex(const std::wstring& strNodeName, const DWORD& val);
		
	public:
		void StartNode(const std::wstring& name);
		void StartAttributes();
		void EndAttributes();
		void EndNode(const std::wstring& name);

		template<typename T>
		void WriteArray(const std::wstring& strName, const std::vector<T>& arr)
		{
			size_t nCount = arr.size();
			if (0 != nCount)
			{
				StartNode(strName);
				m_oWriter.WriteString(g_bstr_nodeclose);
				for (size_t i = 0; i < nCount; ++i)
					arr[i].toXmlWriter(this);
				EndNode(strName);
			}
		}

		template<typename T>
		void WriteArray2(const std::vector<T>& arr)
		{
			size_t nCount = arr.size();
			if (0 != nCount)
			{
				for (size_t i = 0; i < nCount; ++i)
					arr[i].toXmlWriter(this);
			}
		}


		// -------------------------------------------------------------------------------------

		void ReplaceString(std::wstring from, std::wstring to);
	};
}
