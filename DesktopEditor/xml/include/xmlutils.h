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
#ifndef _BUILD_XMLUTILS_CROSSPLATFORM_H_
#define _BUILD_XMLUTILS_CROSSPLATFORM_H_

#include "xmlwriter.h"

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

	typedef enum {
		XML_C14N_1_0            = 0,    /* Origianal C14N 1.0 spec */
		XML_C14N_EXCLUSIVE_1_0  = 1,    /* Exclusive C14N 1.0 spec */
		XML_C14N_1_1            = 2     /* C14N 1.1 spec */
	} xmlC14NMode;

	class CXmlLiteReader_Private;
	class KERNEL_DECL CXmlLiteReader
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

		bool MoveToStart();

		bool Read(XmlNodeType &oNodeType);
		bool ReadNextNode();
		bool ReadNextSiblingNode(int nDepth);
		bool ReadNextSiblingNode2(int nDepth);
		bool ReadTillEnd(int nDepth = -2);
		
		std::wstring	GetName();
		std::string     GetNameA();
        std::wstring	GetNameNoNS();
        std::string     GetNameNoNSA();

		const char*     GetNameChar();
		int             GetDepth();
		bool            IsEmptyNode();

		std::wstring	GetText();
		std::string     GetTextA();
		const char*		GetTextChar();
		std::wstring	GetAttributeTextWithHHHH();

		std::wstring	GetText2();
		std::string     GetText2A();
		
		std::wstring	GetText3();
		void GetTextWithHHHH(bool bPreserve, wchar_t*& sBuffer, long& nSize, long& nLen);
		std::wstring GetTextWithHHHH(bool bPreserve);

		std::wstring	GetOuterXml();
		std::wstring	GetInnerXml();

		int GetAttributesCount();
		bool MoveToFirstAttribute();
		bool MoveToNextAttribute();
		bool MoveToElement();

		bool IsEmptyElement();

		std::wstring GetNamespacePrefix();
		XmlNodeType GetNodeType();
		bool IsDefaultAttribute();
	};

	class KERNEL_DECL IXmlDOMDocument
	{
	private:
		unsigned int m_lRef;

	public:
		IXmlDOMDocument();
		virtual ~IXmlDOMDocument();

		virtual unsigned int AddRef();
		virtual unsigned int Release();

		static void DisableOutput();
		static void EnableOutput();
	};

	class CXmlNodeBase;
	class CXmlNode;
	class KERNEL_DECL CXmlNodes
	{
	private:
		std::vector<CXmlNode> m_nodes;

	public:
		CXmlNodes();
		~CXmlNodes();
		bool IsValid();
		int GetCount();
		bool GetAt(int nIndex, CXmlNode& oXmlNode);

		friend class CXmlNode;
	};


	class KERNEL_DECL CXmlNode
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
		void ReadAllAttributesA(std::vector<std::string>& strNames, std::vector<std::string>& strValues);
		void ReadAllAttributes(std::vector<std::wstring>& strNames, std::vector<std::wstring>& strValues);

		int GetAttributesCount();
		void GetAllAttributes(std::vector<std::wstring>& names, std::vector<std::wstring>& values);
		void GetAllAttributes(std::vector<std::string>& names, std::vector<std::string>& values);
		void GetAllAttributes(std::list<std::wstring>& names, std::list<std::wstring>& values);
		void GetAllAttributes(std::list<std::string>& names, std::list<std::string>& values);
		
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

	private:
		void SetBase(CXmlNodeBase* pBase);
		std::wstring GetNamespace(const std::wstring& strNodeName);
		std::wstring GetNameNoNS(const std::wstring& strNodeName);
	};

	std::wstring KERNEL_DECL GetNameNoNS(const std::wstring & strNodeName);
	KERNEL_DECL const char* GetNameNoNS(const char* strNodeName);
	std::wstring KERNEL_DECL GetNamespace(const std::wstring& strNodeName);

    #define XmlMacroLoadArray(node, name, list, type)   \
    {                                                   \
        XmlUtils::CXmlNodes oNodes;                     \
        if (node.GetNodes(name, oNodes))                \
        {                                               \
            int nCount = oNodes.GetCount();             \
            for (int i = 0; i < nCount; ++i)            \
            {                                           \
                XmlUtils::CXmlNode oItem;               \
                oNodes.GetAt(i, oItem);                 \
                list.push_back(type());                 \
                list[i].fromXML(oItem);                 \
            }                                           \
        }                                               \
    }
    #define XmlMacroLoadArrayS(node, name, subname, list, type)     \
    {                                                               \
        XmlUtils::CXmlNode oNode;                                   \
        if (node.GetNode(sName, oNode))                             \
            LoadArrayMacro(oNode, subname, list, type);             \
    }
    #define XmlMacroReadAttributeBase(node, name, value)    \
    {                                                       \
        std::wstring sAttr;                                 \
        if (node.GetAttributeIfExist(name, sAttr))          \
            value = sAttr;                                  \
    }
    #define XmlMacroReadNodeValueBase(node, name, value)    \
    {                                                       \
        value = node.ReadNodeTextBase(name);                \
    }

    namespace NSXmlCanonicalizator
    {
        std::string KERNEL_DECL Execute(const std::string& sXml, int mode = XML_C14N_1_0, bool withComments = false);
        std::string KERNEL_DECL Execute(const std::wstring& sXmlFile, int mode = XML_C14N_1_0, bool withComments = false);
    }

    // UTF-8 BOM, UTF-16BE BOM, UTF-16LE BOM, UTF-32BE BOM, UTF-32LE BOM
    std::string KERNEL_DECL GetUtf8FromFileContent(unsigned char* pData, unsigned int len);
}

#endif // _BUILD_XMLUTILS_CROSSPLATFORM_H_
