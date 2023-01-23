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

		CXmlWriter() : m_oWriter()
		{
		}
		~CXmlWriter()
		{
		}
		
		std::wstring GetXmlString()
		{
			return m_oWriter.GetData();
		}
		void ClearNoAttack()
		{
			m_oWriter.ClearNoAttack();
		}
		int GetSize()
		{
			return (int)m_oWriter.GetCurSize();
		}
		
		// write value
		void WriteString(const std::wstring& strValue)
		{
			m_oWriter.WriteString(strValue);
		}
		void WriteStringXML(std::wstring strValue)
		{
			m_oWriter.WriteEncodeXmlString(strValue);
		}
		void WriteDouble(const double& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));
		}
		void WriteLONG(const long& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));

		}
		void WriteINT(const int& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));

		}
		void WriteDWORD(const DWORD& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));

		}
		void WriteDWORD_hex(const DWORD& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));

		}		
		void WriteBool(const bool& val)
		{
			m_oWriter.WriteString(XmlUtils::ToString(val));
		}
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			m_oWriter.WriteString(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute2(const std::wstring& strAttributeName, const std::wstring& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			m_oWriter.WriteEncodeXmlString(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const double& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDouble(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const int& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteINT(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const bool& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteBool(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const LONG& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteLONG(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const DWORD& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDWORD(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		void WriteAttributeDWORD_hex(const std::wstring& strAttributeName, const DWORD& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDWORD_hex(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		// document methods
        void WriteNodeBegin(std::wstring strNodeName, bool bAttributed = false)
		{
			m_oWriter.WriteString(g_bstr_nodeopen);
			m_oWriter.WriteString(strNodeName);
			
			if (!bAttributed)
				m_oWriter.WriteString(g_bstr_nodeclose);
		}
        void WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode = false, bool bEndNode = true)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_oWriter.WriteString(g_bstr_nodeclose_slash);
				else
					m_oWriter.WriteString(g_bstr_nodeclose);
			}
			else
			{
				m_oWriter.WriteString(g_bstr_nodeopen_slash);
				m_oWriter.WriteString(strNodeName);
				m_oWriter.WriteString(g_bstr_nodeclose);
			}
		}
		// write node values
		void WriteNodeValue(const std::wstring& strNodeName, const std::wstring& val)
		{
			WriteNodeBegin(strNodeName);
			WriteString(val);
			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValue(const std::wstring& strNodeName, const bool& val)
		{
			WriteNodeBegin(strNodeName);
			
			if (val)
				WriteString(_T("1"));
			else
				WriteString(_T("0"));

			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValue(const std::wstring& strNodeName, const double& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDouble(val);
			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValue(const std::wstring& strNodeName, const LONG& val)
		{
			WriteNodeBegin(strNodeName);
			WriteLONG(val);
			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValue(const std::wstring& strNodeName, const int& val)
		{
			WriteNodeBegin(strNodeName);
			WriteINT(val);
			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValue(const std::wstring& strNodeName, const DWORD& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDWORD(val);
			WriteNodeEnd(strNodeName);
		}
		void WriteNodeValueDWORD_hex(const std::wstring& strNodeName, const DWORD& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDWORD_hex(val);
			WriteNodeEnd(strNodeName);
		}
		
	public:

		void StartNode(const std::wstring& name)
		{
			m_oWriter.WriteString(g_bstr_nodeopen);
			m_oWriter.WriteString(name);
		}
		void StartAttributes()
		{
			// none			
		}
		void EndAttributes()
		{
			m_oWriter.WriteString(g_bstr_nodeclose);
		}
		void EndNode(const std::wstring& name)
		{
			m_oWriter.WriteString(g_bstr_nodeopen_slash);
			m_oWriter.WriteString(name);
			m_oWriter.WriteString(g_bstr_nodeclose);
		}

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

		void ReplaceString(std::wstring from, std::wstring to)
		{
			// ужасная функция. вызывать ее не надо. не для этого класс писался.
			std::wstring sCur = m_oWriter.GetData();

			size_t start_pos = 0;
			while((start_pos = sCur .find(from, start_pos)) != std::wstring::npos) 
			{
				sCur .replace(start_pos, from.length(), to);
				start_pos += to.length(); 
			}

			ClearNoAttack();
			WriteString(sCur);
		}
	};
}
