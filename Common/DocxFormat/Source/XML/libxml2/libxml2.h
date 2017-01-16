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

#include "XML/include/libxml/xmlreader.h"

#include <vector>
#include <map>
#include <string>

#include "../../../../../DesktopEditor/common/File.h"
#include "../Utils.h"

namespace XmlUtils
{
    typedef enum XmlNodeType
	{	
		XmlNodeType_None					= 0,
		XmlNodeType_Element					= 1,
		XmlNodeType_Attribute				= 2,
		XmlNodeType_Text					= 3,
		XmlNodeType_CDATA					= 4,
		XmlNodeType_ENTITY_REFERENCE		= 5,
		XmlNodeType_ENTITY					= 6,
		XmlNodeType_ProcessingInstruction	= 7,
		XmlNodeType_Comment					= 8,
		XmlNodeType_Document				= 9,
		XmlNodeType_DocumentType			= 10,
		XmlNodeType_DOCUMENT_FRAGMENT		= 11,
		XmlNodeType_NOTATION				= 12,
		XmlNodeType_Whitespace				= 13,
		XmlNodeType_SIGNIFICANT_WHITESPACE	= 14,
		XmlNodeType_EndElement				= 15,
		XmlNodeType_TYPE_END_ENTITY			= 16,
		XmlNodeType_XmlDeclaration			= 17,
		_XmlNodeType_Last					= 17
    } XmlNodeType;

    class CXmlLiteReader
	{
	protected:
		xmlTextReaderPtr	reader;
		
		BYTE*				m_pStream;
		LONG				m_lStreamLen;
		std::wstring		m_sTemp;
		std::string			m_sTempA;
	public:

        CXmlLiteReader()
		{			
			reader = NULL;
			m_pStream = NULL;
		}
        virtual ~CXmlLiteReader()
		{
			Clear();
		}

		xmlTextReaderPtr getNativeReader() { return reader; }

	public:

		inline void Clear()
		{
			xmlFreeTextReader(reader);
			reader = NULL;
			if (NULL != m_pStream)
				delete []m_pStream;
			m_pStream = NULL;
			m_lStreamLen = 0;
		}

        inline bool IsValid()
		{
			return ( NULL != reader );
		}

        inline bool FromFile(const wchar_t* sFilePath)
		{
			return FromFile(std::wstring(sFilePath));
		}
        inline bool FromFile(const std::wstring& sFilePath)
		{
			Clear();

			NSFile::CFileBinary oFile;
			oFile.OpenFile(sFilePath);
			m_lStreamLen = (LONG)oFile.GetFileSize();
			m_pStream = new BYTE[m_lStreamLen];
			DWORD dwRead = 0;
			oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
			oFile.CloseFile();

			reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

            return true;
		}
        inline bool FromString(const wchar_t* sXml)
		{
			return FromString(std::wstring(sXml));
		}
        inline bool FromString(const std::wstring& sXml)
		{
			Clear();

			NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.c_str(), (long)sXml.length(), m_pStream, m_lStreamLen, false);
			reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

            return true;
		}
        inline bool FromStringA(const std::string& sXml)
		{
			Clear();

			reader = xmlReaderForMemory((char*)sXml.c_str(), (int)sXml.length(), NULL, NULL, 0);

            return true;
		}
        inline bool Read(XmlNodeType &oNodeType)
		{
			if ( !IsValid() )
                return false;

			if ( 1 != xmlTextReaderRead(reader) )
                return false;

			int nTempType = xmlTextReaderNodeType(reader);
			if(-1 == nTempType)
                return false;

            oNodeType = (XmlNodeType)nTempType;

            return true;
		}
        inline bool ReadNextNode()
		{
			if ( !IsValid() )
                return false;

            XmlNodeType oNodeType = XmlNodeType_None;
			
			while ( XmlNodeType_Element != oNodeType )
			{
				if (1 != xmlTextReaderRead(reader))
					break;

				int nTempType = xmlTextReaderNodeType(reader);
				if(-1 == nTempType)
					break;

                oNodeType = (XmlNodeType)nTempType;
			}

            if ( XmlNodeType_Element == oNodeType )
                return true;

            return false;
		}
        inline bool ReadNextSiblingNode(int nDepth)
		{
			// ����� �������������� ���� ������� ���� ���������,
			// ������ �� ������������ ����. 
			if ( !IsValid() )
                return false;

            XmlNodeType eNodeType = XmlNodeType_None;
			int nCurDepth = -1;
			
			while ( 1 == xmlTextReaderRead(reader) )
			{
				int nTempType = xmlTextReaderNodeType(reader);
				int nTempDepth = xmlTextReaderDepth(reader);
				if(-1 == nTempType || -1 == nTempDepth)
                    return false;
                eNodeType = (XmlNodeType)nTempType;
				nCurDepth = nTempDepth;

				// � ������������ ���� ������� ����� �� ��� � ��������������
				if (nCurDepth < nDepth)
					break;

                if ( XmlNodeType_Element == eNodeType && nCurDepth == nDepth + 1 )
                    return true;
                else if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth )
                    return false;
			}

            return false;
		}
         inline bool ReadNextSiblingNode2(int nDepth)
		{
			if ( !IsValid() )
                return false;

            XmlNodeType eNodeType = XmlNodeType_None;
			int nCurDepth = -1;
			
			while ( 1 == xmlTextReaderRead(reader) )
			{
				int nTempType = xmlTextReaderNodeType(reader);
				int nTempDepth = xmlTextReaderDepth(reader);
				
				if(-1 == nTempType || -1 == nTempDepth)
                    return false;
				
                eNodeType = (XmlNodeType)nTempType;
				nCurDepth = nTempDepth;

				if (nCurDepth < nDepth)
					break;

				if ((	XmlNodeType_Element		== eNodeType  && nCurDepth == nDepth + 1 )	
					|| ((XmlNodeType_Text		== eNodeType  || 
						 XmlNodeType_Whitespace	== eNodeType) && nCurDepth == nDepth + 1) )
                    return true;
				else if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth )
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
			else if ( nDepth == GetDepth() && 0 != xmlTextReaderIsEmptyElement(reader) )
                return true;

            XmlNodeType eNodeType = XmlNodeType_None;

			int nCurDepth = -1;
			// � ������������ ���� ������� ����� �� ��� � ��������������
            while( true )
			{
				if ( 1 != xmlTextReaderRead(reader) )
					break;

				int nTempType = xmlTextReaderNodeType(reader);
				if(-1 == nTempType)
                    return false;
                eNodeType = (XmlNodeType)nTempType;
				nCurDepth = GetDepth();

				if ( nCurDepth < nDepth )
					break;
				
				if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth )
					break;
			}

            return true;
		}
		//inline const wchar_t* GetName()
		//{
		//	if ( !IsValid() )
		//		return L"";

		//	xmlChar* pName = xmlTextReaderName(reader);
		//	if (NULL == pName)
		//		return L"";

		//	m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
		//	free(pName);
		//	return m_sTemp.c_str();
		//}
		inline const std::wstring GetName()
		{
			if ( !IsValid() )
				return L"";

			xmlChar* pName = xmlTextReaderName(reader);
			if (NULL == pName)
				return L"";

			m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
			free(pName);
			return m_sTemp;
		}
		inline const char* GetNameA()
		{
			if ( !IsValid() )
				return "";

			xmlChar* pName = xmlTextReaderName(reader);
			if (NULL == pName)
				return "";

			//return std::string((const char*)pName);
			m_sTempA = (const char*)pName;
			free(pName);
			return m_sTempA.c_str();
		}
		inline int GetDepth()
		{
			if ( !IsValid() )
				return -1;

			int nTempDepth = xmlTextReaderDepth(reader);
			if(-1 == nTempDepth)
				return -1;
			return nTempDepth;
		}
        inline bool IsEmptyNode()
		{
			if ( !IsValid() )
                return false;

            return 0 != xmlTextReaderIsEmptyElement(reader) ? true : false;
		}

		inline const wchar_t* GetText()
		{
			if ( !IsValid() )
				return NULL;

			xmlChar* pValue = xmlTextReaderValue(reader);
			if (NULL == pValue)
				return L"";
			
			m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen((const char*)pValue));
			free(pValue);
			return m_sTemp.c_str();
		}
		inline const char* GetTextA()
		{
			if ( !IsValid() )
				return NULL;

			xmlChar* pValue = xmlTextReaderValue(reader);
			if (NULL == pValue)
				return "";

			m_sTempA = (const char*)pValue;
			free(pValue);
			return m_sTempA.c_str();
		}

        inline std::wstring GetText2()
		{
			if ( !IsValid() )
				return _T("");

            std::wstring sResult;

			if ( 0 != xmlTextReaderIsEmptyElement(reader) )
				return sResult;

			int nDepth = GetDepth();
            XmlNodeType eNodeType = XmlNodeType_EndElement;
			while ( Read( eNodeType ) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType )
			{
				if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
					sResult += GetText();
			}

			return sResult;
		}
		inline std::wstring GetText3()
		{
			if ( !IsValid() )
				return _T("");

			std::wstring sResult;

			if ( 0 != xmlTextReaderIsEmptyElement(reader) )
				return sResult;

			int nDepth = GetDepth();
            XmlNodeType eNodeType = XmlNodeType_EndElement;
			while ( Read( eNodeType ) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType )
			{
				if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
					sResult += GetText();
			}

			return sResult;
		}
        inline std::wstring GetOuterXml()
		{
			return GetXml(false);
		}
        inline std::wstring GetInnerXml()
		{
			return GetXml(true);
		}
		inline int GetAttributesCount()
		{
			if ( !IsValid() )
				return -1;

			return xmlTextReaderAttributeCount(reader);
		}
        inline bool MoveToFirstAttribute()
		{
			if ( !IsValid() )
                return false;

            return 1 == xmlTextReaderMoveToFirstAttribute(reader) ? true: false;
		}
        inline bool MoveToNextAttribute()
		{
			if ( !IsValid() )
                return false;

            return 1 == xmlTextReaderMoveToNextAttribute(reader) ? true: false;
		}

        inline bool MoveToElement()
		{
			if ( !IsValid() )
                return false;

            return 1 == xmlTextReaderMoveToElement(reader) ? true: false;
		}
	private:
        inline std::wstring GetXml(bool bInner)
		{
			if ( !IsValid() )
				return L"";

            CStringWriter oResult;
			if(false == bInner)
				WriteElement(oResult);

			int nDepth = GetDepth();
			if ( 0 == xmlTextReaderIsEmptyElement(reader) )
			{
                XmlNodeType eNodeType = XmlNodeType_None;

				int nCurDepth = -1;
				// � ������������ ���� ������� ����� �� ��� � ��������������
                while( true )
				{
					if ( 1 != xmlTextReaderRead(reader) )
						break;

					int nTempType = xmlTextReaderNodeType(reader);
					if(-1 == nTempType)
						break;
					eNodeType = (XmlNodeType)nTempType;

					nCurDepth = GetDepth();
					if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
						oResult.WriteEncodeXmlString(GetText());
					else if(eNodeType == XmlNodeType_Element)
						WriteElement(oResult);
					else if(eNodeType == XmlNodeType_EndElement)
					{
						if(false == bInner || nCurDepth != nDepth)
						{
							oResult.AddChar2Safe(TCHAR('<'), TCHAR('/'));
							oResult.WriteEncodeXmlString(GetName().c_str());
							oResult.AddCharSafe(TCHAR('>'));
						}
					}

					nCurDepth = GetDepth();
					if ( nCurDepth < nDepth )
						break;

					if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth )
						break;
				}
			}

			return oResult.GetData();
		}
        void WriteElement(CStringWriter& oResult)
		{
			oResult.AddCharSafe((TCHAR)'<');
			oResult.WriteEncodeXmlString(GetName().c_str());
			if(GetAttributesCount() > 0)
			{
				MoveToFirstAttribute();
				std::wstring sName = GetName();
				while( !sName.empty() )
				{
					oResult.AddCharSafe(TCHAR(' '));
					oResult.WriteEncodeXmlString(GetName().c_str());
					oResult.AddChar2Safe(TCHAR('='), TCHAR('\"'));
					oResult.WriteEncodeXmlString(GetText());
					oResult.AddCharSafe(TCHAR('\"'));

					if ( !MoveToNextAttribute() )
						break;
					sName = GetName();
				}
				MoveToElement();
			}
			if (IsEmptyNode())
				oResult.AddChar2Safe(TCHAR('/'), TCHAR('>'));
			else
				oResult.AddCharSafe(TCHAR('>'));
		}
	};

	class IXmlDOMDocument
	{
	private:
		ULONG m_lRef;
	
	public:
		IXmlDOMDocument()
		{
			m_lRef = 1;
		}
		virtual ~IXmlDOMDocument()
		{
		}

		virtual ULONG AddRef()
		{
			++m_lRef;
			return m_lRef;
		}
		virtual ULONG Release()
		{
			ULONG lReturn = --m_lRef;
			if (0 == m_lRef)
				delete this;
			return lReturn;
		}		
	};

    class CXmlNodeBase : public IXmlDOMDocument
	{
	public:
		IXmlDOMDocument* m_pDocument;

        std::map<std::string, std::string> m_attributes;
        std::vector<CXmlNodeBase*> m_nodes;
        std::wstring m_sText;
        std::wstring m_sName;

	public:
        CXmlNodeBase() : IXmlDOMDocument()
		{
			m_pDocument = NULL;
		}
        ~CXmlNodeBase()
		{
			if (NULL != m_pDocument)
				m_pDocument->Release();

			int nCount = (int)m_nodes.size();
			for (int i = 0; i < nCount; ++i)
			{
				CXmlNodeBase* pNode = m_nodes[i];
				if (NULL != pNode)
					pNode->Release();
			}
		}

		void AddRefDoc()
		{
			if (NULL != m_pDocument)
				m_pDocument->AddRef();
		}
		void ReleaseDoc()
		{
			if (NULL != m_pDocument)
				m_pDocument->Release();
		}

        std::wstring GetXml();
        void GetXml(CStringWriter& oWriter);
	};

    //class CXmlNode;
    //class CXmlNodes
    //{
    //private:
    //    std::vector<CXmlNode> m_nodes;

    //public:
    //    CXmlNodes();
    //    bool IsValid();
    //    int GetCount();
    //    bool GetAt(int nIndex, CXmlNode& oXmlNode);

    //    friend class CXmlNode;
    //};


	//class CXmlNode
	//{
	//private:
	//	CXmlNodeBase* m_pBase;

	//public:
	//	CXmlNode()
	//	{
	//		m_pBase = NULL;
	//	}
	//	~CXmlNode()
	//	{
	//		Clear();
	//	}
	//	CXmlNode(const CXmlNode& oSrc);

	//public:
	//	bool FromXmlFile(const wchar_t* sFile, bool bRemoveRootNode = false)
	//	{
	//		return FromXmlFile(std::wstring(sFile), bRemoveRootNode);
	//	}
	//	bool FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode = false);
 //       bool FromXmlFile2(const std::wstring& strXmlFilePath);
	//	bool FromXmlStringA(const std::string& sString);
	//	bool FromXmlString(const wchar_t* sString)
	//	{
	//		return FromXmlString(std::wstring(sString));
	//	}
	//	bool FromXmlString(const std::wstring& sString);

 //       bool IsValid() const;
	//	void Clear();
    //	std::wstring GetName();
    //	std::wstring GetText();
    //	bool GetTextIfExist(std::wstring& sOutput);
    //	std::wstring GetTextExt(const std::wstring& strDefaultValue = _T(""));
    //	std::wstring GetXml(const std::wstring& strDefaultValue = _T(""));

    //	std::wstring ReadAttributeBase(const wchar_t* bstrName);
	//	template<typename T>
	//	void ReadAttributeBase(const wchar_t* bsName, T& value)
	//	{
    //		std::wstring sAttr;
    //		if(GetAttributeIfExist(std::wstring(bsName), sAttr))
	//			value = sAttr.GetString();
	//	}
    //	std::wstring ReadAttribute(const std::wstring& strAttibuteName);
	//	template<typename T>
	//	void ReadAllAttributes(T& strNames, T& strValues)
	//	{
	//		if (!IsValid())
	//			return;

    //		std::map<std::string, std::string>::iterator p;
	//		for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
	//		{
 //               strNames.push_back(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.GetString(), p->first.GetLength(), true));
 //               strValues.push_back(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.GetString(), p->second.GetLength(), true));
	//		}
	//	}
	//	
    //	std::string GetAttributeA(const std::string& sName, const std::string& _default = "");
    //	std::string GetAttributeA(const std::wstring& sName, const std::string& _default = "");

    //	std::wstring GetAttribute(const std::string& sName, const std::wstring& _default = L"");
    //	std::wstring GetAttribute(const std::wstring& sName, const std::wstring& _default = L"");
    //	std::wstring GetAttribute(const wchar_t* sName, const std::wstring& _default = L"");
    //	std::wstring GetAttributeBase(const wchar_t* strAttributeName, const std::wstring& strDefaultValue = _T(""));
    //	bool GetAttributeIfExist(const std::wstring& sName, std::wstring& sOutput);

    //	int GetAttributeInt(const std::string& sName, const int& _default = 0);
    //	int GetAttributeInt(const std::wstring& sName, const int& _default = 0);
    //	int ReadAttributeInt(const std::wstring& str, const int& nDef = 0);
    //	double ReadAttributeDouble(const std::wstring& str, const double& nDef = 0);

    //	double GetAttributeDouble(const std::string& sName, const double& _default = 0);
    //	double GetAttributeDouble(const std::wstring& sName, const double& _default = 0);
	//	

    //	std::wstring ReadNodeTextBase(const wchar_t* bsName);
	//	template <typename T>
	//	void ReadNodeValueBase(const wchar_t* bsName, T& value)
	//	{
	//		value = ReadNodeTextBase(bsName);
	//	}
    //	std::wstring ReadNodeText(const std::wstring& strName);
    //	std::wstring ReadValueString(const std::wstring& sName, const std::wstring& nDef = _T(""));
    //	int ReadValueInt(const std::wstring& str, const int& nDef = 0);

    //	std::wstring GetAttributeOrValue(const std::wstring& strAttributeName, const std::wstring& strDefaultValue = _T(""));

 //       /*
 //       void LoadArray(const std::wstring& sName, std::vector<T>& arList)
 //       {
 //           if (GetNodes(sName, oNodes))
 //       }
 //       */

	//	template <typename T>
 //       void LoadArray(const std::wstring& sName, std::vector<T>& arList)
	//	{
	//		CXmlNodes oNodes;
	//		if (GetNodes(sName, oNodes))
 //           {
	//			int nCount = oNodes.GetCount();
	//			for (int i = 0; i < nCount; ++i)
	//			{
	//				CXmlNode oItem;
	//				oNodes.GetAt(i, oItem);

 //                   //arList.Add ();
 //                   //arList[i].fromXML(oItem);
 //                   arList.push_back(T());
 //                   arList[i].fromXML(oItem);
	//			}
	//		}
	//	}
	//	template <typename T>
 //       void LoadArray(const std::wstring& sName, const std::wstring& sSubName, std::vector<T>& arList)
	//	{
	//		CXmlNode oNode;
	//		if (GetNode(sName, oNode))
	//			oNode.LoadArray(sSubName, arList);
	//	}
	//	CXmlNode ReadNode(const TCHAR* strNodeName);
    //	CXmlNode ReadNode(const std::wstring& strNodeName);
    //	CXmlNode ReadNodeNoNS(const std::wstring& strNodeName);

    //	CXmlNode GetNode(const std::wstring& sName);
    //	CXmlNodes GetNodes(const std::wstring& sName);
 //       bool GetChilds(CXmlNodes& oXmlNodes);

    //	bool GetNode(const std::wstring& sName, CXmlNode& oNode);
    //	bool GetNodes(const std::wstring& sName, CXmlNodes& oNodes);

	//	CXmlNode& operator=(const CXmlNode& oSrc);
	//private:
	//	void SetBase(CXmlNodeBase* pBase);
    //	std::wstring GetNamespace(const std::wstring& strNodeName);
    //	//std::wstring GetNameNoNS(const std::wstring& strNodeName);
	//	std::wstring GetNameNoNS(const std::wstring& strNodeName);
	//public:
    //	std::wstring private_GetXml();
    //	std::wstring private_GetXml(const std::wstring& strDefaultValue = _T(""));
    //	std::wstring private_GetXmlFast();
    //	std::wstring private_GetXmlFast(const std::wstring& strDefaultValue);
	//};


}

#if 0
#define LIBXML_HTML_ENABLED

#include "XML/include/libxml/parser.h"
#include "XML/include/libxml/HTMLParser.h"

namespace XmlUtils
{
	class CHtmlReader
	{
		htmlDocPtr			reader;
		
		BYTE*				m_pStream;
		LONG				m_lStreamLen;

	public:

		CHtmlReader()
		{			
			reader = NULL;
			m_pStream = NULL;
		}
		~CHtmlReader()
		{
			if (NULL != m_pStream)
				delete []m_pStream;
		}

		inline void Clear()
		{
		}

        inline bool IsValid()
		{
			return ( NULL != reader );
		}

        inline bool FromFile(std::wstring& sFilePath)
		{
			Clear();

			NSFile::CFile oFile;
			oFile.OpenFile(sFilePath);
			m_lStreamLen = (int)oFile.GetFileSize();
			m_pStream = new BYTE[m_lStreamLen];
			oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen);
			oFile.CloseFile();

			reader = htmlReadMemory((char*)m_pStream, m_lStreamLen, "baseUrl", NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | 
				HTML_PARSE_NOWARNING | HTML_PARSE_NONET);

            return true;
		}
        inline bool FromString(std::wstring& sXml)
		{
			Clear();
			UnicodeToUtf8(sXml, m_pStream, m_lStreamLen);

			reader = htmlReadMemory((char*)m_pStream, m_lStreamLen, "baseUrl", "utf-8", HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | 
				HTML_PARSE_NOWARNING | HTML_PARSE_NONET);

            return true;
		}

        std::wstring Parse()
		{
            std::wstring strRet = _T("");

			xmlNodePtr root = xmlDocGetRootElement(reader);

			strRet = ParseNode(root);

			return strRet;
        }

        std::wstring ParseNode(xmlNodePtr root)
		{
            std::wstring strRet = _T("");

			XmlNodeType oNodeType = XmlNodeType_None;
			xmlNodePtr curNode = root;

			for (; curNode; curNode = curNode->next)
			{
				int type = curNode->type;
				
				if (XmlNodeType_Element == curNode->type)
				{
					strRet += _T("<");
                    std::wstring sName(curNode->name);
					strRet += sName;
					strRet += _T(">");

                    std::wstring strRet2 = ParseNode(curNode->children);
					strRet += strRet2;

					strRet += _T("</");
					strRet += sName;
					strRet += _T(">");
				}
			}

			return strRet;
		}

	private:
        void UnicodeToUtf8(std::wstring& strXml, BYTE*& pBuffer, LONG& lLen)
		{
			int nLength = strXml.GetLength();

			pBuffer = new BYTE[nLength*3 + 1];
			
			// Encoding Unicode to UTF-8
			WideCharToMultiByte(CP_UTF8, 0, strXml.GetBuffer(), nLength + 1, (LPSTR)pBuffer, nLength*3, NULL, NULL);
			lLen = strlen((LPSTR)pBuffer);
		}
	};
}

#endif
