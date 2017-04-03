/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#ifndef _BUILD_XMLUTILS_CROSSPLATFORM_H_
#define _BUILD_XMLUTILS_CROSSPLATFORM_H_

#include <vector>
#include <map>
#include <string>

#include "../../common/File.h"
#include "../../common/StringBuilder.h"

namespace XmlUtils
{
	typedef enum XmlNodeType
	{
		XmlNodeType_None = 0,
		XmlNodeType_Element = 1,
		XmlNodeType_Attribute = 2,
		XmlNodeType_Text = 3,
		XmlNodeType_CDATA = 4,
		XmlNodeType_ENTITY_REFERENCE = 5,
		XmlNodeType_ENTITY = 6,
		XmlNodeType_ProcessingInstruction = 7,
		XmlNodeType_Comment = 8,
		XmlNodeType_Document = 9,
		XmlNodeType_DocumentType = 10,
		XmlNodeType_DOCUMENT_FRAGMENT = 11,
		XmlNodeType_NOTATION = 12,
		XmlNodeType_Whitespace = 13,
		XmlNodeType_SIGNIFICANT_WHITESPACE = 14,
		XmlNodeType_EndElement = 15,
		XmlNodeType_TYPE_END_ENTITY = 16,
		XmlNodeType_XmlDeclaration = 17,
		_XmlNodeType_Last = 17
	} XmlNodeType;

	class CXmlLiteReader_Private;
	class CXmlLiteReader
	{
	protected:
		CXmlLiteReader_Private* m_pInternal;
	public:

		CXmlLiteReader();
		virtual ~CXmlLiteReader();
		
        public:

                void Clear();
		bool IsValid();

		bool FromFile	(const wchar_t* sFilePath);
		bool FromFile	(const std::wstring& sFilePath);
		bool FromString	(const wchar_t* sXml);
		bool FromString	(const std::wstring& sXml);
		bool FromStringA(const std::string& sXml);

		bool Read(XmlNodeType &oNodeType);
		bool ReadNextNode();
		bool ReadNextSiblingNode(int nDepth);
		bool ReadNextSiblingNode2(int nDepth);
		bool ReadTillEnd(int nDepth = -2);
		
		std::wstring	GetName();
        std::string     GetNameA();
        int             GetDepth();
        bool            IsEmptyNode();

		std::wstring	GetText();
        std::string     GetTextA();

		std::wstring	GetText2();
        std::string     GetText2A();
		
		std::wstring	GetText3();

		std::wstring	GetOuterXml();
		std::wstring	GetInnerXml();

		int GetAttributesCount();
		bool MoveToFirstAttribute();
		bool MoveToNextAttribute();
		bool MoveToElement();

        bool IsEmptyElement();
	};

	class IXmlDOMDocument
	{
	private:
		unsigned int m_lRef;

	public:
		IXmlDOMDocument()
		{
			m_lRef = 1;
		}
		virtual ~IXmlDOMDocument()
		{
		}

		virtual unsigned int AddRef()
		{
			++m_lRef;
			return m_lRef;
		}
		virtual unsigned int Release()
		{
			unsigned int lReturn = --m_lRef;
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
		CXmlNodeBase();
		~CXmlNodeBase();

		void AddRefDoc();
		void ReleaseDoc();

		std::wstring GetXml();
		void GetXml(NSStringUtils::CStringBuilder& oWriter);
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
		CXmlNode();
		CXmlNode(const CXmlNode& oSrc);
		~CXmlNode();
		
	public:
		bool FromXmlFile(const wchar_t* sFile, bool bRemoveRootNode = false);
		bool FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode = false);
		
		bool FromXmlStringA(const std::string& sString);
		bool FromXmlString(const wchar_t* sString);
		bool FromXmlString(const std::wstring& sString);

		bool IsValid() const;
		void Clear();
		std::wstring GetName();
		std::wstring GetText();
		bool GetTextIfExist(std::wstring& sOutput);
		std::wstring GetTextExt(const std::wstring& strDefaultValue = L"");
		std::wstring GetXml(const std::wstring& strDefaultValue = L"");

		std::wstring ReadAttributeBase(const wchar_t* bstrName);		
		std::wstring ReadAttribute(const std::wstring& strAttibuteName);
		
		std::string GetAttributeA(const std::string& sName, const std::string& _default = "");
		std::string GetAttributeA(const std::wstring& sName, const std::string& _default = "");

		std::wstring GetAttribute(const std::string& sName, const std::wstring& _default = L"");
		std::wstring GetAttribute(const std::wstring& sName, const std::wstring& _default = L"");
		std::wstring GetAttribute(const wchar_t* sName, const std::wstring& _default = L"");

		std::wstring GetAttributeBase(const wchar_t* strAttributeName, const std::wstring& strDefaultValue = L"");
		bool GetAttributeIfExist(const std::wstring& sName, std::wstring& sOutput);

		int GetAttributeInt(const std::string& sName, const int& _default = 0);
		int GetAttributeInt(const std::wstring& sName, const int& _default = 0);
		
		bool SetAttributeInt(const std::wstring& sName, const int& value);
		
		int ReadAttributeInt(const std::wstring& str, const int& nDef = 0);
		double ReadAttributeDouble(const std::wstring& str, const double& nDef = 0);

		double GetAttributeDouble(const std::string& sName, const double& _default = 0);
		double GetAttributeDouble(const std::wstring& sName, const double& _default = 0);

		std::wstring ReadNodeTextBase(const wchar_t* bsName);		
		
		std::wstring ReadNodeText(const std::wstring& strName);
		std::wstring ReadValueString(const std::wstring& sName, const std::wstring& nDef = L"");
		int ReadValueInt(const std::wstring& str, const int& nDef = 0);

		std::wstring GetAttributeOrValue(const std::wstring& strAttributeName, const std::wstring& strDefaultValue = L"");

		CXmlNode ReadNode(const wchar_t* strNodeName);
		CXmlNode ReadNode(const std::wstring& strNodeName);
		CXmlNode ReadNodeNoNS(const std::wstring& strNodeName);
		CXmlNodes ReadNodesNoNS(const std::wstring& strNodeName);

		CXmlNode GetNode(const std::wstring& sName);
		CXmlNodes GetNodes(const std::wstring& sName);
		bool GetChilds(CXmlNodes& oXmlNodes);

		bool GetNode(const std::wstring& sName, CXmlNode& oNode);
		bool GetNodes(const std::wstring& sName, CXmlNodes& oNodes);

		CXmlNode& operator=(const CXmlNode& oSrc);

        public:
                std::wstring private_GetXml();
                std::wstring private_GetXml(const std::wstring& strDefaultValue = L"");
                std::wstring private_GetXmlFast();
                std::wstring private_GetXmlFast(const std::wstring& strDefaultValue);

        public:
                template <typename T>
                void LoadArray(const std::wstring& sName, std::vector<T>& arList)
                {
                    CXmlNodes oNodes;
                    if (GetNodes(sName, oNodes))
                    {
                        int nCount = oNodes.GetCount();
                        for (int i = 0; i < nCount; ++i)
                        {
                            CXmlNode oItem;
                            oNodes.GetAt(i, oItem);

                            arList.push_back(T());
                            arList[i].fromXML(oItem);
                        }
                    }
                }
                template <typename T>
                void LoadArray(const std::wstring& sName, const std::wstring& sSubName, std::vector<T>& arList)
                {
                    CXmlNode oNode;
                    if (GetNode(sName, oNode))
                        oNode.LoadArray(sSubName, arList);
                }
                template<typename T>
                void ReadAttributeBase(const wchar_t* bsName, T& value)
                {
                    std::wstring sAttr;
                    if (GetAttributeIfExist(bsName, sAttr))
                        value = sAttr;
                }
                template<typename T>
                void ReadAllAttributes(T& strNames, T& strValues)
                {
                    if (!IsValid())
                        return;

                    std::map<std::string, std::string>::iterator p;
                    for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
                    {
                        strNames.push_back  (NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->first.c_str(), (long)p->first.length()));
                        strValues.push_back (NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->second.c_str(), (long)p->second.length()));
                    }
                }
                template <typename T>
                void ReadNodeValueBase(const wchar_t* bsName, T& value)
                {
                    value = ReadNodeTextBase(bsName);
                }

	private:
		void SetBase(CXmlNodeBase* pBase);
		std::wstring GetNamespace(const std::wstring& strNodeName);
		std::wstring GetNameNoNS(const std::wstring& strNodeName);
	};


	class CXmlWriter
	{
		std::wstring m_str;
	
	public:

		CXmlWriter()
		{
		}
		
		std::wstring GetXmlString()
		{
			return m_str;
		}
		void SetXmlString(const std::wstring& strValue)
		{
			m_str = strValue;
		}
		
		bool SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/)
		{
			return NSFile::CFileBinary::SaveToFile(strFilePath, m_str);
//#if defined(_WIN32) || defined (_WIN64)
//            // win32 unicode and multibyte strings
//			FILE* pFile = _tfopen(strFilePath, (L"wt"));
//#else
//            // *nix build
//            #ifdef _UNICODE
//                std::string sFilePathUtf8 = stringWstingToUtf8String (strFilePath);
//                FILE* pFile = fopen(sFilePathUtf8.c_str(), "wt");
//            #else
//                // path is already utf8
//                std::string sFilePathUtf8 = strFilePath;
//                FILE* pFile = fopen(sFilePathUtf8.c_str(), "wt");
//            #endif
//#endif
//
//			if (!pFile)
//                                return false;
//
//			std::string str; str = m_str;
//			if (bEncodingToUTF8)
//#ifdef _UNICODE
//				str = EncodingUnicodeToUTF8();
//#else
//				str = EncodingASCIIToUTF8();
//#endif
//
//			fprintf(pFile, str);
//
//			fclose(pFile);
//
//			return true;
		}
//#ifdef _UNICODE
//        std::string EncodingUnicodeToUTF8()
//        {
//            //#if defined(_WIN32) || defined (_WIN64)
//            //    int nLength = m_str.GetLength();
//
//            //    // Encoding Unicode to UTF-8
//            //    std::string saStr;
//            //    WideCharToMultiByte(CP_UTF8, 0, m_str.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
//            //    saStr.ReleaseBuffer();
//            //    return saStr;
//            //#else
//
//                std::string sStrTempUtf8 = stringWstingToUtf8String (m_str);
//                std::string saStr = sStrTempUtf8;
//                return saStr;
//            //#endif
//        }
//#else
//		std::wstring EncodingASCIIToUTF8()
//        {
//            int nLength = m_str.GetLength();
//
//			wchar_t* pWStr = new wchar_t[nLength + 1];
//			if (!pWStr)
//				return (L"");
//
//			// set end string
//			pWStr[nLength] = 0;
//
//			// Encoding ASCII to Unicode
//            MultiByteToWideChar(CP_ACP, 0, m_str, nLength, pWStr, nLength);
//
//            int nLengthW = (int) wcslen(pWStr);
//
//			// Encoding Unicode to UTF-8
//			std::wstring cStr; 
//            WideCharToMultiByte(CP_UTF8, 0, pWStr, nLengthW + 1, cStr.GetBuffer(nLengthW*3 + 1), nLengthW*3, NULL, NULL);
//			cStr.ReleaseBuffer();
//    
//		    delete[] pWStr;
//
//            return cStr;
//        }
//#endif


		void WriteString(const std::wstring& strValue)
		{
			m_str += strValue;
		}
        void WriteInteger(int Value)
		{
            m_str += std::to_wstring(Value);
		}
		void WriteDouble(double Value)
		{
            m_str += std::to_wstring(Value);
		}
		void WriteBoolean(bool Value)
		{
			if (Value)
				m_str += (L"true");
			else
				m_str += (L"false");
		}
		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false)
		{
			m_str += (L"<") + strNodeName;

			if (!bAttributed)
				m_str += (L">");
		}
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_str += (L" />");
				else
					m_str += (L">");
			}
			else
				m_str += (L"</") + strNodeName + (L">");
		}
		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue)
		{
			if (strNodeValue.empty())
				m_str += L"<" + strNodeName + L"/>";
			else
				m_str += L"<" + strNodeName + L">" + strNodeValue + L"</" + strNodeName + L">";
		}
        void WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"")
		{
			WriteNodeBegin(strNodeName);
			WriteString(strTextBeforeValue);
            WriteInteger(nValue);
			WriteString(strTextAfterValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteNode(const std::wstring& strNodeName, double dValue)
		{
			WriteNodeBegin(strNodeName);
			WriteDouble(dValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue)
		{
			m_str += L" " + strAttributeName + L"=\"" + strAttributeValue + L"\"";
		}
        void WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = (L""))
		{
			WriteString(L" " + strAttributeName + L"=");
			WriteString(L"\"");
			WriteString(strTextBeforeValue);
            WriteInteger(nValue);
			WriteString(strTextAfterValue);
			WriteString(L"\"");
		}
		void WriteAttribute(const std::wstring& strAttributeName, double dValue)
		{
			WriteString(L" " + strAttributeName + L"=");
			WriteString(L"\"");
			WriteDouble(dValue);
			WriteString(L"\"");
		}
	};
	static inline std::wstring GetNameNoNS(const std::wstring & strNodeName)
	{
		int nFind = (int)strNodeName.find(L":");
		if (-1 == nFind)
			return strNodeName;
		return strNodeName.substr(nFind + 1);
	}
    static inline std::wstring GetNamespace(const std::wstring& strNodeName)
	{
		int nFind = (int)strNodeName.find(L":");
		if (-1 == nFind)
			return L"";
		return strNodeName.substr(0, nFind);
	}
}

#endif // _BUILD_XMLUTILS_CROSSPLATFORM_H_
