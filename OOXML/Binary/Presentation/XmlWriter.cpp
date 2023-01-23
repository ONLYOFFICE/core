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

#include "XmlWriter.h"

namespace NSBinPptxRW
{
CXmlWriter::CXmlWriter (BYTE ooxType) : m_oWriter()
{
	m_lDocType					= ooxType;
	m_lFlag                     = 0;
	m_lGroupIndex               = 0;
	m_lObjectId                 = 0;
	m_lObjectIdVML              = 0;
	m_lObjectIdOle              = 0;

	m_bIsUseOffice2007          = false;

	m_pOOXToVMLRenderer         = NULL;
	m_bIsTop                    = false;
}
CXmlWriter::~CXmlWriter()
{
}

std::wstring CXmlWriter::GetXmlString()
{
	return m_oWriter.GetData();
}
void CXmlWriter::ClearNoAttack()
{
	m_oWriter.ClearNoAttack();
}
int CXmlWriter::GetSize()
{
	return (int)m_oWriter.GetCurSize();
}

// write value
void CXmlWriter::WriteString(const std::wstring& strValue)
{
	m_oWriter.WriteString(strValue);
}
void CXmlWriter::WriteStringXML(std::wstring strValue)
{
	std::wstring s = strValue;
	XmlUtils::replace_all( s, L"&",	L"&amp;");
	XmlUtils::replace_all( s, L"'",	L"&apos;");
	XmlUtils::replace_all( s, L"<",	L"&lt;");
	XmlUtils::replace_all( s, L">",	L"&gt;");
	XmlUtils::replace_all( s, L"\"",	L"&quot;");
	m_oWriter.WriteString(s);
}
void CXmlWriter::WriteDouble(const double& val)
{
	m_oWriter.WriteString(std::to_wstring(val));
}
void CXmlWriter::WriteLONG(const long& val)
{
	m_oWriter.WriteString(std::to_wstring(val));
}
void CXmlWriter::WriteINT(const int& val)
{
	m_oWriter.WriteString(std::to_wstring(val));
}
void CXmlWriter::WriteUINT(const unsigned int& val)
{
	m_oWriter.WriteString(std::to_wstring(val));
}
void CXmlWriter::WriteDWORD(const DWORD& val)
{
	m_oWriter.WriteString(std::to_wstring(val));
}
void CXmlWriter::WriteSIZET(const size_t& val)
{
	m_oWriter.WriteString(std::to_wstring((unsigned int)val));
}
void CXmlWriter::WriteDWORD_hex(const DWORD& val)
{
	m_oWriter.WriteString(XmlUtils::ToString((_UINT32)val, L"%x"));
}
void CXmlWriter::WriteBool(const bool& val)
{
	if (val)
		m_oWriter.WriteString(g_bstr_boolean_true2);
	else
		m_oWriter.WriteString(g_bstr_boolean_false2);
}

// write attribute
void CXmlWriter::WriteAttributeCSS(const std::wstring& strAttributeName, const std::wstring& val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(15);
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.WriteString(val);
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}
void CXmlWriter::WriteAttributeCSS(const std::wstring& strAttributeName, const wchar_t* val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(15);
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.WriteString(val);
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}
void CXmlWriter::WriteAttributeCSS_int(const std::wstring& strAttributeName, const int& val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(15);
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.AddIntNoCheck(val);
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}
void CXmlWriter::WriteAttributeCSS_double1(const std::wstring& strAttributeName, const double& val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(15);
	std::wstring s = XmlUtils::DoubleToString(val, L"%.1lf");
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.WriteString(s);
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}
void CXmlWriter::WriteAttributeCSS_int_pt(const std::wstring& strAttributeName, const int& val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(15);
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.AddIntNoCheck(val);
	m_oWriter.AddCharNoCheck(WCHAR('p'));
	m_oWriter.AddCharNoCheck(WCHAR('t'));
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}
void CXmlWriter::WriteAttributeCSS_double1_pt(const std::wstring& strAttributeName, const double& val)
{
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.AddSize(20);
	std::wstring s = XmlUtils::DoubleToString(val, L"%.1lf");
	m_oWriter.AddCharNoCheck(WCHAR(':'));
	m_oWriter.WriteString(s);
	m_oWriter.AddCharNoCheck(WCHAR('p'));
	m_oWriter.AddCharNoCheck(WCHAR('t'));
	m_oWriter.AddCharNoCheck(WCHAR(';'));
}

void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const std::wstring& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	m_oWriter.WriteString(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const wchar_t* val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	m_oWriter.WriteString(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute2(const std::wstring& strAttributeName, const std::wstring& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	m_oWriter.WriteEncodeXmlString(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const double& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteDouble(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const int& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteINT(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const bool& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteBool(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const LONG& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteLONG(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const DWORD& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteDWORD(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}

#if defined (_WIN32) || defined(_WIN64)
void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const size_t& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteSIZET(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}
#endif

void CXmlWriter::WriteAttributeDWORD_hex(const std::wstring& strAttributeName, const DWORD& val)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strAttributeName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteDWORD_hex(val);
	m_oWriter.WriteString(g_bstr_node_quote);
}

// document methods
void CXmlWriter::WriteNodeBegin(std::wstring strNodeName, bool bAttributed)
{
	m_oWriter.WriteString(g_bstr_nodeopen);
	m_oWriter.WriteString(strNodeName);

	if (!bAttributed)
		m_oWriter.WriteString(g_bstr_nodeclose);
}
void CXmlWriter::WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode, bool bEndNode)
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
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const std::wstring& val)
{
	WriteNodeBegin(strNodeName);
	WriteString(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue2(const std::wstring& strNodeName, const std::wstring& val)
{
	WriteNodeBegin(strNodeName);
	WriteStringXML(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const bool& val)
{
	WriteNodeBegin(strNodeName);

	if (val)
		WriteString(_T("1"));
	else
		WriteString(_T("0"));

	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const double& val)
{
	WriteNodeBegin(strNodeName);
	WriteDouble(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const LONG& val)
{
	WriteNodeBegin(strNodeName);
	WriteLONG(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const int& val)
{
	WriteNodeBegin(strNodeName);
	WriteINT(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const unsigned int& val)
{
	WriteNodeBegin(strNodeName);
	WriteUINT(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strNodeName, const DWORD& val)
{
	WriteNodeBegin(strNodeName);
	WriteDWORD(val);
	WriteNodeEnd(strNodeName);
}
void CXmlWriter::WriteNodeValueDWORD_hex(const std::wstring& strNodeName, const DWORD& val)
{
	WriteNodeBegin(strNodeName);
	WriteDWORD_hex(val);
	WriteNodeEnd(strNodeName);
}

bool CXmlWriter::SaveToFile(std::wstring strFilePath, bool bEncodingToUTF8, bool bIsClearNoAttack)
{
	std::wstring strData = m_oWriter.GetData();
	if (!bEncodingToUTF8)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(strFilePath);
		oFile.WriteFile((BYTE*)strData.c_str(), (DWORD)(strData.length() * sizeof(wchar_t)));
		oFile.CloseFile();
	}
	else
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(strFilePath);
		std::wstring strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		oFile.WriteStringUTF8(strHead);
		oFile.WriteStringUTF8(strData);
		oFile.CloseFile();
	}

	if (bIsClearNoAttack)
	{
		m_oWriter.ClearNoAttack();
	}

	return true;
}

// ATTRIBUTES --------------------------------------------------------------------------
void CXmlWriter::WriteAttribute(const std::wstring& strName, const nullable_int& value)
{
	if (value.IsInit())
		WriteAttribute(strName, *value);
}
void CXmlWriter::WriteAttribute2(const std::wstring& strName, const unsigned int& value)
{
	m_oWriter.WriteString(g_bstr_node_space);
	m_oWriter.WriteString(strName);
	m_oWriter.WriteString(g_bstr_node_equal);
	m_oWriter.WriteString(g_bstr_node_quote);
	WriteUINT(value);
	m_oWriter.WriteString(g_bstr_node_quote);

}
void CXmlWriter::WriteAttribute2(const std::wstring& strName, const nullable_uint& value)
{
	if (value.IsInit())
		WriteAttribute2(strName, *value);
}
void CXmlWriter::WriteAttribute(const std::wstring& strName, const nullable_sizet& value)
{
	if (value.IsInit())
#ifdef __ANDROID__
		WriteAttribute(strName, (int)(*value));
#else
		WriteAttribute(strName, *value);
#endif

}
void CXmlWriter::WriteAttribute(const std::wstring& strName, const nullable_double& value)
{
	if (value.IsInit())
		WriteAttribute(strName, *value);
}
void CXmlWriter::WriteAttribute(const std::wstring& strName, const nullable_string& value)
{
	if (value.IsInit())
		WriteAttribute(strName, *value);
}
void CXmlWriter::WriteAttribute2(const std::wstring& strName, const nullable_string& value)
{
	if (value.IsInit())
		WriteAttribute2(strName, *value);
}
void CXmlWriter::WriteAttribute(const std::wstring& strName, const nullable_bool& value)
{
	if (value.IsInit())
		WriteAttribute(strName, *value);
}

// -------------------------------------------------------------------------------------
// NODES -------------------------------------------------------------------------------
void CXmlWriter::WriteNodeValue(const std::wstring& strName, const nullable_int& value)
{
	if (value.IsInit())
		WriteNodeValue(strName, *value);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strName, const nullable_uint& value)
{
	if (value.IsInit())
		WriteNodeValue(strName, *value);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strName, const nullable_double& value)
{
	if (value.IsInit())
		WriteNodeValue(strName, *value);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strName, const nullable_string& value)
{
	if (value.IsInit())
		WriteNodeValue(strName, *value);
}
void CXmlWriter::WriteNodeValue2(const std::wstring& strName, const nullable_string& value)
{
	if (value.IsInit())
		WriteNodeValue2(strName, *value);
}
void CXmlWriter::WriteNodeValue(const std::wstring& strName, const nullable_bool& value)
{
	if (value.IsInit())
		WriteNodeValue(strName, *value);
}

// -------------------------------------------------------------------------------------
// DOCUMENT ----------------------------------------------------------------------------
void CXmlWriter::StartNode(const std::wstring& name)
{
	m_oWriter.WriteString(g_bstr_nodeopen);
	m_oWriter.WriteString(name);
}
void CXmlWriter::StartAttributes()
{
	// none
}
void CXmlWriter::EndAttributes()
{
	m_oWriter.WriteString(g_bstr_nodeclose);
}
void CXmlWriter::EndNode(const std::wstring& name)
{
	m_oWriter.WriteString(g_bstr_nodeopen_slash);
	m_oWriter.WriteString(name);
	m_oWriter.WriteString(g_bstr_nodeclose);
}

// -------------------------------------------------------------------------------------

void CXmlWriter::ReplaceString(std::wstring str1, std::wstring str2)
{
	// ужасная функция. вызывать ее не надо. не для этого класс писался. .. получше .. но все равно не надо !!!

	std::wstring sCur = m_oWriter.GetData();
	XmlUtils::replace_all(sCur, str1, str2);

	ClearNoAttack();
	WriteString(sCur);
}

}
