/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#pragma once
#include "Utils.h"
#include "stringcommon.h"

#include "../Base/Base.h"

#include <vector>

class CWCharWrapper
{
public:

	CWCharWrapper():m_cwsString(NULL) 
	{
	}

	CWCharWrapper(const wchar_t* cwsString):m_cwsString(cwsString)
	{
	}

	CWCharWrapper(const std::wstring& cwsString):m_cwsString(cwsString.c_str())
	{
	}

    AVSINLINE const CWCharWrapper operator=(const wchar_t* cwsOther)
	{
		m_cwsString = cwsOther;
		return *this;
	}

    AVSINLINE const bool operator==(const wchar_t* cwsOther)
	{
		if ( 0 == WStrCmp( m_cwsString, cwsOther ) )
			return true;

		return false;
	}

    AVSINLINE const bool operator!=(const wchar_t* cwsOther)
	{
		if ( 0 != WStrCmp( m_cwsString, cwsOther ) )
			return true;

		return false;
	}

    AVSINLINE const wchar_t operator[](const int nIndex) const
	{
		int nLen = GetLength();

		if ( nIndex >= nLen )
			return '\0';

		return m_cwsString[nIndex];
	}
    AVSINLINE const bool IsNull() const
	{
		return (m_cwsString == NULL);
	}


    AVSINLINE const int  GetLength() const
	{
		if ( NULL == m_cwsString )
			return 0;

		return (const int)wcslen( m_cwsString );
	}
public:
    static AVSINLINE int WStrCmp(const wchar_t* cwsStr1, const wchar_t* cwsStr2)
	{
		if ( NULL == cwsStr1 && NULL == cwsStr2 )
			return 0;
		else if ( NULL == cwsStr1 || NULL == cwsStr2 )
			return -1;

		return wcscmp( cwsStr1, cwsStr2 );
	}

public:

	const wchar_t* m_cwsString;
};

AVSINLINE const bool operator==(const wchar_t* cwsStr1, const CWCharWrapper& cwsStr2)
{
	if ( 0 == CWCharWrapper::WStrCmp( cwsStr2.m_cwsString, cwsStr1 ) )
		return true;

	return false;
}

AVSINLINE const bool operator!=(const wchar_t* cwsStr1, const CWCharWrapper& cwsStr2)
{
	if ( 0 != CWCharWrapper::WStrCmp( cwsStr2.m_cwsString, cwsStr1 ) )
		return true;

	return false;
}

namespace XmlUtils
{
	AVSINLINE std::wstring GetNamespace(std::wstring & strNodeName)
	{
		int nFind = strNodeName.find(_T(':'));
		if (-1 == nFind)
			return _T("");
		return strNodeName.substr(0, nFind);
	}
	AVSINLINE CString GetNamespace(CString& strNodeName)
	{
		int nFind = strNodeName.Find(TCHAR(':'));
		if (-1 == nFind)
			return _T("");
		return strNodeName.Mid(0, nFind);
	}
	AVSINLINE CString GetNamespace(const wchar_t* strNodeName)
	{
		const wchar_t* find = ::wcschr(strNodeName, wchar_t(':'));
		if(NULL == find)
			return _T("");
		return CString(strNodeName, find - strNodeName);
	}
	AVSINLINE std::wstring GetNameNoNS(const std::wstring & strNodeName)
	{
		int nFind = strNodeName.find(_T(':'));
		if (-1 == nFind)
			return strNodeName;
		return strNodeName.substr(nFind + 1);
	}
	AVSINLINE CString GetNameNoNS(const CString& strNodeName)
	{
		int nFind = strNodeName.Find(TCHAR(':'));
		if (-1 == nFind)
			return strNodeName;
		return strNodeName.Mid(nFind + 1);
	}
	AVSINLINE CString GetNameNoNS(const wchar_t* strNodeName)
	{
		const wchar_t* find = ::wcschr(strNodeName, TCHAR(':'));
		if(NULL == find)
			return CString(strNodeName);
		return CString(find + 1);
	}
	// classes
	class CXmlWriter
	{
		CString m_str;
	
	public:

		CXmlWriter()
		{
			m_str.Empty();
		}
		
		CString GetXmlString()
		{
			return m_str;
		}
		void SetXmlString(const CString& strValue)
		{
			m_str = strValue;
		}
		
                bool SaveToFile(const CString& strFilePath, bool bEncodingToUTF8 = false)
		{
#if defined(_WIN32) || defined (_WIN64)
            // win32 unicode and multibyte strings
			FILE* pFile = _tfopen(strFilePath, _T("wt"));
#else
            // *nix build
            #ifdef _UNICODE
                std::string sFilePathUtf8 = stringWstingToUtf8String (strFilePath);
                FILE* pFile = fopen(sFilePathUtf8.c_str(), "wt");
            #else
                // path is already utf8
                std::string sFilePathUtf8 = strFilePath;
                FILE* pFile = fopen(sFilePathUtf8.c_str(), "wt");
            #endif
#endif

			if (!pFile)
                                return false;

			CStringA str; str = m_str;
			if (bEncodingToUTF8)
#ifdef _UNICODE
				str = EncodingUnicodeToUTF8();
#else
				str = EncodingASCIIToUTF8();
#endif

			fprintf(pFile, str);

			fclose(pFile);

                        return true;
		}
#ifdef _UNICODE
        CStringA EncodingUnicodeToUTF8()
        {
            #if defined(_WIN32) || defined (_WIN64)
                int nLength = m_str.GetLength();

                // Encoding Unicode to UTF-8
                CStringA saStr;
                WideCharToMultiByte(CP_UTF8, 0, m_str.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
                saStr.ReleaseBuffer();
                return saStr;
            #else

                std::string sStrTempUtf8 = stringWstingToUtf8String (m_str);
                CStringA saStr = sStrTempUtf8;
                return saStr;
            #endif
        }
#else
		CString EncodingASCIIToUTF8()
        {
            int nLength = m_str.GetLength();

			wchar_t* pWStr = new wchar_t[nLength + 1];
			if (!pWStr)
				return _T("");

			// set end string
			pWStr[nLength] = 0;

			// Encoding ASCII to Unicode
            MultiByteToWideChar(CP_ACP, 0, m_str, nLength, pWStr, nLength);

            int nLengthW = (int) wcslen(pWStr);

			// Encoding Unicode to UTF-8
			CString cStr; 
            WideCharToMultiByte(CP_UTF8, 0, pWStr, nLengthW + 1, cStr.GetBuffer(nLengthW*3 + 1), nLengthW*3, NULL, NULL);
			cStr.ReleaseBuffer();
    
		    delete[] pWStr;

            return cStr;
        }
#endif


		void WriteString(const CString& strValue)
		{
			m_str += strValue;
		}
		void WriteInteger(int Value, int Base = 10)
		{
            char str[33];
            memset(str, 0, 33);
#if defined(_WIN32) || defined (_WIN64)
            _itoa(Value, str, Base);
#else
            itoa(Value, str, Base);
#endif

			m_str += str;
		}
		void WriteDouble(double Value)
		{
			CString str;
			
			str.Format(_T("%lf"), Value);

			m_str += str;
		}
                void WriteBoolean(bool Value)
		{
			if (Value)
				m_str += _T("true");
			else
				m_str += _T("false");
		}
                void WriteNodeBegin(const CString& strNodeName, bool bAttributed = false)
		{
			m_str += _T("<") + strNodeName;

			if (!bAttributed)
				m_str += _T(">");
		}
                void WriteNodeEnd(const CString& strNodeName, bool bEmptyNode = false, bool bEndNode = true)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_str += _T(" />");
				else
					m_str += _T(">");
			}
			else
				m_str += _T("</") + strNodeName + _T(">");
		}
		void WriteNode(const CString& strNodeName, const CString& strNodeValue)
		{
			if (strNodeValue.GetLength() == 0)
				m_str += _T("<") + strNodeName + _T("/>");
			else
				m_str += _T("<") + strNodeName + _T(">") + strNodeValue + _T("</") + strNodeName + _T(">");
		}
		void WriteNode(const CString& strNodeName, int nValue, int nBase = 10, const CString& strTextBeforeValue = _T(""), const CString& strTextAfterValue = _T(""))
		{
			WriteNodeBegin(strNodeName);
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteNode(const CString& strNodeName, double dValue)
		{
			WriteNodeBegin(strNodeName);
			WriteDouble(dValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteAttribute(const CString& strAttributeName, const CString& strAttributeValue)
		{
			m_str += _T(" ") + strAttributeName + _T("=\"") + strAttributeValue + _T("\"");
		}
		void WriteAttribute(const CString& strAttributeName, int nValue, int nBase = 10, const CString& strTextBeforeValue = _T(""), const CString& strTextAfterValue = _T(""))
		{
			WriteString(_T(" ") + strAttributeName + _T("="));
			WriteString(_T("\""));
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
			WriteString(_T("\""));
		}
		void WriteAttribute(const CString& strAttributeName, double dValue)
		{
			WriteString(_T(" ") + strAttributeName + _T("="));
			WriteString(_T("\""));
			WriteDouble(dValue);
			WriteString(_T("\""));
		}
	};
}

//#define _USE_LIBXML2_READER_

#ifdef _USE_LIBXML2_READER_

#include "libxml2/libxml2.h"

#else
#if defined(_WIN32) || defined (_WIN64)
#import <msxml3.dll> rename_namespace("XML")

#ifdef  _USE_XMLLITE_READER_
#pragma comment(lib, "XmlLite.lib")
#include "XmlLite.h"
#endif



namespace MSXML
{
	struct __declspec(uuid("f6d90f11-9c73-11d3-b32e-00c04f990bb4"))
		DOMDocument;

	struct __declspec(uuid("f5078f1b-c551-11d3-89b9-0000f81fe221"))
		DOMDocument26;

	struct __declspec(uuid("f5078f32-c551-11d3-89b9-0000f81fe221"))
		DOMDocument30;

	struct __declspec(uuid("88d969c0-f192-11d4-a65f-0040963251e5"))
		DOMDocument40;

	struct __declspec(uuid("88d969e5-f192-11d4-a65f-0040963251e5"))
		DOMDocument50;

	struct __declspec(uuid("88d96a05-f192-11d4-a65f-0040963251e5"))
		DOMDocument60;
}

#define MSXMLDocument __uuidof(MSXML2::DOMDocument60)


namespace XmlUtils
{
	static CString GetNodeAttrib(XML::IXMLDOMNodePtr node, const CString& attrib, const CString& def = _T(""))
	{
		try
		{
			if (node == NULL || attrib.GetLength() < 1)
				return def;

			_bstr_t x = attrib;

			IXMLDOMNamedNodeMapPtr attributes = node->attributes;
			if (/*node->*/attributes == NULL)// || node->attributes->getNamedItem(x) == NULL)
				return def;

			IXMLDOMNodePtr tempNode;
			/*node->*/attributes->getNamedItem(x, &tempNode);// ->getNamedItem(x);
			if(tempNode == NULL)
				return def;
			BSTR str;
			tempNode->get_text(&str);//node->attributes->getNamedItem(x)->text.GetBSTR();
			CString s = CString(str);
			SysFreeString(str);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}

	static CString GetNodeAttribExt(XML::IXMLDOMNodePtr node, const CString& attrib, const CString& def = _T(""))
	{
		try
		{
			if (node == NULL || attrib.GetLength() < 1)
				return def;

			_bstr_t x = attrib;

			if (node->attributes == NULL || node->attributes->getNamedItem(x) == NULL)
				return def;

			VARIANT var;
			node->attributes->getNamedItem(x)->get_nodeValue(&var);

			if (VT_BSTR == var.vt)
			{
				CString s = (CString)var.bstrVal;
				VariantClear(&var);
				return s;
			}

			VariantClear(&var);
		}
		catch (...)
		{
		}

		return def;
	}

	static CString GetNodeText(XML::IXMLDOMNodePtr node, const CString& def = _T(""))
	{
		try
		{
			if (node == NULL)
				return def;

			CString s; s = node->text.GetBSTR();

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeXml(XML::IXMLDOMNodePtr node, const CString& subnode, const CString& def = _T(""))
	{
		try
		{
			if (node == NULL)
				return def;

			CString s; 

			if (subnode.GetLength() < 1)
				s = node->xml.GetBSTR();
			else
			{
				_bstr_t x = subnode;
				s = node->selectSingleNode(x)->xml.GetBSTR();
			}

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValue(XML::IXMLDOMNodePtr node, const CString& subnode, const CString& def = _T(""))
	{
		try
		{
			if (node == NULL || subnode.GetLength() < 1)
				return def;

			_bstr_t x = subnode;

			if (node->selectSingleNode(x) == NULL)
				return def;

			CString s; s = node->selectSingleNode(x)->text.GetBSTR();

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValueExt(XML::IXMLDOMNodePtr ptrNode, const CString& sDef = _T(""))
	{
		try
		{
			if (ptrNode == NULL)
				return sDef;

			CString sPath = _T("./text()");

			XML::IXMLDOMNodeList* pChildTextNodeList = NULL;
			BSTR bsPath = sPath.AllocSysString();
			ptrNode->raw_selectNodes(bsPath, &pChildTextNodeList);
			SysFreeString(bsPath);

			if (NULL!=pChildTextNodeList)
			{
				CString sRet; 
				long lCount = 0;
				pChildTextNodeList->get_length(&lCount);
				for (long lIndex = 0; lIndex < lCount; lIndex++)
				{
					XML::IXMLDOMNode *pChildTextNode = NULL;
					pChildTextNodeList->get_item(lIndex, &pChildTextNode);
					if (NULL==pChildTextNode)
						continue;
					VARIANT vt;
					pChildTextNode->get_nodeValue(&vt);
					pChildTextNode->Release();
					CString sValue; 
					if (VT_BSTR == vt.vt)
						sValue = vt.bstrVal;
					VariantClear(&vt);
					sRet += sValue;
				}
				pChildTextNodeList->Release();
				return sRet;
			}
		}
		catch (...)
		{
		}

		return sDef;
	}
        static bool GetNodeTextTyped(XML::IXMLDOMNodePtr node, _variant_t* variant, const CString& type = _T("bin.base64"))
	{
		try
		{
			if (node == NULL)
                                return false;

			BSTR bstrType = type.AllocSysString();

			node->put_dataType(bstrType);

			SysFreeString(bstrType);

			*variant = node->nodeTypedValue;

                        return true;
		}
		catch (...)
		{
		}

                return false;
	}
		
	static CString GetNodeAttrib(XML::IXMLDOMNodeListPtr nodes, int index, const CString& attrib, const CString& def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeAttrib(nodes->item[index], attrib, def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeText(XML::IXMLDOMNodeListPtr nodes, int index, const CString& def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeText(nodes->item[index], def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeXml(XML::IXMLDOMNodeListPtr nodes, int index, const CString& def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeXml(nodes->item[index], def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValue(XML::IXMLDOMNodeListPtr nodes, int index, const CString& subnode, const CString& def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeValue(nodes->item[index], subnode, def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
        static bool GetNodeTextTyped(XML::IXMLDOMNodeListPtr nodes, int index, _variant_t* variant, const CString& type = _T("bin.base64"))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
                                return false;

			return GetNodeTextTyped(nodes->item[index], variant, type);
		}
		catch (...)
		{
		}

                return false;
	}
	
	class CXmlReader
	{
		CString m_str;
		XML::IXMLDOMDocument2Ptr	m_pXmlDocument;
		XML::IXMLDOMNodeListPtr     m_pXmlNodeList;

	protected:

		XML::IXMLDOMNodePtr m_pXmlNode;

	public:

		CXmlReader()
		{
			Clear();

			m_pXmlDocument = NULL;

			if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument60))))
			{
				if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument))))
				{
					m_pXmlDocument = NULL;
				}
			}
			if (NULL != m_pXmlDocument)
			{
				m_pXmlDocument->setProperty( _bstr_t(_T("SelectionLanguage")), _variant_t(_T("XPath")) );
			}
		}
                CXmlReader(bool bCreateDocument)
		{
			Clear();

			m_pXmlDocument = NULL;

			if (bCreateDocument)
			{
				if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument60))))
				{
					if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument))))
					{
						m_pXmlDocument = NULL;
					}
				}
				if (NULL != m_pXmlDocument)
				{
					m_pXmlDocument->setProperty( _bstr_t(_T("SelectionLanguage")), _variant_t(_T("XPath")) );
				}
			}
		}
		
		void Clear()
		{
			m_str.Empty();

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;
		}
		CString GetXmlString()
		{
			return m_str;
		}
                bool SetXmlString(const CString& strValue)
		{
			if (NULL == m_pXmlDocument)
                                return false;

			try
			{
				_bstr_t bstrXml; bstrXml = strValue;

				if (_T("") != strValue && VARIANT_TRUE == m_pXmlDocument->loadXML(bstrXml))
				{
					if (ReadRootNode())
					{
						m_str = strValue;

                                                return true;
					}
				}
			}
			catch (...)
			{
				
			}

			Clear();

                        return false;
		}
		void SetProperty(const CString& strName, const CString& strValue)
		{
			if (NULL != m_pXmlDocument)
				m_pXmlDocument->setProperty( _bstr_t((LPCTSTR)strName), _variant_t((LPCTSTR)strValue));
		}
	
                bool OpenFromXmlString(const CString& strXml)
		{
			return SetXmlString(strXml);
		}
                bool OpenFromXmlNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			m_pXmlNode = pXmlNode;

			if (pXmlNode == NULL)
                                return false;

                        return true;
		}
                bool OpenFromFile(const CString& strFilePath)
		{
			if (NULL == m_pXmlDocument)
                                return true;

			BSTR bstrFilePath = strFilePath.AllocSysString();
                        bool bSuccess = false;

			try
			{
				if (VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
					bSuccess = ReadRootNode();
			}
			catch (...)
			{
			}

			if (!bSuccess)
				Clear();

			SysFreeString(bstrFilePath);

			return bSuccess;
		}
	
		CString ReadNodeName()
		{
			if (NULL == m_pXmlNode)
				return _T("");

			CString sName;
			try
			{
				BSTR bsName;
				m_pXmlNode->get_nodeName(&bsName);
				sName = bsName;
				SysFreeString(bsName);
			}
			catch (...)
			{
			}			
			return sName;
		}
                bool ReadRootNode()
		{
			if (NULL == m_pXmlDocument)
                                return false;

			try
			{
				m_pXmlNode = m_pXmlDocument->firstChild;

                                return true;
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;

                        return false;
		}
                bool ReadRootNode(const CString& strRootNodeName)
		{
			if (_T("") == strRootNodeName)
				return ReadRootNode();

			if (NULL == m_pXmlDocument)
                                return false;

			try
			{
				_bstr_t bstrNode; bstrNode = strRootNodeName;

				m_pXmlNode = m_pXmlDocument->selectSingleNode(bstrNode);

                                return true;
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;

                        return false;
		}
                bool ReadNode(const CString& strSubNodeName)
		{
			if (NULL == m_pXmlNode)
                                return false;

			try
			{
				_bstr_t bstrNode; bstrNode = strSubNodeName;

				XML::IXMLDOMElementPtr pNewNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL != pNewNode)
				{
					m_pXmlNode = pNewNode;
                                        return true;
				}
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;

                        return false;
		}
                bool ReadChilds()
		{
			if (NULL == m_pXmlNode)
                                return false;

			try
			{
				XML::IXMLDOMNodeListPtr pNewNodeList = m_pXmlNode->GetchildNodes();

				if (NULL != pNewNodeList)
				{
					m_pXmlNodeList = pNewNodeList;
                                        return true;
				}
			}
			catch (...)
			{
			}

			m_pXmlNodeList = NULL;

                        return false;
		}
                bool ReadNodeList(const CString& strSubNodesName)
		{
			if (NULL == m_pXmlNode)
                                return false;

			try
			{
				_bstr_t bstrNodes; bstrNodes = strSubNodesName;

				XML::IXMLDOMNodeListPtr pNewNodeList = m_pXmlNode->selectNodes(bstrNodes);

				if (NULL != pNewNodeList)
				{
					m_pXmlNodeList = pNewNodeList;
                                        return true;
				}
			}
			catch (...)
			{
			}

			m_pXmlNodeList = NULL;

                        return false;
		}
	
		CString ReadNodeAttributeOrValue(const CString& strSubNodeName, const CString& def = _T(""))
		{
			//CString strInvalidValue = _T("x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = ReadNodeAttribute(strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CString strNodeValue = ReadNodeValue(strSubNodeName, strInvalidValue);

			if (strNodeValue != strInvalidValue)
				return strNodeValue;

			return def;
		}
		CString ReadNodeValue(const CString& strSubNodeName, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeValue(m_pXmlNode, strSubNodeName, def);
		}
		CString ReadNodeAttribute(const CString& strAttributeName, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeAttrib(m_pXmlNode, strAttributeName, def);
		}
		CString ReadNodeText(const CString& def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeText(m_pXmlNode, def);
		}
		CString ReadNodeXml(const CString& def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeXml(m_pXmlNode, def);
		}
                bool ReadNodeTextTyped(_variant_t* variant, const CString& type = _T("bin.base64"))
		{
			if (NULL == m_pXmlNode)
                                return false;

			return GetNodeTextTyped(m_pXmlNode, variant, type);
		}
		
		CString ReadNodeName(int nIndex)
		{
			if (NULL == m_pXmlNodeList)
				return _T("");

			CString sName;
			try
			{
				BSTR bsName;
				m_pXmlNodeList->item[nIndex]->get_nodeName(&bsName);
				sName = bsName;
				SysFreeString(bsName);
			}
			catch (...)
			{
			}			
			return sName;
		}
		CString ReadNodeAttributeOrValue(int nIndex, const CString& strSubNodeName, const CString& def = _T(""))
		{
			//CString strInvalidValue = _T("dx(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = ReadNodeAttribute(nIndex, strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CString strNodeValue = ReadNodeValue(nIndex, strSubNodeName, strInvalidValue);

			if (strNodeValue != strInvalidValue)
				return strNodeValue;

			return def;
		}
		CString ReadNodeValue(int nIndex, const CString& strSubNodeName, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeValue(m_pXmlNodeList, nIndex, strSubNodeName, def);
		}
		CString ReadNodeAttribute(int nIndex, const CString& strAttributeName, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeAttrib(m_pXmlNodeList, nIndex, strAttributeName, def);
		}
		CString ReadNodeText(int nIndex, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeText(m_pXmlNodeList, nIndex, def);
		}
		CString ReadNodeXml(int nIndex, const CString& def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeXml(m_pXmlNodeList, nIndex, def);
		}
                bool ReadNodeTextTyped(int nIndex, _variant_t* variant, const CString& type = _T("bin.base64"))
		{
			if (NULL == m_pXmlNodeList)
                                return false;

			return GetNodeTextTyped(m_pXmlNodeList, nIndex, variant, type);
		}
		
		int GetLengthList()
		{
			try
			{
				if (NULL == m_pXmlNodeList)
					return 0;

				int length = m_pXmlNodeList->length;

				return length;
			}
			catch (...)
			{
			}

			return 0;
		}
        bool GetNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			pXmlNode = m_pXmlNode;

			return (m_pXmlNode != NULL);
		}
        bool GetNode(int nIndex, XML::IXMLDOMNodePtr& pXmlNode)
		{
			if (m_pXmlNodeList == NULL)
                                return false;

			pXmlNode = m_pXmlNodeList->item[nIndex];

            return true;
		}
	};
};
	
//Расширение для сохранения в XML свойств интерфейса
namespace XmlUtils
{
	static LPCTSTR g_cpszSADescriptorNodeName	= _T("SADescriptor");
	static LPCTSTR g_cpszSADimsAttributeName	= _T("SADims");
	static LPCTSTR g_cpszSASizeNodeNameFormat	= _T("Size%d");
	static LPCTSTR g_cpszSADataNodeName			= _T("SAData");
	static LPCTSTR g_cpszVariantTypeNodeAttr	= _T("type");
	
	struct SDispatchProperty
	{
		SDispatchProperty()
			: m_sName(_T(""))
			//, m_lDISPID(0)
		{
		}
		SDispatchProperty(const CString &sName, const VARIANT &var)
			: m_sName(sName)
			, m_varValue(var)
		{
			
		}
		CString m_sName;
		//DISPID m_lDISPID;
		VARIANT m_varValue;
	};
	
	class CDispatchPropertyList 
		: public std::vector<SDispatchProperty>
	{
	public:
		CDispatchPropertyList()
		{
		}
		virtual ~CDispatchPropertyList()
		{
			size_t nCount = GetCount();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++)
			{
				if ((VT_BSTR==GetAt(nIndex).m_varValue.vt)&&(NULL!=GetAt(nIndex).m_varValue.bstrVal))
				{
					SysFreeString(GetAt(nIndex).m_varValue.bstrVal);
				}
				else if ((0!=(VT_ARRAY & GetAt(nIndex).m_varValue.vt))&&(NULL!=GetAt(nIndex).m_varValue.parray))
					SafeArrayDestroy(GetAt(nIndex).m_varValue.parray);
			}
		}
		const SDispatchProperty& GetAt( size_t iElement)
		{
			return at(iElement);
		}
		size_t GetCount() { return size();}
		size_t Add()
		{
			SDispatchProperty elem;
			push_back(elem);

			return size()-1;
		}
		size_t Add(SDispatchProperty & elem)
		{
			push_back(elem);
			
			return size()-1;
		}
	};

	//Только свойства которые не имеют дополнительных параметров на вход
        static bool GetPropertyList(IDispatch *pDisp, CDispatchPropertyList &arProp, INVOKEKIND nType)
	{
		if (NULL==pDisp)
                        return false;
		ITypeInfo *pTypeInfo = NULL;
		pDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &pTypeInfo);
		if (NULL==pTypeInfo)
                        return false;
		
		TYPEATTR* pTypeAttr = NULL;
		HRESULT hRes = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if (S_OK!=hRes)
		{
			pTypeInfo->Release();
                        return false;
		}
		int nMethodCount = pTypeAttr->cFuncs;
		for (int i=0; i<nMethodCount; i++)
		{
			SDispatchProperty oPropRec;
			FUNCDESC* pFuncDesc;
			hRes = pTypeInfo->GetFuncDesc(i, &pFuncDesc); 
			if (S_OK!=hRes)
				continue;

			if (0!=pFuncDesc->cParams)
				continue;//СОХРАНЯЕМ ТОЛЬКО ТЕ СВОЙСТВА У КОТОРЫХ НЕТ ПАРАМЕТРОВ
			if (nType!=pFuncDesc->invkind)
				continue;

//				oPropRec.m_lDISPID = pFuncDesc->memid;
			BSTR bsName; 
			unsigned int nCount;
			hRes = pTypeInfo->GetNames(pFuncDesc->memid, &bsName, 1, &nCount); 
			if (S_OK!=hRes)
				continue;

			oPropRec.m_sName = bsName; 
			SysFreeString(bsName);

			{
				DISPPARAMS dispParam;
				dispParam.cArgs = 0;
				dispParam.cNamedArgs = 0;
				dispParam.rgdispidNamedArgs = NULL;
				dispParam.rgvarg = NULL;

				hRes = pDisp->Invoke(pFuncDesc->memid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, 
					&dispParam, &oPropRec.m_varValue, NULL, NULL);
			}
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			arProp.Add(oPropRec);
		}			
		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeInfo->Release();		
        return true;
	}
        static bool ApplyPropertyList(IDispatch *pDisp, CDispatchPropertyList &arProp)
	{
		if (NULL==pDisp)
                        return false;
		
		long lCount = (long)arProp.GetCount();
		for (long i=0; i<lCount; i++)
		{
			DISPID dispid;
			CStringW swName; swName = arProp[i].m_sName; 
			OLECHAR FAR* szMember = swName.GetBuffer();
			pDisp->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

			DISPPARAMS dispParam;
			DISPID dispidNamed = DISPID_PROPERTYPUT;
			dispParam.cArgs = 1;
			dispParam.cNamedArgs = 1;
			dispParam.rgdispidNamedArgs = &dispidNamed;			
			dispParam.rgvarg = &arProp[i].m_varValue;

			HRESULT hr = pDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, 
				&dispParam, NULL, NULL, NULL);

		}
        return true;
	}

	class CDispatchXmlWriter 
		: public CXmlWriter
	{
	public:
		CDispatchXmlWriter()
		{
		}
		~CDispatchXmlWriter()
		{
		}
		void WriteNode(const CString& sName, const CLSID &clsid)
		{
			CStringW swGUID;
			int nSize = StringFromGUID2(clsid, swGUID.GetBuffer(50), 50);
			swGUID.ReleaseBuffer(nSize);
			CString sGUID; sGUID = swGUID;
			CXmlWriter::WriteNode(sName,sGUID);
		}
		void WriteNode(LPSAFEARRAY psa)
		{
			if (NULL==psa)
				return;
			UINT nDim = psa->cDims;
			if (0==nDim)
				return;
			ULONG ulSADataSize = 1;
			for (UINT nIndex = 0; nIndex < nDim; nIndex++)
				ulSADataSize *= psa->rgsabound[nIndex].cElements;
			if (0==ulSADataSize)
				return;

			CString sTemp;
            CXmlWriter::WriteNodeBegin(g_cpszSADescriptorNodeName, true);
			CXmlWriter::WriteAttribute(g_cpszSADimsAttributeName, (int)nDim);
			CXmlWriter::WriteString(_T(">"));
			for (UINT nIndex = 0; nIndex < nDim; nIndex++)
			{
				sTemp.Format(g_cpszSASizeNodeNameFormat,nIndex);
				CXmlWriter::WriteNode(sTemp, (long)(psa->rgsabound[nIndex].cElements));
			}
                        CXmlWriter::WriteNodeEnd(g_cpszSADescriptorNodeName, false);
			if (BinaryToBase64((LPBYTE)psa->pvData, ulSADataSize, sTemp))			
				CXmlWriter::WriteNode(g_cpszSADataNodeName, sTemp);
			else
				CXmlWriter::WriteNode(g_cpszSADataNodeName, _T(""));
		}
		void WriteNode(const CString &sName, const VARIANT &val)
		{
			if ((0!=(VT_VECTOR & val.vt)) || (0!=(VT_BYREF & val.vt)))
				return;
			if ((0!=(VT_ARRAY & val.vt))&&(VT_UI1 != (VT_TYPEMASK & val.vt)))
				return;

            CXmlWriter::WriteNodeBegin(sName, true);
			CXmlWriter::WriteAttribute(g_cpszVariantTypeNodeAttr, val.vt);
			CXmlWriter::WriteString(_T(">"));
			if (0!=(VT_ARRAY & val.vt))
				WriteNode(val.parray);
			else
				WriteString(VariantToString(val));
                        CXmlWriter::WriteNodeEnd(sName, false);
		}
		void WritePropertyList(CDispatchPropertyList &arProp)
		{
			size_t nCount = arProp.GetCount();
			for (size_t nIndex = 0; nIndex<nCount; nIndex++)
			{
				const SDispatchProperty &oPropRec = arProp[nIndex];
				WriteNode(oPropRec.m_sName, oPropRec.m_varValue);
			}
		}
	protected:
                static bool BinaryToBase64(const BYTE* pData, long lSize, CString& sResult)
		{
			if ((NULL==pData) || (0==lSize))
                                return false;

			int nStrSize = Base64EncodeGetRequiredLength(lSize);

			CStringA saTemp;
			LPSTR pStrData = saTemp.GetBuffer(nStrSize + 1);
                        bool bSuccess = Base64Encode(pData, lSize, pStrData, &nStrSize);
			
			pStrData[nStrSize] = '\0';
			saTemp.ReleaseBuffer();
			
			sResult = saTemp;
			return bSuccess;
		}

		static CString VariantToString(VARIANT val)
		{
			CString sVal = _T("");
			switch (VT_TYPEMASK & val.vt)
			{
			case VT_EMPTY:
				break;
			case VT_NULL:
				sVal = _T("");
				break;
			case VT_I2:
				sVal.Format(_T("%d"), val.iVal);
				break;
			case VT_I4:
				sVal.Format(_T("%d"), val.lVal);
				break;
			case VT_R4:
				sVal.Format(_T("%f"), val.fltVal);
				break;
			case VT_R8:
				sVal.Format(_T("%f"), val.dblVal);
				break;
			case VT_DATE:
				sVal.Format(_T("%f"), val.date);
				break;
			case VT_BSTR:
				sVal = val.bstrVal;
				break;
			case VT_BOOL:
				sVal = (VARIANT_TRUE == val.boolVal) ? _T("1") : _T("0");
				break;
			case VT_VARIANT:
				sVal = _T("");
				break;
			case VT_UNKNOWN:
				sVal = _T("");
				//можно сохранять пытаясь получить IDispatch интерфейс, но вроде нам он не надо
				break;
			case VT_I1:
				sVal.Format(_T("%d"), val.cVal);
				break;
			case VT_UI1:
				sVal.Format(_T("%d"), val.bVal);
				break;
			case VT_UI2:
				sVal.Format(_T("%d"), val.uiVal);
				break;
			case VT_UI4:
				sVal.Format(_T("%d"), val.ulVal);
				break;
			case VT_I8:
				sVal.Format(_T("%d"), val.llVal);
				break;
			case VT_UI8:
				sVal.Format(_T("%d"), val.ullVal);
				break;
			case VT_INT:
				sVal.Format(_T("%d"), val.intVal);
				break;
			case VT_UINT:
				sVal.Format(_T("%d"), val.uintVal);
				break;
			case VT_DECIMAL:
			case VT_CY:
			case VT_SAFEARRAY:
			case VT_VOID:
			case VT_HRESULT:
			case VT_CARRAY:
			case VT_USERDEFINED:
			case VT_LPSTR:
			case VT_LPWSTR:
			case VT_RECORD:
			case VT_INT_PTR:
			case VT_UINT_PTR:
			case VT_FILETIME:
			case VT_BLOB:
			case VT_STREAM:
			case VT_STORAGE:
			case VT_STREAMED_OBJECT:
			case VT_STORED_OBJECT:
			case VT_BLOB_OBJECT:
			case VT_CF:
			case VT_CLSID:
			case VT_VERSIONED_STREAM:
			case VT_BSTR_BLOB:
				sVal = _T("");
				break;
			}

			return sVal;
		}

	};
	class CDispatchXmlReader 
		: public XmlUtils::CXmlReader
	{
	public:
		CDispatchXmlReader()
		{
		}
		~CDispatchXmlReader()
		{
		}
		size_t GetPropertyList(const CString &strSubNodeName, CDispatchPropertyList &arProp)
		{
			if (NULL == m_pXmlNode)
				return 0;

			try
			{
				_bstr_t bstrNode; bstrNode = strSubNodeName;
				XML::IXMLDOMNodePtr pNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL==pNode)
					return 0;
				XML::IXMLDOMNodeListPtr pNodeList = pNode->selectNodes("*");

				if (NULL != pNodeList)
				{
					long lNodeCount = pNodeList->length;
					CString sNodeName, sType, sValue;
					VARIANT val;
					for (long lIndex = 0; lIndex<lNodeCount; lIndex++)
					{
						XML::IXMLDOMNodePtr ptrNode = pNodeList->item[lIndex];
						sNodeName = ptrNode->baseName.GetBSTR();

						sType = XmlUtils::GetNodeAttrib(ptrNode, g_cpszVariantTypeNodeAttr);
						sValue = ptrNode->text.GetBSTR();
						val.vt = _ttoi(sType);
						if ((0!=(VT_VECTOR & val.vt)) || (0!=(VT_BYREF & val.vt)))
							continue;
						if (0!=(VT_ARRAY & val.vt))
						{
							if (VT_UI1 != (VT_TYPEMASK & val.vt))
								continue;
							if (!GetSafeArray(ptrNode, val.parray))
								continue;
						}						
						else
						{
							if (!GetVariantValue(sType, sValue, val))
								continue;
						}
						arProp.Add(SDispatchProperty(sNodeName, val));						
					}					
				}
			}
			catch (...)
			{
			}
			return (arProp.GetCount());
		}
	protected:
                bool GetSafeArray(XML::IXMLDOMNodePtr ptrNode, LPSAFEARRAY &psa)
		{
			long lDims = 0;
			LPSAFEARRAYBOUND psabound = NULL;
			try
			{
				long lSize = 1;
				{
					_bstr_t bstrNode; bstrNode = g_cpszSADescriptorNodeName;
					XML::IXMLDOMElementPtr pDescriptionNode = ptrNode->selectSingleNode(bstrNode);
					CString sTemp = XmlUtils::GetNodeAttrib(pDescriptionNode, g_cpszSADimsAttributeName);
					lDims = _ttoi(sTemp);
					if (0==lDims)
                                                return false;

					psabound = new SAFEARRAYBOUND[lDims];
					if (NULL==psabound)
                                                return false;


					for (long i=0; i<lDims; i++)
					{
						sTemp.Format(g_cpszSASizeNodeNameFormat, i);
						sTemp = XmlUtils::GetNodeValue(pDescriptionNode, sTemp);
						psabound[i].lLbound = 0;
						psabound[i].cElements = _ttoi(sTemp);
						lSize *= (psabound[i].cElements);
					}
					if (0==lSize)
					{
						delete []psabound;
                                                return false;
					}
				}
				psa = SafeArrayCreate(VT_UI1, lDims, psabound);
				if (NULL==psa)
                                        return false;

				_bstr_t bstrNode; bstrNode = g_cpszSADataNodeName;
				XML::IXMLDOMElementPtr pDataNode = ptrNode->selectSingleNode(bstrNode);
				CString sTemp; 
				sTemp = pDataNode->text.GetBSTR();
				if (!Base64ToBinary(sTemp, (LPBYTE)psa->pvData, lSize))
				{
					delete []psabound;
                                        return false;
				}
			}
			catch (...)
			{
				if (NULL != psabound)
					delete []psabound;
                                return false;
			}
			delete []psabound;
                        return true;
		}
                bool GetVariantValue(const CString &sType, const CString &sValue, VARIANT &val)
		{
			if ((sType.IsEmpty())||(sValue.IsEmpty()))
                                return false;
			val.vt = _ttoi(sType);

                        bool bRet = true;
			switch (VT_TYPEMASK & val.vt)
			{
			case VT_I2:
				val.iVal = _ttoi(sValue);
				break;
			case VT_I4:
				val.lVal = _ttoi(sValue);
				break;
			case VT_R4:
				val.fltVal = (float)_tstof(sValue);
				break;
			case VT_R8:
				val.dblVal = _tstof(sValue);
				break;
			case VT_DATE:
				val.date = _tstof(sValue);
				break;
			case VT_BSTR:
				val.bstrVal = sValue.AllocSysString();
				break;
			case VT_BOOL:
				val.boolVal = (0!=_ttoi(sValue)) ? VARIANT_TRUE : VARIANT_FALSE;
				break;
			case VT_I1:
				val.cVal = _ttoi(sValue);
				break;
			case VT_UI1:
				val.bVal = _ttoi(sValue);
				break;
			case VT_UI2:
				val.uiVal = _ttoi(sValue);
				break;
			case VT_UI4:
				val.ulVal = _ttoi(sValue);
				break;
			case VT_I8:
				val.llVal = _ttoi(sValue);
				break;
			case VT_UI8:
				val.ullVal = _ttoi(sValue);
				break;
			case VT_INT:
				val.intVal = _ttoi(sValue);
				break;
			case VT_UINT:
				val.uintVal = _ttoi(sValue);
				break;
			case VT_DECIMAL:
			case VT_CY:
			case VT_EMPTY:
			case VT_NULL:
			case VT_SAFEARRAY:
			case VT_VOID:
			case VT_HRESULT:
			case VT_CARRAY:
			case VT_USERDEFINED:
			case VT_LPSTR:
			case VT_LPWSTR:
			case VT_RECORD:
			case VT_INT_PTR:
			case VT_UINT_PTR:
			case VT_FILETIME:
			case VT_BLOB:
			case VT_STREAM:
			case VT_STORAGE:
			case VT_STREAMED_OBJECT:
			case VT_STORED_OBJECT:
			case VT_BLOB_OBJECT:
			case VT_CF:
			case VT_CLSID:
			case VT_VERSIONED_STREAM:
			case VT_BSTR_BLOB:
                                bRet = false;
				break;
			}

			return bRet;
		}
                static bool Base64ToBinary(const CString& sData, BYTE* pData, int nSize)
		{
			if ((NULL==pData) || (0==nSize))
                                return false;
//			LPSTR pStrData = sData.GetBuffer(nStrSize + 1);
			CStringA saData; saData = sData;
			return Base64Decode((LPCSTR)saData/*.GetBuffer()*/, saData.GetLength(), pData, &nSize);
		}
	};
};
	
// Расширение для простого чтения xml через классы
namespace XmlUtils
{
	class IXmlNode
	{
	public:

                virtual bool FromXmlNode(XML::IXMLDOMNodePtr& pXmlNode) = 0;
                virtual bool FromXmlString(const CString& strXml) = 0;
                virtual bool FromXmlFile(const CString& strXmlFilePath, bool bRemoveRootNode = false) = 0;
	};
	class CXmlNodes
	{
		XML::IXMLDOMNodeListPtr m_pXmlNodes;
		
	public:
		
		CXmlNodes()
		{
			Clear();
		}
		
		void Clear()
		{
			m_pXmlNodes = NULL;
		}
                bool IsValid()
		{
			return (NULL != m_pXmlNodes);
		}
		
                bool FromXmlNodes(XML::IXMLDOMNodeListPtr& pXmlNodes)
		{
			try
			{
				Clear();

				m_pXmlNodes = pXmlNodes;
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
		
		int GetCount()
		{
			if (!IsValid())
				return 0;

			return m_pXmlNodes->length;
		}
                bool GetAt(int nIndex, IXmlNode& oXmlNode)
		{
			if (nIndex < 0)
                                return false;

			try
			{
				XML::IXMLDOMNodePtr pXmlNode = NULL;

				pXmlNode = m_pXmlNodes->item[nIndex];

				return oXmlNode.FromXmlNode(pXmlNode);
			}
			catch (...)
			{
			}

                        return false;
		}
	};
	class CXmlNode : public IXmlNode
	{
		XML::IXMLDOMDocument2Ptr	m_pXmlDocument; // для нода, созданного из строки
		XML::IXMLDOMNodePtr m_pXmlNode;

		IXMLDOMNamedNodeMapPtr m_pAttributes;
			
	private:
		
		void CreateDocument()
		{
			if (NULL == m_pXmlDocument)
			{
				if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument30))))
				{
					if (FAILED(m_pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument))))
					{
						m_pXmlDocument = NULL;
					}
				}
				//if (NULL != m_pXmlDocument)
				//{
				//	m_pXmlDocument->setProperty( _bstr_t(_T("SelectionLanguage")), _variant_t(_T("XPath")) );
				//}

				if (NULL != m_pXmlDocument)
				{
					m_pXmlDocument->put_preserveWhiteSpace(VARIANT_TRUE);
				}
			}
		}
		void ReadRootNode()
		{
			m_pXmlNode = m_pXmlDocument->firstChild;
		}
	public:
		void ReadRootNode(const CString& strRootNodeName)
		{
			if (strRootNodeName.IsEmpty())
				return ReadRootNode();

			_bstr_t bstrNode; bstrNode = strRootNodeName;

			m_pXmlNode = m_pXmlDocument->selectSingleNode(bstrNode);
		}

		template<typename T>
		void ReadAllAttributes(T& strNames, T& strValues)
		{
			if (!IsValid())
				return;

			if (NULL == m_pAttributes)
			{
				m_pAttributes = m_pXmlNode->attributes;
				if (NULL == m_pAttributes)
					return;
			}

			long lCountAttr = 0;
			m_pAttributes->get_length(&lCountAttr);

			for (long i = 0; i < lCountAttr; ++i)
			{
				IXMLDOMNodePtr tempNode;
				m_pAttributes->get_item(i, &tempNode);

				BSTR bsName;
				BSTR bsValue;
				tempNode->get_nodeName(&bsName);
				tempNode->get_text(&bsValue);

				strNames.push_back((CString)bsName);
				strValues.push_back((CString)bsValue);

				SysFreeString(bsName);
				SysFreeString(bsValue);
			}
		}
		
	public:
		
		CXmlNode()
		{
			Clear();
		}
		
		void Clear()
		{
			m_pXmlDocument	= NULL;
			m_pXmlNode		= NULL;

			m_pAttributes	= NULL;
		}
                inline bool IsValid() const
		{
			return (NULL != m_pXmlNode);
		}
		
                virtual bool FromXmlNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			try
			{
				Clear();

				m_pXmlNode = pXmlNode;
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
                virtual bool FromXmlString(const CString& strXml)
		{
			return FromXmlString(strXml, _T(""));
		}
                virtual bool FromXmlString(const CString& strXml, const CString& strNamespaces)
		{
			try
			{
				Clear();

				CreateDocument();

				if (_T("") != strNamespaces)
				{
					m_pXmlDocument->setProperty( _T("SelectionNamespaces"), _variant_t((LPCTSTR)strNamespaces));
				}

				_bstr_t bstrXml; bstrXml = strXml;

				if (!strXml.IsEmpty() && VARIANT_TRUE == m_pXmlDocument->loadXML(bstrXml))
					ReadRootNode();
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
                virtual bool FromXmlFile(const CString& strXmlFilePath, bool bRemoveRootNode = false)
		{
			BSTR bstrFilePath = strXmlFilePath.AllocSysString();
                        bool bSuccess = false;

			try
			{
				Clear();

				CreateDocument();

				if ( bRemoveRootNode && VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
				{
					XML::IXMLDOMNodeListPtr ptrNodeList = m_pXmlDocument->childNodes;
					int nCount = ptrNodeList->length;
					for (int nIndex = 0; nIndex < nCount; nIndex++ )
					{
						XML::IXMLDOMNodePtr ptrNode = ptrNodeList->item[nIndex];
						int nType = ptrNode->nodeType;
						if (NODE_ELEMENT == nType)
						{
							m_pXmlNode = ptrNode;
							break;
						}
					}

                                        bSuccess = true;
				}
				else
				{
					if (VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
                                                bSuccess = true;

					ReadRootNode();
				}
			}
			catch (...)
			{
                                bSuccess = false;
			}

			SysFreeString(bstrFilePath);

			if (!bSuccess)
				Clear();

			return IsValid();
		}
                virtual bool FromXmlFile2(const CString& strXmlFilePath)
		{
			BSTR bstrFilePath = strXmlFilePath.AllocSysString();
                        bool bSuccess = false;

			try
			{
				Clear();

				CreateDocument();

				if (VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
                                        bSuccess = true;

				m_pXmlNode = m_pXmlDocument->lastChild;
			}
			catch (...)
			{
                                bSuccess = false;
			}

			SysFreeString(bstrFilePath);

			if (!bSuccess)
				Clear();

			return IsValid();
		}
		
		CString GetName()
		{
			if (!IsValid())
				return _T("");

			CString strName;

			try
			{
				BSTR bstrName;

				m_pXmlNode->get_nodeName(&bstrName);

				strName = bstrName;

				SysFreeString(bstrName);
			}
			catch (...)
			{
			}			

			return strName;
		}
		
		// attribute ---------------------------------------------------
		BSTR ReadAttributeBase(const _bstr_t& bstrName)
		{
			if (!IsValid())
				return NULL;

			if (NULL == m_pAttributes)
			{
				m_pAttributes = m_pXmlNode->attributes;
				if (NULL == m_pAttributes)
					return NULL;
			}

			IXMLDOMNodePtr tempNode;
			m_pAttributes->getNamedItem(bstrName, &tempNode);
			
			if (NULL == tempNode)
				return NULL;

			BSTR strRes;
			tempNode->get_text(&strRes);
			return strRes;
		}

		template<typename T>
		AVSINLINE void ReadAttributeBase(const _bstr_t& bsName, T& value)
		{
			BSTR bsVal = ReadAttributeBase(bsName);
			value = bsVal;
			SysFreeString(bsVal);
		}

		inline BSTR ReadAttribute(const CString& strAttibuteName)
		{
			_bstr_t bstrName = strAttibuteName;
			return ReadAttributeBase(bstrName);
		}
		//text----------------------------------------------------------
		BSTR ReadNodeTextBase(const _bstr_t& bsName)
		{
			if (!IsValid())
				return NULL;

			try
			{
				XML::IXMLDOMNodePtr pNewNode = m_pXmlNode->selectSingleNode(bsName);

				if (NULL == pNewNode)
					return NULL;

				BSTR bsXml = NULL;
				pNewNode->get_text(&bsXml);
				
				return bsXml;
			}
			catch (...)
			{
			}

			return NULL;
		}

		template <typename T>
		AVSINLINE void ReadNodeValueBase(const _bstr_t& bsName, T& value)
		{
			BSTR bsValue = ReadNodeTextBase(bsName);
			value = bsValue;
			SysFreeString(bsValue);
		}

		inline BSTR ReadNodeText(const CString& strName)
		{
			_bstr_t bsName = strName;
			return ReadNodeTextBase(bsName);
		}
		inline CString ReadValueString(const CString& sName, const CString& nDef = _T(""))
		{
			_bstr_t bsName = sName;
			BSTR bsRes = ReadNodeTextBase(bsName);
			if (NULL == bsRes)
				return nDef;

			CString str = (CString)bsRes;
			SysFreeString(bsRes);
			return str;
		}

		// int ------------------------------------------------------------------------------
		inline int ReadAttributeIntBase(const _bstr_t& bstrName, const int& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			int nRes = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline int ReadAttributeInt(const CString& str, const int& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadAttributeIntBase(bsName, nDef);
		}
		inline int ReadValueIntBase(const _bstr_t& bstrName, const int& nDef = 0)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			int nRes = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline int ReadValueInt(const CString& str, const int& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadValueIntBase(bsName, nDef);
		}
		inline void UpdateAttributeBase(const _bstr_t& bstrName, int& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeBase(const _bstr_t& bstrName, long& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = (long)XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttribute(const CString& strName, int& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeBase(bsName, nValue);
		}
		inline void UpdateAttribute(const CString& strName, long& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeBase(bsName, nValue);
		}
		inline void UpdateValueBase(const _bstr_t& bstrName, int& nValue)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateValueBase(const _bstr_t& bstrName, long& nValue)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = (long)XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateValue(const CString& strName, int& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateValueBase(bsName, nValue);
		}
		inline void UpdateValue(const CString& strName, long& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateValueBase(bsName, nValue);
		}
		inline int ReadAttributeOrValueIntBase(const _bstr_t& bstrName, const int& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return ReadValueIntBase(bstrName, nDef);
		
			int nRes = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline int ReadAttributeOrValueInt(const CString& strName, const int& nDef = 0)
		{
			_bstr_t bsName = strName;
			return ReadAttributeOrValueIntBase(bsName, nDef);
		}
		inline void UpdateAttributeOrValueBase(const _bstr_t& bstrName, int& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return UpdateValueBase(bstrName, nValue);

			nValue = XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeOrValueBase(const _bstr_t& bstrName, long& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return UpdateValueBase(bstrName, nValue);

			nValue = (long)XmlUtils::GetInteger(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeOrValue(const CString& strName, int& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeOrValueBase(bsName, nValue);
		}
		inline void UpdateAttributeOrValue(const CString& strName, long& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeOrValueBase(bsName, nValue);
		}
		// double ---------------------------------------------------------------------
		inline double ReadAttributeDoubleBase(const _bstr_t& bstrName, const double& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			double nRes = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}		
		inline double ReadValueDoubleBase(const _bstr_t& bstrName, const double& nDef = 0)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			double nRes = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline double ReadValueDouble(const CString& str, const double& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadValueDoubleBase(bsName, nDef);
		}
		inline void UpdateAttributeBase(const _bstr_t& bstrName, double& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttribute(const CString& strName, double& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeBase(bsName, nValue);
		}
		inline void UpdateValueBase(const _bstr_t& bstrName, double& nValue)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateValue(const CString& strName, double& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateValueBase(bsName, nValue);
		}
		inline double ReadAttributeOrValueDoubleBase(const _bstr_t& bstrName, const double& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return ReadValueDoubleBase(bstrName, nDef);

			double nRes = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline double ReadAttributeOrValueDouble(const CString& strName, const double& nDef = 0)
		{
			_bstr_t bsName = strName;
			return ReadAttributeOrValueDoubleBase(bsName, nDef);
		}
		inline void UpdateAttributeOrValueBase(const _bstr_t& bstrName, double& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return UpdateValueBase(bstrName, nValue);

			nValue = XmlUtils::GetDouble(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeOrValue(const CString& strName, double& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeOrValueBase(bsName, nValue);
		}
		
		// float ----------------------------------------------------------------------
		inline float ReadAttributeFloatBase(const _bstr_t& bstrName, const float& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			float nRes = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline double ReadAttributeDouble(const CString& str, const double& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadAttributeDoubleBase(bsName, nDef);
		}
		inline float ReadAttributeFloat(const CString& str, const float& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadAttributeFloatBase(bsName, nDef);
		}
		inline float ReadValueFloatBase(const _bstr_t& bstrName, const float& nDef = 0)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return nDef;

			float nRes = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline float ReadValueFloat(const CString& str, const float& nDef = 0)
		{
			_bstr_t bsName = str;
			return ReadValueFloatBase(bsName, nDef);
		}
		inline void UpdateAttributeBase(const _bstr_t& bstrName, float& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttribute(const CString& strName, float& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeBase(bsName, nValue);
		}
		inline void UpdateValueBase(const _bstr_t& bstrName, float& nValue)
		{
			BSTR bsRes = ReadNodeTextBase(bstrName);
			if (NULL == bsRes)
				return;

			nValue = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateValue(const CString& strName, float& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateValueBase(bsName, nValue);
		}
		inline float ReadAttributeOrValueFloatBase(const _bstr_t& bstrName, const float& nDef = 0)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return ReadValueFloatBase(bstrName, nDef);

			float nRes = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
			return nRes;
		}
		inline float ReadAttributeOrValueFloat(const CString& strName, const float& nDef = 0)
		{
			_bstr_t bsName = strName;
			return ReadAttributeOrValueFloatBase(bsName, nDef);
		}
		inline void UpdateAttributeOrValueBase(const _bstr_t& bstrName, float& nValue)
		{
			BSTR bsRes = ReadAttributeBase(bstrName);
			if (NULL == bsRes)
				return UpdateValueBase(bstrName, nValue);

			nValue = XmlUtils::GetFloat(bsRes);
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeOrValue(const CString& strName, float& nValue)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeOrValueBase(bsName, nValue);
		}
		//string-----------------------------------------------------------------------
		inline CString GetAttributeBase(const _bstr_t& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			BSTR bsRes = ReadAttributeBase(strAttributeName);
			if (NULL == bsRes)
				return strDefaultValue;

			CString strRes = (CString)bsRes;
			SysFreeString(bsRes);
			return strRes;
		}
		inline CString GetAttribute(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			_bstr_t bsName = strAttributeName;
			return GetAttributeBase(bsName, strDefaultValue);
		}
		inline CString GetAttributeNoSpace(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			_bstr_t bsName = strAttributeName;
			BSTR bsRes = ReadAttributeBase(bsName);
			if (NULL == bsRes)
				return strDefaultValue;

			int nLen = SysStringLen(bsRes);		
			if (0 == nLen)
			{
				SysFreeString(bsRes);
				return _T("");
			}

			int nStart = 0;
			while (nStart < nLen)
			{
				if (bsRes[nStart] != WCHAR(' '))
					break;
				++nStart;
			}
			int nEnd = nLen - 1;
			while (nEnd > nStart)
			{
				if (bsRes[nEnd] != WCHAR(' '))
					break;
				--nEnd;
			}
			if (nEnd < nStart)
			{
				SysFreeString(bsRes);
				return _T("");
			}
			CString strRes(bsRes + nStart, nEnd - nStart + 1);
			SysFreeString(bsRes);
			return strRes;
		}
		inline CString GetValueBase(const _bstr_t& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			BSTR bsRes = ReadNodeTextBase(strAttributeName);
			if (NULL == bsRes)
				return strDefaultValue;

			CString strRes = (CString)bsRes;
			SysFreeString(bsRes);
			return strRes;
		}
		inline CString GetValue(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			_bstr_t bsName = strAttributeName;
			return GetValueBase(bsName);
		}
		inline void UpdateAttributeStringBase(const _bstr_t& strName, CString& strVal)
		{
			BSTR bsRes = ReadAttributeBase(strName);
			if (NULL == bsRes)
				return;

			strVal = (CString)bsRes;
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeString(const CString& strName, CString& strVal)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeStringBase(bsName, strVal);
		}
		inline void UpdateValueStringBase(const _bstr_t& strName, CString& strVal)
		{
			BSTR bsRes = ReadNodeTextBase(strName);
			if (NULL == bsRes)
				return;

			strVal = (CString)bsRes;
			SysFreeString(bsRes);
		}
		inline void UpdateValueString(const CString& strName, CString& strVal)
		{
			_bstr_t bsName = strName;
			return UpdateValueStringBase(bsName, strVal);
		}
		inline CString GetAttributeOrValueBase(const _bstr_t& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			BSTR bsRes = ReadAttributeBase(strAttributeName);
			if (NULL == bsRes)
				return GetValueBase(strAttributeName, strDefaultValue);

			CString strRes = (CString)bsRes;
			SysFreeString(bsRes);
			return strRes;
		}
		inline CString GetAttributeOrValue(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			_bstr_t bsName = strAttributeName;
			return GetAttributeOrValueBase(bsName, strDefaultValue);
		}
		inline void UpdateAttributeOrValueStringBase(const _bstr_t& strName, CString& strVal)
		{
			BSTR bsRes = ReadAttributeBase(strName);
			if (NULL == bsRes)
				return UpdateValueStringBase(strName, strVal);

			strVal = (CString)bsRes;
			SysFreeString(bsRes);
		}
		inline void UpdateAttributeOrValueString(const CString& strName, CString& strVal)
		{
			_bstr_t bsName = strName;
			return UpdateAttributeOrValueStringBase(bsName, strVal);
		}
		//---------------------------------------------------------------------------------
		template <typename T>
		AVSINLINE void LoadArray(const CString& sName, std::vector<T>& arList)
		{
			XmlUtils::CXmlNodes oNodes;
			if (GetNodes(sName, oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oItem;
					oNodes.GetAt(i, oItem);

					T elm;
					arList.push_back(elm);
					arList.back().fromXML(oItem);
				}
			}
		}
		template <typename T>
		AVSINLINE void LoadArray(const CString& sName, const CString& sSubName, std::vector<T>& arList)
		{
			XmlUtils::CXmlNode oNode;
			if (GetNode(sName, oNode))
			{
				XmlUtils::CXmlNodes oNodes;
				if (oNode.GetNodes(sSubName, oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oItem;
						oNodes.GetAt(i, oItem);

						T elm;
						arList.push_back(elm);
						arList.back().fromXML(oItem);
					}
				}
			}
		}

		//----------------------------------------------------------------------------------
		AVSINLINE XmlUtils::CXmlNode ReadNode(const CString& strNodeName)
		{
			CXmlNode oNode;
			GetNode(strNodeName, oNode);
			return oNode;
		}
		AVSINLINE XmlUtils::CXmlNode ReadNodeNoNS(const CString& strNodeName)
		{
			XmlUtils::CXmlNodes oNodes;
			if (GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode node;
					oNodes.GetAt(i, node);

					if (strNodeName == GetNameNoNS(node.GetName()))
						return node;
				}
			}
					
			CXmlNode node;
			return node;
		}
		CString GetXml(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeXml(m_pXmlNode, strDefaultValue);
		}
		CString GetText(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeText(m_pXmlNode, strDefaultValue);
		}
                bool GetTextTyped(_variant_t* pVariant, const CString& strNodeType = _T("bin.base64"))
		{
			if (!IsValid())
                                return false;

			return GetNodeTextTyped(m_pXmlNode, pVariant, strNodeType);
		}
                bool GetNode(const CString& strSubNodeName, CXmlNode& oXmlNode)
		{
			if (!IsValid())
                                return false;

			try
			{
				//CString strXPath;
				//strXPath.Format( _T("*[local-name(.) = '%ls']"), strSubNodeName);

				_bstr_t bstrNode; bstrNode = strSubNodeName;
				//_bstr_t bstrNode; bstrNode = strXPath;

				XML::IXMLDOMNodePtr pNewNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL != pNewNode)
					return oXmlNode.FromXmlNode(pNewNode);
			}
			catch (...)
			{
			}

                        return false;
		}
                bool GetNodes(const CString& strSubNodesName, CXmlNodes& oXmlNodes)
		{
			if (!IsValid())
                                return false;

			try
			{
				_bstr_t bstrNodes; bstrNodes = strSubNodesName;

				XML::IXMLDOMNodeListPtr pNewNodes = m_pXmlNode->selectNodes(bstrNodes);

				if (NULL != pNewNodes)
					return oXmlNodes.FromXmlNodes(pNewNodes);
			}
			catch (...)
			{
			}

                        return false;
		}
                bool GetChilds(CXmlNodes& oXmlNodes)
		{
			if (!IsValid())
                                return false;

			try
			{
				XML::IXMLDOMNodeListPtr pNewNodeList = m_pXmlNode->GetchildNodes();
				if (NULL != pNewNodeList)
					return oXmlNodes.FromXmlNodes(pNewNodeList);
			}
			catch (...)
			{
			}

                        return false;
		}

		CString GetTextExt(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			XML::IXMLDOMElementPtr run_elem = NULL;
			m_pXmlNode->QueryInterface(__uuidof(XML::IXMLDOMElement), (void**)&run_elem);
			run_elem->setAttribute(L"xml:space",(const _variant_t&)L"preserve");

			return GetText(strDefaultValue);

			// old version
			/*
			XML::IXMLDOMNode* pNode = NULL;
			m_pXmlNode->raw_selectSingleNode(g_cpszXML_TextExt.GetBSTR(), &pNode);

			if (NULL != pNode)
			{
				VARIANT var;
				pNode->get_nodeValue(&var);
				
				pNode->Release();
				
				CString strRet = strDefaultValue;
				if (VT_BSTR == var.vt)
				{
					strRet = var.bstrVal;
				}
				
				VariantClear(&var);
				return strRet;
			}
			return strDefaultValue;
			*/
		}

		CString GetTextExt2(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeValueExt(m_pXmlNode, strDefaultValue);
		}

		LONG GetTextLenExt()
		{
			if (!IsValid())
				return 0;

			return (LONG)m_pXmlNode->xml.length();
		}

		CString GetAttributeExt(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeAttribExt(m_pXmlNode, strAttributeName, strDefaultValue);
		}

		CString GetAttributeOrValueExt(const CString& strSubNodeName, const CString& strDefaultValue = _T(""))
		{
			//CString strInvalidValue = _T("x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = GetAttributeExt(strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CXmlNode oNode;
			if (GetNode(strSubNodeName, oNode))
				return oNode.GetTextExt(strDefaultValue);

			return strDefaultValue;
		}

		XML::IXMLDOMNamedNodeMapPtr GetAttributes()
		{
			if (NULL == m_pXmlNode)
				return NULL;
			return m_pXmlNode->attributes;
		}
	};

}


//
namespace XmlUtils
{
	// Remove <?xml .. ?> node
	static CStringW RemoveXMLRootNode (const CStringW& aXML)
	{
		CStringW sRes;
		try
		{
			XML::IXMLDOMDocument2Ptr	pXmlDocument = NULL;

			if (FAILED(pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument60))))
			{
				if (FAILED(pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument))))
				{
					pXmlDocument = NULL;
				}
			}
			if (NULL != pXmlDocument)
			{
				pXmlDocument->setProperty( _bstr_t(_T("SelectionLanguage")), _variant_t(_T("XPath")) );
			}

			_bstr_t bstrXml; bstrXml = aXML;

			if (VARIANT_TRUE == pXmlDocument->loadXML(bstrXml))
			{
				XML::IXMLDOMNodeListPtr ptrNodeList = pXmlDocument->childNodes;				
				for (int i = 0; i < ptrNodeList->length;)
				{
					XML::IXMLDOMNodePtr ptrNode = ptrNodeList->item[i];
					if (NODE_ELEMENT == ptrNode->nodeType)
						break;
					pXmlDocument->removeChild(ptrNode);
				}				
				sRes = pXmlDocument->xml.GetBSTR();
			}
		}
		catch (...)
		{
		}
		return sRes;
	}

	static CStringA ConvertToUTF8 (const CStringW& aUnicodeXML)
	{
		// delete prefix, if exist
		CStringW sXML = RemoveXMLRootNode(aUnicodeXML);

		// Add xml root node
		sXML.Insert(0, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");

		// Convert to UTF-8
		int iSize = WideCharToMultiByte (CP_UTF8, 0, sXML, -1, NULL, 0, NULL, NULL);
		if (iSize <= 0)
		{
			ATLTRACE2 ("ConvertToUTF8() error (size detection): 0x%x\n", GetLastError());
			return CStringA(sXML);	// Conversion to ANSI
		}
		CStringA sOutXML;
		if (0 == WideCharToMultiByte (CP_UTF8, 0, sXML, -1, sOutXML.GetBuffer(iSize + 1), iSize, NULL, NULL))
		{
			ATLTRACE2 ("ConvertToUTF8() error (utf-8 conversion): 0x%x\n", GetLastError());
			return CStringA(sXML);
		}
		sOutXML.ReleaseBuffer();
		return sOutXML;
	}
	// Check XMl Encoding (ANSI or UTF-8) and converting to WideString
	static CStringW ConvertToUnicode (CStringA aAnsiUtf8XML)
	{
		// find xml root element
		CString sRootElement; sRootElement = aAnsiUtf8XML;
		int nXMLLength = sRootElement.GetLength();
		int nStartPos (-1);
		int nEndPos (-1);
		CString sEncoding = _T("ascii");

		try
		{
			if (nXMLLength > 7)	// <?xml?>
			{
				sRootElement.MakeLower();
				nStartPos = sRootElement.Find(_T("<?xml"));
				if (nStartPos >= 0 && nStartPos < nXMLLength - 2)
				{
					nEndPos = sRootElement.Find(_T("?>"), nStartPos + 5);
					if (nEndPos > nStartPos && nEndPos < nXMLLength)
					{
						// Extract <?xml .. ?> node
						CString sXMLNode = sRootElement.Mid (nStartPos, nEndPos - nStartPos + 2);	// strlen("?>") = 2

						// Check encoding
						sXMLNode += _T("<test/>"); // Add extra node

						XML::IXMLDOMDocument2Ptr pXmlDocument = NULL;
						if (FAILED(pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument60))))
						{
							if (FAILED(pXmlDocument.CreateInstance(__uuidof(MSXML::DOMDocument))))
							{
								pXmlDocument = NULL;
							}
						}
						if (NULL != pXmlDocument)
						{
							pXmlDocument->setProperty( _bstr_t(_T("SelectionLanguage")), _variant_t(_T("XPath")) );
						}

						_bstr_t bstrRootXml; bstrRootXml = sXMLNode;

						if (VARIANT_TRUE == pXmlDocument->loadXML(bstrRootXml))
						{
							XML::IXMLDOMNodeListPtr ptrNodeList = pXmlDocument->childNodes;
							for (int i = 0; i < ptrNodeList->length; i++)
							{
								XML::IXMLDOMNodePtr ptrNode = ptrNodeList->item[i];
								CString sName; sName = ptrNode->nodeName.GetBSTR(); sName.MakeLower();
								if (_T("xml") == sName)
								{
									sEncoding = XmlUtils::GetNodeAttrib(ptrNode, _T("encoding"), _T("ascii"));
									break;
								}
							}
						}	// if (VARIANT_TRUE == pXmlDocument->loadXML(bstrXml))
					}	// if (nEndPos > nStartPos && nEndPos < nXMLLength)
				}	// if (nStartPos > 0 && nStartPos < nXMLLength - 2)
			}
		}
		catch (...)
		{
			//MessageBox(NULL, CString (pStr), L"pXmlDocument->loadXML(bstrXml) exception", 0);
			return CStringW (aAnsiUtf8XML);
		}


		sEncoding.MakeLower();

		CStringW sw;
		if (_T("utf-8") == sEncoding)
		{
			int nSize = MultiByteToWideChar(CP_UTF8, 0, aAnsiUtf8XML.GetBuffer(), -1, NULL, 0);
			MultiByteToWideChar(CP_UTF8, 0, aAnsiUtf8XML.GetBuffer(), -1, sw.GetBuffer(nSize + 1), nSize);
			sw.ReleaseBuffer();
		}
		else
		{
			int nSize = MultiByteToWideChar(CP_ACP, 0, aAnsiUtf8XML.GetBuffer(), -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, aAnsiUtf8XML.GetBuffer(), -1, sw.GetBuffer(nSize + 1), nSize);
			sw.ReleaseBuffer();
		}

		return RemoveXMLRootNode(sw);
	}
}

#endif // #if defined(_WIN32) || defined (_WIN64)

#ifdef _USE_XMLLITE_READER_
namespace XmlUtils
{
	class CXmlLiteReader
	{
	public:

		CXmlLiteReader()
		{
			m_pReader = NULL;
			m_pStream = NULL;
		}
		~CXmlLiteReader()
		{
			RELEASEINTERFACE( m_pReader );
			RELEASEINTERFACE( m_pStream );
		}

	public:

		inline void Clear()
		{
			RELEASEINTERFACE( m_pReader );
			RELEASEINTERFACE( m_pStream );
		}

                inline bool IsValid()
		{
			return ( NULL != m_pReader );
		}

                inline bool FromFile(CString& sFilePath)
		{
			Clear();

			if ( FAILED( ::SHCreateStreamOnFileW( sFilePath, STGM_READ, &m_pStream ) ) )
                                return false;

			if ( FAILED( ::CreateXmlReader( __uuidof(IXmlReader), reinterpret_cast<void**>(&m_pReader), 0 ) ) )
			{
				RELEASEINTERFACE( m_pStream );
                                return false;
			}
			m_pReader->SetInput( m_pStream );

                        return true;
		}
                inline bool FromString(CString& sXml)
		{
			Clear();

            if ( FAILED( ::CreateStreamOnHGlobal( NULL, true, &m_pStream ) ) )
                                return false;

			ULONG cbWritten = 0;
			if ( FAILED( m_pStream->Write( sXml.GetBuffer(), sXml.GetLength() * sizeof(TCHAR), &cbWritten ) ) )
			{
				RELEASEINTERFACE( m_pStream );
                                return false;
			}

			LARGE_INTEGER liBeggining = { 0 };
			if ( FAILED( m_pStream->Seek( liBeggining, STREAM_SEEK_SET, NULL ) ) )
			{
				RELEASEINTERFACE( m_pStream );
                                return false;
			}
			
			if ( FAILED( ::CreateXmlReader( __uuidof(IXmlReader), reinterpret_cast<void**>(&m_pReader), 0 ) ) )
			{
				RELEASEINTERFACE( m_pStream );
                                return false;
			}
			if ( FAILED( m_pReader->SetInput( m_pStream )))
			{
				RELEASEINTERFACE( m_pStream );
                                return false;
			}

                        return true;
		}
                inline bool Read(XmlNodeType &oNodeType)
		{
			if ( !IsValid() )
                                return false;

			if ( FAILED( m_pReader->Read( &oNodeType ) ) )
                                return false;

                        return true;
		}
                inline bool ReadNextNode()
		{
			if ( !IsValid() )
                                return false;

			XmlNodeType oNodeType = XmlNodeType_None;
			
			while ( S_OK == m_pReader->Read( &oNodeType ) && XmlNodeType_Element != oNodeType );

			if ( XmlNodeType_Element == oNodeType )
                                return true;

                        return false;
		}
                inline bool ReadNextSiblingNode(int nDepth)
		{
			// Перед использованием этой функции надо проверить,
			// пустая ли родительская нода. 
			if ( !IsValid() )
                                return false;

			XmlNodeType eNodeType = XmlNodeType_None;
			int nCurDepth = -1;
			
			while ( S_OK == m_pReader->Read( &eNodeType ) && (nCurDepth = GetDepth()) > nDepth )
			{
				if ( XmlNodeType_Element == eNodeType && nCurDepth == nDepth + 1 )
                                        return true;
				else if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth + 1 )
                                        return false;
			}

                        return false;
		}
                inline bool ReadTillEnd(int nDepth = -2)
		{
			if ( !IsValid() )
                                return false;

			if ( -2 == nDepth )
				nDepth = GetDepth();
			else if ( nDepth == GetDepth() && m_pReader->IsEmptyElement() )
                                return true;

			XmlNodeType eNodeType = XmlNodeType_None;

			int nCurDepth = -1;
			// У закрывающего тэга глубина на 1 больше, чем у открывающего
                        while( true )
			{
				if ( S_OK != m_pReader->Read( &eNodeType ) )
					break;

				nCurDepth = GetDepth();
				if ( nCurDepth <= nDepth )
					break;
				
				if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth + 1 )
					break;
			}

                        return true;
		}
		inline const wchar_t* GetName()
		{
			if ( !IsValid() )
				return NULL;

			const WCHAR* pwszQName;
			if ( FAILED( m_pReader->GetQualifiedName( &pwszQName, NULL ) ) )
				return NULL;

			return pwszQName;
		}
		inline int     GetDepth()
		{
			if ( !IsValid() )
				return -1;
			
			UINT unDepth = 0;
			if ( S_OK != m_pReader->GetDepth( &unDepth ) )
				return -1;

			return unDepth;
		}
                inline bool IsEmptyNode()
		{
			if ( !IsValid() )
                                return false;

			return m_pReader->IsEmptyElement();
		}

		inline const WCHAR *GetText()
		{
			if ( !IsValid() )
				return NULL;

			const WCHAR* pwszValue;
			if ( FAILED( m_pReader->GetValue( &pwszValue, NULL ) ) )
				return NULL;

			return pwszValue;
		}
		inline CString GetText2()
		{
			if ( !IsValid() )
				return _T("");

			// TO DO: Ускорить (убрать CString)
			CString sResult;

			if ( m_pReader->IsEmptyElement() )
				return sResult;

			int nDepth = GetDepth();
			XmlNodeType eNodeType = XmlNodeType_EndElement;
			while( Read( eNodeType ) && GetDepth() > nDepth && XmlNodeType_EndElement != eNodeType )
			{
				if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace )
					sResult += GetText();
			}

			return sResult;
		}
		inline CString GetOuterXml()
		{
			return GetXml(false);
		}
		inline CString GetInnerXml()
		{
			return GetXml(true);
		}
		inline int  GetAttributesCount()
		{
			if ( !IsValid() )
				return -1;

			UINT unAttrsCount = 0;
			if ( FAILED( m_pReader->GetAttributeCount( &unAttrsCount ) ) )
				return -1;

			return (int)unAttrsCount;
		}
                inline bool MoveToFirstAttribute()
		{
			if ( !IsValid() )
                                return false;

			if ( S_OK != m_pReader->MoveToFirstAttribute() )
                                return false;

                        return true;
		}
                inline bool MoveToNextAttribute()
		{
			if ( !IsValid() )
                                return false;

			if ( S_OK != m_pReader->MoveToNextAttribute() )
                                return false;

                        return true;
		}

                inline bool MoveToElement()
		{
			if ( !IsValid() )
                                return false;

			if ( FAILED( m_pReader->MoveToElement() ) )
                                return false;

                        return true;
		}
	private:
		inline CString GetXml(bool bInner)
		{
			if ( !IsValid() )
				return _T("");

			CStringWriter oResult;
			if(false == bInner)
				WriteElement(oResult);

			int nDepth = GetDepth();
			if ( false == m_pReader->IsEmptyElement() )
			{
				XmlNodeType eNodeType = XmlNodeType_None;

				int nCurDepth = -1;
				// У закрывающего тэга глубина на 1 больше, чем у открывающего
                                while( true )
				{
					if ( S_OK != m_pReader->Read( &eNodeType ) )
						break;

					nCurDepth = GetDepth();
					if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace )
						oResult.WriteEncodeXmlString(GetText());
					else if(eNodeType == XmlNodeType_Element)
						WriteElement(oResult);
					else if(eNodeType == XmlNodeType_EndElement)
					{
						if(false == bInner || nCurDepth != nDepth + 1)
						{
							oResult.AddChar2Safe(TCHAR('<'), TCHAR('/'));
							oResult.WriteEncodeXmlString(GetName());
							oResult.AddCharSafe(TCHAR('>'));
						}
					}

					nCurDepth = GetDepth();
					if ( nCurDepth <= nDepth )
						break;

					if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth + 1 )
						break;
				}
			}

			return oResult.GetData();
		}
		void WriteElement(XmlUtils::CStringWriter& oResult)
		{
			/*
			oResult.WriteString(CString(_T("<")));
			oResult.WriteString(EncodeXmlString(GetName()));
			if(GetAttributesCount() > 0)
			{
				MoveToFirstAttribute();
				CString sName = GetName();
				while( !sName.IsEmpty() )
				{
					oResult.WriteString(CString(_T(" ")));
					oResult.WriteString(EncodeXmlString(GetName()));
					oResult.WriteString(CString(_T("=\"")));
					oResult.WriteString(EncodeXmlString(GetText()));
					oResult.WriteString(CString(_T("\"")));

					if ( !MoveToNextAttribute() )
						break;
					sName = GetName();
				}
				MoveToElement();
			}
			if(m_pReader->IsEmptyElement())
				oResult.WriteString(CString(_T("/>")));
			else
				oResult.WriteString(CString(_T(">")));
			*/

			oResult.AddCharSafe((TCHAR)'<');
			oResult.WriteEncodeXmlString(GetName());
			if(GetAttributesCount() > 0)
			{
				MoveToFirstAttribute();
				CString sName = GetName();
				while( !sName.IsEmpty() )
				{
					oResult.AddCharSafe(TCHAR(' '));
					oResult.WriteEncodeXmlString(GetName());
					oResult.AddChar2Safe(TCHAR('='), TCHAR('\"'));
					oResult.WriteEncodeXmlString(GetText());
					oResult.AddCharSafe(TCHAR('\"'));

					if ( !MoveToNextAttribute() )
						break;
					sName = GetName();
				}
				MoveToElement();
			}
			if (m_pReader->IsEmptyElement())
				oResult.AddChar2Safe(TCHAR('/'), TCHAR('>'));
			else
				oResult.AddCharSafe(TCHAR('>'));
		}
	private:

		IStream                     *m_pStream;
		IXmlReader                  *m_pReader;

	};


	enum EXmlLiteElement
	{
		xmlliteUnknown    = 0,
		xmlliteNode       = 1,
		xmlliteText       = 2,
		xmlliteWhiteSpace = 3,
	};

	struct TXmlLiteAttribute
	{
		TXmlLiteAttribute()
		{
			wsName  = NULL;
			wsValue = NULL;
		}

		const TXmlLiteAttribute& operator=(const TXmlLiteAttribute& oOther)
		{
			// Копируем ссылки, а не строки
			wsName  = oOther.wsName;
			wsValue = oOther.wsValue;

			return (*this);
		}

		wchar_t* wsName;
		wchar_t* wsValue;
	};

	// Базовый класс для элементов XML
	class CXmlLiteWrapper;
	class IXmlLiteElement
	{
	public:

		virtual ~IXmlLiteElement() {}
		virtual EXmlLiteElement get_Type() const = 0;
	};
	class CXmlLiteText : public IXmlLiteElement
	{
	public:

		CXmlLiteText()
		{
			m_wsValue = NULL;
		}

		CXmlLiteText(const wchar_t* wsValue)
		{
			if ( NULL != wsValue )
				m_wsValue = wcsdup( wsValue );
		}

		virtual ~CXmlLiteText()
		{
			if ( m_wsValue )
				free( m_wsValue );
		}

		virtual EXmlLiteElement get_Type() const
		{
			return xmlliteText;
		}

	public:

		wchar_t* get_Text() const
		{
			return m_wsValue;
		}

	private:

		wchar_t* m_wsValue; // Text

	};

	class CXmlLiteWhiteSpace : public IXmlLiteElement
	{
	public:

		CXmlLiteWhiteSpace()
		{
			m_wsValue = NULL;
		}

		CXmlLiteWhiteSpace(const wchar_t* wsValue)
		{
			if ( NULL != wsValue )
				m_wsValue = wcsdup( wsValue );
		}

		virtual ~CXmlLiteWhiteSpace()
		{
			if ( m_wsValue )
				free( m_wsValue );
		}

		virtual EXmlLiteElement get_Type() const
		{
			return xmlliteWhiteSpace;
		}

	public:

		wchar_t* get_Text() const
		{
			return m_wsValue;
		}

	private:

		wchar_t* m_wsValue; // White Space

	};

	class CXmlLiteNode : public IXmlLiteElement
	{
	public:

		CXmlLiteNode()
		{
			m_nIterator     = 0;
			m_nAttrIterator = 0;
			m_wsName        = NULL;
		}

		CXmlLiteNode(const wchar_t* wsName)
		{
			m_nIterator     = 0;
			m_nAttrIterator = 0;
			if ( NULL != wsName )
				m_wsName = wcsdup( wsName );
		}

		virtual ~CXmlLiteNode()
		{
			if ( NULL != m_wsName )
				free( m_wsName );

			for ( int nIndex = 0; nIndex < m_arrAttributes.GetSize(); nIndex++ )
			{
				if ( m_arrAttributes[nIndex].wsName )
					free( m_arrAttributes[nIndex].wsName );

				if ( m_arrAttributes[nIndex].wsValue )
					free( m_arrAttributes[nIndex].wsValue );
			}

			for ( int nIndex = 0; nIndex < m_arrChilds.GetSize(); nIndex++ )
			{
				if ( m_arrChilds[nIndex] )
					delete m_arrChilds[nIndex];
			}
		}

		virtual EXmlLiteElement get_Type() const
		{
			return xmlliteNode;
		}

	public:

		wchar_t*          get_Name() const
		{
			return m_wsName;
		}
		void              move_ToStartNode()
		{
			m_nIterator = 0;
		}
		void              move_ToFirstAttribute()
		{
			m_nAttrIterator = 0;
		}

		IXmlLiteElement*  get_NextElement()
		{
			if ( m_nIterator >= m_arrChilds.GetSize() )
				return NULL;

			return m_arrChilds[m_nIterator++];
		}

		IXmlLiteElement*  get_At(int nIndex)
		{
			if ( nIndex >= 0 && nIndex < m_arrChilds.GetSize() )
				return m_arrChilds[nIndex];

			return NULL;
		}

		TXmlLiteAttribute get_NextAttribute()
		{
			TXmlLiteAttribute oEmpty;
			if ( m_nAttrIterator >= m_arrAttributes.GetSize() )
				return oEmpty;

			return m_arrAttributes[m_nAttrIterator++];
		}

		wchar_t*          get_Attribute(int nIndex)
		{
			if ( nIndex >= 0 && nIndex < m_arrAttributes.GetSize() )
				return m_arrAttributes[nIndex].wsValue;

			return NULL;
		}		
		wchar_t*          get_Attribute(wchar_t* wsAttributeName)
		{
			for ( int nIndex = 0; nIndex < m_arrAttributes.GetSize(); nIndex++ )
			{
				if ( 0 == CWCharWrapper::WStrCmp( wsAttributeName, m_arrAttributes[nIndex].wsName ) )
				{
					return m_arrAttributes[nIndex].wsValue;
				}
			}

			return NULL;
		}
		TXmlLiteAttribute get_Attribute2(int nIndex)
		{
			TXmlLiteAttribute oEmpty;
			if ( nIndex >= 0 && nIndex < m_arrAttributes.GetSize() )
				return m_arrAttributes[nIndex];

			return oEmpty;
		}
		int               get_AttributesCount() const
		{
			return m_arrAttributes.GetSize();
		}
		CXmlLiteNode*     get_NextNode()
		{
			IXmlLiteElement* pResult = NULL;
			while ( NULL != ( pResult = get_NextElement() ) )
			{
				if ( xmlliteNode == pResult->get_Type() )
					return ((CXmlLiteNode*)pResult);
			}

			return NULL;
		}

	protected:

		void              add_Attribute(const wchar_t* wsName, const wchar_t* wsValue)
		{
			TXmlLiteAttribute oAttribute;

			if ( NULL != wsName )
				oAttribute.wsName = wcsdup( wsName );

			if ( NULL != wsValue )
				oAttribute.wsValue = wcsdup( wsValue );

			m_arrAttributes.Add( oAttribute );
		}

		IXmlLiteElement*  add_Child(const wchar_t* wsValue, EXmlLiteElement eType)
		{
			IXmlLiteElement* pChild = NULL;

			switch(eType)
			{
			case xmlliteNode:
				pChild = new CXmlLiteNode( wsValue );
				break;
			case xmlliteText:
				pChild = new CXmlLiteText( wsValue );
				break;
			case xmlliteWhiteSpace:
				pChild = new CXmlLiteWhiteSpace( wsValue );
				break;
			}

			if ( NULL != pChild )
				m_arrChilds.Add( pChild );

			return pChild;
		}

	private:

		int                             m_nIterator;     //
		int                             m_nAttrIterator; //

		wchar_t*                        m_wsName;        // Node name
		CSimpleArray<TXmlLiteAttribute> m_arrAttributes; // Attributes
		CSimpleArray<IXmlLiteElement*>  m_arrChilds;     // Childs

		friend class CXmlLiteWrapper;
	};

	class CXmlLiteWrapper : public CXmlLiteNode
	{
	public:

		virtual ~CXmlLiteWrapper() {}			

                inline bool FromFile(CString& sFilePath)
		{
			CXmlLiteReader oReader;
			if ( !oReader.FromFile( sFilePath ) )
                                return false;

			Parse( this, oReader );

                        return true;
		}
                inline bool FromString(CString& sXml)
		{
			CXmlLiteReader oReader;
			if ( !oReader.FromString( sXml ) )
                                return false;

			Parse( this, oReader );

                        return true;
		}

	private:

		void Parse(CXmlLiteNode* pNode, CXmlLiteReader& oReader)
		{
			ParseAttributes( pNode, oReader );

			int nCurDepth = oReader.GetDepth();
			if ( oReader.IsEmptyNode() )
				return;

			XmlNodeType eNodeType;
			while ( oReader.Read( eNodeType ) && ( oReader.GetDepth() > nCurDepth + 1 || XmlNodeType_EndElement != eNodeType ) && XmlNodeType_None != eNodeType )
			{
				switch( eNodeType )
				{
				case XmlNodeType_Text:
					pNode->add_Child( oReader.GetText(), xmlliteText );
					break;
				case XmlNodeType_Whitespace:
					pNode->add_Child( oReader.GetText(), xmlliteWhiteSpace );
					break;
				case XmlNodeType_Element:
					{
						CXmlLiteNode* pNewNode = (CXmlLiteNode*)pNode->add_Child( oReader.GetName(), xmlliteNode );
						if ( NULL != pNewNode )
							Parse( pNewNode, oReader );
						break;
					}
				}
			}
		}

		void ParseAttributes(CXmlLiteNode* pNode, CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			const wchar_t* wsName = oReader.GetName();
			while( NULL != wsName )
			{
				const wchar_t* wsValue = oReader.GetText();
				pNode->add_Attribute( wsName, wsValue );

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}

			oReader.MoveToElement();
		}
	};
}
#endif

#endif
