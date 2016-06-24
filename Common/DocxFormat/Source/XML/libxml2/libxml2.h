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

#include "../../XML/stringcommon.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "../Utils.h"

namespace XmlUtils
{
	//class CStringWriter
	//{
	//private:
	//	wchar_t*	m_pData;
	//	size_t		m_lSize;

	//	wchar_t*	m_pDataCur;
	//	size_t		m_lSizeCur;

	//	int			m_lBinaryFactor;

	//public:
	//	CStringWriter()
	//	{
	//		m_pData = NULL;
	//		m_lSize = 0;

	//		m_pDataCur	= m_pData;
	//		m_lSizeCur	= m_lSize;

    //		m_bInitTable = false;

	//		m_lBinaryFactor = (((sizeof(wchar_t)) >> 1));
	//	}
	//	~CStringWriter()
	//	{
	//		RELEASEMEM(m_pData);
	//	}

 //       AVSINLINE void AddSize(size_t nSize)
	//	{
	//		if (NULL == m_pData)
	//		{
	//			m_lSize = max(nSize, 1000);				
	//			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

	//			m_lSizeCur = 0;
	//			m_pDataCur = m_pData;
	//			return;
	//		}

	//		if ((m_lSizeCur + nSize) > m_lSize)
	//		{
	//			while ((m_lSizeCur + nSize) > m_lSize)
	//			{
	//				m_lSize *= 2;
	//			}

	//			wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
	//			if (NULL != pRealloc)
	//			{
	//				// ������� ��������
	//				m_pData		= pRealloc;
	//				m_pDataCur	= m_pData + m_lSizeCur;
	//			}
	//			else
	//			{
	//				wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
	//				memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

	//				free(m_pData);
	//				m_pData		= pMalloc;
	//				m_pDataCur	= m_pData + m_lSizeCur;
	//			}
	//		}
	//	}

	//public:

 //       AVSINLINE void WriteString(const wchar_t* pString, const size_t& nLen)
	//	{
	//		AddSize(nLen);
	//		memcpy(m_pDataCur, pString, nLen << m_lBinaryFactor);
	//		m_pDataCur += nLen;
	//		m_lSizeCur += nLen;
	//	}
 //       AVSINLINE void WriteString(const std::wstring& bsString)
	//	{
	//		WriteString(bsString.c_str(), (size_t)bsString.length());
	//	}
	//	
 //       AVSINLINE void AddCharSafe(const TCHAR& _c)
	//	{
	//		AddSize(1);
	//		*m_pDataCur++ = _c;
	//		++m_lSizeCur;
	//	}
 //       AVSINLINE void AddChar2Safe(const TCHAR _c1, const TCHAR& _c2)
	//	{
	//		AddSize(2);
	//		*m_pDataCur++ = _c1;
	//		*m_pDataCur++ = _c2;
	//		m_lSizeCur += 2;
	//	}

	//	inline void WriteEncodeXmlString(const std::wstring& _string)
	//	{
	//		WriteEncodeXmlString(_string.c_str());
	//	}

	//	inline void WriteEncodeXmlString(const wchar_t* pString)
	//	{
	//		const wchar_t* pData = pString;
	//		while (*pData != 0)
	//		{
	//			BYTE _code = CheckCode(*pData);

	//			switch (_code)
	//			{
	//			case 1:
	//				AddCharSafe(*pData);
	//				break;
	//			case 0:
	//				AddCharSafe((WCHAR)' ');
	//				break;
	//			case 2:
	//				AddSize(5);
	//				*m_pDataCur++ = (WCHAR)('&');
	//				*m_pDataCur++ = (WCHAR)('a');
	//				*m_pDataCur++ = (WCHAR)('m');
	//				*m_pDataCur++ = (WCHAR)('p');
	//				*m_pDataCur++ = (WCHAR)(';');
	//				m_lSizeCur += 5;
	//				break;
	//			case 3:
	//				AddSize(6);
	//				*m_pDataCur++ = (WCHAR)('&');
	//				*m_pDataCur++ = (WCHAR)('a');
	//				*m_pDataCur++ = (WCHAR)('p');
	//				*m_pDataCur++ = (WCHAR)('o');
	//				*m_pDataCur++ = (WCHAR)('s');
	//				*m_pDataCur++ = (WCHAR)(';');
	//				m_lSizeCur += 6;
	//				break;
	//			case 4:
	//				AddSize(4);
	//				*m_pDataCur++ = (WCHAR)('&');
	//				*m_pDataCur++ = (WCHAR)('l');
	//				*m_pDataCur++ = (WCHAR)('t');
	//				*m_pDataCur++ = (WCHAR)(';');
	//				m_lSizeCur += 4;
	//				break;
	//			case 5:
	//				AddSize(4);
	//				*m_pDataCur++ = (WCHAR)('&');
	//				*m_pDataCur++ = (WCHAR)('g');
	//				*m_pDataCur++ = (WCHAR)('t');
	//				*m_pDataCur++ = (WCHAR)(';');
	//				m_lSizeCur += 4;
	//				break;
	//			case 6:
	//				AddSize(6);
	//				*m_pDataCur++ = (WCHAR)('&');
	//				*m_pDataCur++ = (WCHAR)('q');
	//				*m_pDataCur++ = (WCHAR)('u');
	//				*m_pDataCur++ = (WCHAR)('o');
	//				*m_pDataCur++ = (WCHAR)('t');
	//				*m_pDataCur++ = (WCHAR)(';');
	//				m_lSizeCur += 6;
	//				break;
	//			default:
	//				break;						
	//			}
	//		
	//			++pData;
	//		}
	//	}

 //       AVSINLINE size_t GetCurSize()
	//	{
	//		return m_lSizeCur;
	//	}

 //       AVSINLINE void Write(CStringWriter& oWriter)
	//	{
	//		WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
	//	}

	//	inline void Clear()
	//	{
	//		RELEASEMEM(m_pData);

	//		m_pData = NULL;
	//		m_lSize = 0;

	//		m_pDataCur	= m_pData;
	//		m_lSizeCur	= 0;
	//	}
	//	inline void ClearNoAttack()
	//	{
	//		m_pDataCur	= m_pData;
	//		m_lSizeCur	= 0;
	//	}

	//	std::wstring GetData()
	//	{
	//		std::wstring str(m_pData, (int)m_lSizeCur);
	//		return str;
	//	}

	//protected:
	//	BYTE m_arTableUnicodes[65536];
    //	bool m_bInitTable;

	//protected:
	//	BYTE CheckCode(const WCHAR& c)
	//	{
	//		if (!m_bInitTable)
	//		{
	//			memset(m_arTableUnicodes, 0, 65536);
	//			m_arTableUnicodes[0x0009] = 1;
	//			m_arTableUnicodes[0x000A] = 1;
	//			m_arTableUnicodes[0x000D] = 1;

	//			memset(m_arTableUnicodes + 0x0020, 1, 0xD7FF - 0x0020 + 1);
	//			memset(m_arTableUnicodes + 0xE000, 1, 0xFFFD - 0xE000 + 1);

	//			m_arTableUnicodes['&'] = 2;
	//			m_arTableUnicodes['\''] = 3;
	//			m_arTableUnicodes['<'] = 4;
	//			m_arTableUnicodes['>'] = 5;
	//			m_arTableUnicodes['\"'] = 6;

    //			m_bInitTable = true;
	//		}
	//		return m_arTableUnicodes[c];
	//	}
	//};	

	typedef 
	enum XmlNodeType
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

			NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.c_str(), sXml.length(), m_pStream, m_lStreamLen, false);
			reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

            return true;
		}
        inline bool FromStringA(const std::string& sXml)
		{
			Clear();

			reader = xmlReaderForMemory((char*)sXml.c_str(), sXml.length(), NULL, NULL, 0);

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
		inline const wchar_t* GetName()
		{
			if ( !IsValid() )
				return L"";

			xmlChar* pName = xmlTextReaderName(reader);
			if (NULL == pName)
				return L"";

			m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
			free(pName);
			return m_sTemp.c_str();
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

		inline CString GetText2()
		{
			if ( !IsValid() )
				return _T("");

			CString sResult;

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
		//inline std::wstring GetText3()
		//{
		//	if ( !IsValid() )
		//		return _T("");

		//	std::wstring sResult;

		//	if ( 0 != xmlTextReaderIsEmptyElement(reader) )
		//		return sResult;

		//	int nDepth = GetDepth();
		//	XmlNodeType eNodeType = XmlNodeType_EndElement;
		//	while ( Read( eNodeType ) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType )
		//	{
		//		if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
		//			sResult += GetText();
		//	}

		//	return sResult;
		//}
		inline CString GetOuterXml()
		{
			return GetXml(false);
		}
		inline CString GetInnerXml()
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
		inline CString GetXml(bool bInner)
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
							oResult.WriteEncodeXmlString(GetName());
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
			oResult.WriteEncodeXmlString(GetName());
			if(GetAttributesCount() > 0)
			{
				MoveToFirstAttribute();
				std::wstring sName = GetName();
				while( !sName.empty() )
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

		std::map<CStringA, CStringA> m_attributes;
		std::vector<CXmlNodeBase*> m_nodes;
		CString m_sText;
		CString m_sName;

	public:
		CXmlNodeBase() : IXmlDOMDocument()
		{
			m_pDocument = NULL;
			m_sText = L"";
			m_sName = L"";
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

		CString GetXml();
		void GetXml(CStringWriter& oWriter);
	};

    class CXmlNode;
    class CXmlNodes
    {
    private:
        std::vector<CXmlNode> m_nodes;

    public:
        CXmlNodes();
        bool IsValid();
        int GetCount();
        bool GetAt(int nIndex, CXmlNode& oXmlNode);

        friend class CXmlNode;
    };


	class CXmlNode
	{
	private:
		CXmlNodeBase* m_pBase;

	public:
		CXmlNode()
		{
			m_pBase = NULL;
		}
		~CXmlNode()
		{
			Clear();
		}
		CXmlNode(const CXmlNode& oSrc);

	public:
		bool FromXmlFile(const wchar_t* sFile, bool bRemoveRootNode = false)
		{
			return FromXmlFile(std::wstring(sFile), bRemoveRootNode);
		}
		bool FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode = false);
        bool FromXmlFile2(const CString& strXmlFilePath);
		bool FromXmlStringA(const std::string& sString);
		bool FromXmlString(const wchar_t* sString)
		{
			return FromXmlString(std::wstring(sString));
		}
		bool FromXmlString(const std::wstring& sString);

        bool IsValid() const;
		void Clear();
		CString GetName();
		CString GetText();
		bool GetTextIfExist(CString& sOutput);
		CString GetTextExt(const CString& strDefaultValue = _T(""));
		CString GetXml(const CString& strDefaultValue = _T(""));

		CString ReadAttributeBase(const wchar_t* bstrName);
		template<typename T>
		void ReadAttributeBase(const wchar_t* bsName, T& value)
		{
			CString sAttr;
			if(GetAttributeIfExist(CString(bsName), sAttr))
				value = sAttr.GetString();
		}
		CString ReadAttribute(const CString& strAttibuteName);
		template<typename T>
		void ReadAllAttributes(T& strNames, T& strValues)
		{
			if (!IsValid())
				return;

			std::map<CStringA, CStringA>::iterator p;
			for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
			{
                strNames.push_back(std_string2string(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.GetString(), p->first.GetLength(), true)));
                strValues.push_back(std_string2string(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.GetString(), p->second.GetLength(), true)));
			}
		}
		
		CStringA GetAttributeA(const CStringA& sName, const CStringA& _default = "");
		CStringA GetAttributeA(const CString& sName, const CStringA& _default = "");

		CString GetAttribute(const CStringA& sName, const CString& _default = L"");
		CString GetAttribute(const CString& sName, const CString& _default = L"");
		CString GetAttribute(const wchar_t* sName, const CString& _default = L"");
		CString GetAttributeBase(const wchar_t* strAttributeName, const CString& strDefaultValue = _T(""));
		bool GetAttributeIfExist(const CString& sName, CString& sOutput);

		int GetAttributeInt(const CStringA& sName, const int& _default = 0);
		int GetAttributeInt(const CString& sName, const int& _default = 0);
		int ReadAttributeInt(const CString& str, const int& nDef = 0);
		double ReadAttributeDouble(const CString& str, const double& nDef = 0);

		double GetAttributeDouble(const CStringA& sName, const double& _default = 0);
		double GetAttributeDouble(const CString& sName, const double& _default = 0);
		

		CString ReadNodeTextBase(const wchar_t* bsName);
		template <typename T>
		void ReadNodeValueBase(const wchar_t* bsName, T& value)
		{
			value = ReadNodeTextBase(bsName);
		}
		CString ReadNodeText(const CString& strName);
		CString ReadValueString(const CString& sName, const CString& nDef = _T(""));
		int ReadValueInt(const CString& str, const int& nDef = 0);

		CString GetAttributeOrValue(const CString& strAttributeName, const CString& strDefaultValue = _T(""));

        /*
        void LoadArray(const CString& sName, std::vector<T>& arList)
        {
            if (GetNodes(sName, oNodes))
        }
        */

		template <typename T>
        void LoadArray(const CString& sName, std::vector<T>& arList)
		{
			CXmlNodes oNodes;
			if (GetNodes(sName, oNodes))
            {
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					CXmlNode oItem;
					oNodes.GetAt(i, oItem);

                    //arList.Add ();
                    //arList[i].fromXML(oItem);
                    arList.push_back(T());
                    arList[i].fromXML(oItem);
				}
			}
		}
		template <typename T>
        void LoadArray(const CString& sName, const CString& sSubName, std::vector<T>& arList)
		{
			CXmlNode oNode;
			if (GetNode(sName, oNode))
				oNode.LoadArray(sSubName, arList);
		}
		CXmlNode ReadNode(const TCHAR* strNodeName);
		CXmlNode ReadNode(const CString& strNodeName);
		CXmlNode ReadNodeNoNS(const CString& strNodeName);

		CXmlNode GetNode(const CString& sName);
		CXmlNodes GetNodes(const CString& sName);
        bool GetChilds(CXmlNodes& oXmlNodes);

		bool GetNode(const CString& sName, CXmlNode& oNode);
		bool GetNodes(const CString& sName, CXmlNodes& oNodes);

		CXmlNode& operator=(const CXmlNode& oSrc);
	private:
		void SetBase(CXmlNodeBase* pBase);
		CString GetNamespace(const CString& strNodeName);
		CString GetNameNoNS(const CString& strNodeName);
	public:
		CString private_GetXml();
		CString private_GetXml(const CString& strDefaultValue = _T(""));
		CString private_GetXmlFast();
		CString private_GetXmlFast(const CString& strDefaultValue);
	};


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

        inline bool FromFile(CString& sFilePath)
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
        inline bool FromString(CString& sXml)
		{
			Clear();
			UnicodeToUtf8(sXml, m_pStream, m_lStreamLen);

			reader = htmlReadMemory((char*)m_pStream, m_lStreamLen, "baseUrl", "utf-8", HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | 
				HTML_PARSE_NOWARNING | HTML_PARSE_NONET);

            return true;
		}

		CString Parse()
		{
			CString strRet = _T("");

			xmlNodePtr root = xmlDocGetRootElement(reader);

			strRet = ParseNode(root);

			return strRet;
		}

		CString ParseNode(xmlNodePtr root)
		{
			CString strRet = _T("");

			XmlNodeType oNodeType = XmlNodeType_None;
			xmlNodePtr curNode = root;

			for (; curNode; curNode = curNode->next)
			{
				int type = curNode->type;
				
				if (XmlNodeType_Element == curNode->type)
				{
					strRet += _T("<");
					CString sName(curNode->name);
					strRet += sName;
					strRet += _T(">");

					CString strRet2 = ParseNode(curNode->children);
					strRet += strRet2;

					strRet += _T("</");
					strRet += sName;
					strRet += _T(">");
				}
			}

			return strRet;
		}

	private:
		void UnicodeToUtf8(CString& strXml, BYTE*& pBuffer, LONG& lLen)
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
