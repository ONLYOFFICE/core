#pragma once

#pragma comment(lib, "libxml2.lib")

//#include "./win_build/config.h"
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

	//		m_bInitTable = FALSE;

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
	//				// реаллок сработал
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
	//	BOOL m_bInitTable;

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

	//			m_bInitTable = TRUE;
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
		~CXmlLiteReader()
		{
			if (NULL != m_pStream)
				delete []m_pStream;
		}

		xmlTextReaderPtr getNativeReader() { return reader; }

	public:

		inline void Clear()
		{
			if (NULL != m_pStream)
				delete []m_pStream;
			m_pStream = NULL;
			m_lStreamLen = 0;
		}

		inline BOOL IsValid()
		{
			return ( NULL != reader );
		}

		inline BOOL FromFile(const wchar_t* sFilePath)
		{
			return FromFile(std::wstring(sFilePath));
		}
		inline BOOL FromFile(const std::wstring& sFilePath)
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

			return TRUE;
		}
		inline BOOL FromString(const wchar_t* sXml)
		{
			return FromString(std::wstring(sXml));
		}
		inline BOOL FromString(const std::wstring& sXml)
		{
			Clear();

			NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.c_str(), sXml.length(), m_pStream, m_lStreamLen, false);
			reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

			return TRUE;
		}
		inline BOOL FromStringA(const std::string& sXml)
		{
			Clear();

			reader = xmlReaderForMemory((char*)sXml.c_str(), sXml.length(), NULL, NULL, 0);

			return TRUE;
		}
		inline BOOL Read(XmlNodeType &oNodeType)
		{
			if ( !IsValid() )
				return FALSE;

			if ( 0 == xmlTextReaderRead(reader) )
				return FALSE;

			oNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);

			return TRUE;
		}
		inline BOOL ReadNextNode()
		{
			if ( !IsValid() )
				return FALSE;

			XmlNodeType oNodeType = XmlNodeType_None;
			
			while ( XmlNodeType_Element != oNodeType )
			{
				if (1 != xmlTextReaderRead(reader))
					break;

				oNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);
			}

			if ( XmlNodeType_Element == oNodeType )
				return TRUE;

			return FALSE;
		}
		inline BOOL ReadNextSiblingNode(int nDepth)
		{
			// Перед использованием этой функции надо проверить,
			// пустая ли родительская нода. 
			if ( !IsValid() )
				return FALSE;

			XmlNodeType eNodeType = XmlNodeType_None;
			int nCurDepth = -1;
			
			while ( xmlTextReaderRead(reader) )
			{
				eNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);
				nCurDepth = xmlTextReaderDepth(reader);

				if (nCurDepth <= nDepth)
					break;

				if ( XmlNodeType_Element == eNodeType && nCurDepth == nDepth + 1 )
					return TRUE;
				else if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth + 1 )
					return FALSE;
			}

			return FALSE;
		}
		inline BOOL ReadTillEnd(int nDepth = -2)
		{
			if ( !IsValid() )
				return FALSE;

			if ( -2 == nDepth )
				nDepth = GetDepth();
			else if ( nDepth == GetDepth() && xmlTextReaderIsEmptyElement(reader) )
				return TRUE;

			XmlNodeType eNodeType = XmlNodeType_None;

			int nCurDepth = -1;
			// У закрывающего тэга глубина на 1 больше, чем у открывающего
			while( TRUE )
			{
				if ( 0 == xmlTextReaderRead(reader) )
					break;

				eNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);

				nCurDepth = GetDepth();
				if ( nCurDepth <= nDepth )
					break;
				
				if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth + 1 )
					break;
			}

			return TRUE;
		}
		inline const wchar_t* GetName()
		{
			if ( !IsValid() )
				return L"";

			xmlChar* pName = xmlTextReaderName(reader);
			if (NULL == pName)
				return L"";

			m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
			delete [] pName;
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
			delete [] pName;
			return m_sTempA.c_str();
		}
		inline int GetDepth()
		{
			if ( !IsValid() )
				return -1;

			return xmlTextReaderDepth(reader);
		}
		inline BOOL IsEmptyNode()
		{
			if ( !IsValid() )
				return FALSE;

			return xmlTextReaderIsEmptyElement(reader);
		}

		inline const wchar_t* GetText()
		{
			if ( !IsValid() )
				return NULL;

			xmlChar* pValue = xmlTextReaderValue(reader);
			if (NULL == pValue)
				return L"";
			
			m_sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen((const char*)pValue));
			delete [] pValue;
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
			delete [] pValue;
			return m_sTempA.c_str();
		}

		inline CString GetText2()
		{
			if ( !IsValid() )
				return _T("");

			CString sResult;

			if ( xmlTextReaderIsEmptyElement(reader) )
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
		inline BOOL MoveToFirstAttribute()
		{
			if ( !IsValid() )
				return FALSE;

			return (BOOL)xmlTextReaderMoveToFirstAttribute(reader);
		}
		inline BOOL MoveToNextAttribute()
		{
			if ( !IsValid() )
				return FALSE;

			return (BOOL)xmlTextReaderMoveToNextAttribute(reader);;
		}

		inline BOOL MoveToElement()
		{
			if ( !IsValid() )
				return FALSE;

			return (BOOL)xmlTextReaderMoveToElement(reader);
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
				// У закрывающего тэга глубина такая же как у открывающегося
				while( TRUE )
				{
					if ( 0 == xmlTextReaderRead(reader) )
						break;

					eNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);

					nCurDepth = GetDepth();
					if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace )
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
					if ( nCurDepth <= nDepth )
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
					delete pNode;
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

	class CXmlNodes;
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
		bool FromXmlStringA(const std::string& sString);
		bool FromXmlString(const wchar_t* sString)
		{
			return FromXmlFile(std::wstring(sString));
		}
		bool FromXmlString(const std::wstring& sString);

		bool IsValid();
		void Clear();
		CString GetName();
		CString GetText();

		CString ReadAttributeBase(const wchar_t* bstrName);
		template<typename T>
		void ReadAttributeBase(const wchar_t* bsName, T& value)
		{
			value = GetAttribute(CString(bsName)).GetString();
		}
		CString ReadAttribute(const CString& strAttibuteName);
		template<typename T>
		void ReadAllAttributes(T& strNames, T& strValues)
		{
			if (!IsValid())
				return;

			std::map<CStringA, CStringA>::iterator p;
			for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
			{
				p->first.c_str(), p->first.length()
					strNames.push_back(p->first);
				strValues.push_back(p->second);
			}
		}
		
		CStringA GetAttributeA(const CStringA& sName, const CStringA& _default = "");
		CStringA GetAttributeA(const CString& sName, const CStringA& _default = "");

		CString GetAttribute(const CStringA& sName, const CString& _default = L"");
		CString GetAttribute(const CString& sName, const CString& _default = L"");
		CString GetAttribute(const wchar_t* sName, const CString& _default = L"");
		
		int GetAttributeInt(const CStringA& sName, const int& _default = 0);
		int GetAttributeInt(const CString& sName, const int& _default = 0);
		int ReadAttributeInt(const CString& str, const int& nDef = 0);

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

					arList.reserve(arList.size() + 1);
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
		CXmlNode ReadNode(const CString& strNodeName);
		CXmlNode ReadNodeNoNS(const CString& strNodeName);

		CXmlNode GetNode(const CString& sName);
		CXmlNodes GetNodes(const CString& sName);

		bool GetNode(const CString& sName, CXmlNode& oNode);
		bool GetNodes(const CString& sName, CXmlNodes& oNodes);

		CXmlNode& operator=(const CXmlNode& oSrc);
	private:
		CString GetNamespace(const CString& strNodeName);
		CString GetNameNoNS(const CString& strNodeName);
	public:
		CString private_GetXml();
		CString private_GetXmlFast();
	};

	class CXmlNodes
	{
	private:
		std::vector<CXmlNode> m_nodes;

	public:
		CXmlNodes() : m_nodes()
		{
		}

		int GetCount()
		{
			return (int)m_nodes.size();
		}
		bool GetAt(int nIndex, CXmlNode& oXmlNode)
		{
			if (nIndex < 0 && nIndex >= GetCount())
				return false;

			oXmlNode = m_nodes[nIndex];
			return true;
		}

		friend class CXmlNode;
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

		inline BOOL IsValid()
		{
			return ( NULL != reader );
		}

		inline BOOL FromFile(CString& sFilePath)
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

			return TRUE;
		}
		inline BOOL FromString(CString& sXml)
		{
			Clear();
			UnicodeToUtf8(sXml, m_pStream, m_lStreamLen);

			reader = htmlReadMemory((char*)m_pStream, m_lStreamLen, "baseUrl", "utf-8", HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | 
				HTML_PARSE_NOWARNING | HTML_PARSE_NONET);

			return TRUE;
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
