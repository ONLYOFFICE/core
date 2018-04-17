/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

	class KERNEL_DECL IXmlDOMDocument
	{
	private:
		unsigned int m_lRef;

	public:
		IXmlDOMDocument();
		virtual ~IXmlDOMDocument();

		virtual unsigned int AddRef();
		virtual unsigned int Release();
	};

	class CXmlNodeBase;
	class CXmlNode;
	class KERNEL_DECL CXmlNodes
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

		int GetAttributesCount();
		
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
        void LoadArray(const std::wstring& sName, std::vector<T>& arList);
        template <typename T>
        void LoadArray(const std::wstring& sName, const std::wstring& sSubName, std::vector<T>& arList);
        template<typename T>
        void ReadAttributeBase(const wchar_t* bsName, T& value);
        template<typename T>
        void ReadAllAttributes(T& strNames, T& strValues);
        template<typename T>
        void ReadAllAttributesA(T& strNames, T& strValues);
        template <typename T>
        void ReadNodeValueBase(const wchar_t* bsName, T& value);

	private:
		void SetBase(CXmlNodeBase* pBase);
		std::wstring GetNamespace(const std::wstring& strNodeName);
		std::wstring GetNameNoNS(const std::wstring& strNodeName);
	};


	class KERNEL_DECL CXmlWriter
	{
	private:
		std::wstring m_str;
	
	public:

		CXmlWriter();
		
		std::wstring GetXmlString();
		void SetXmlString(const std::wstring& strValue);
		
		bool SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/);
		void WriteString(const std::wstring& strValue);
		void WriteInteger(int Value);
		void WriteDouble(double Value);
		void WriteBoolean(bool Value);
		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false);
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true);
		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue);
		void WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"");
		void WriteNode(const std::wstring& strNodeName, double dValue);
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue);
		void WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = (L""));
		void WriteAttribute(const std::wstring& strAttributeName, double dValue);
	};

	std::wstring KERNEL_DECL GetNameNoNS(const std::wstring & strNodeName);
	std::wstring KERNEL_DECL GetNamespace(const std::wstring& strNodeName);
}

#endif // _BUILD_XMLUTILS_CROSSPLATFORM_H_
